#include <stdio.h>
#include <osgDB/ReadFile>
#include <osg/UserDataContainer>
#include <osg/ValueObject>
#include "AssetsManager.h"
#include "LevelManager.h"
#include "Logging.h"

LevelManager::LevelManager()
{
    m_osgFile=AssetsManager::instance().getRootPath() + "models/levels.osgt";    
    readLevelData();
}

LevelManager& LevelManager::instance()
{
    static LevelManager instance;
    return instance;
}

bool LevelManager::readLevelData()
{
    LOG_INFO("LevelManager::readSceneData=> %s\n",m_osgFile.c_str());
    osg::Object *obj = AssetsManager::instance().loadObject(m_osgFile.c_str(),"osgt");

    osg::ref_ptr<osg::Group> rootGroup = dynamic_cast<osg::Group*>(obj);

    unsigned int i,j;    
    SceneEpisode *ep;
    SceneLevel *lvl;
    if(rootGroup != NULL)
    {
        for(i=0;i<rootGroup->getNumChildren();i++)
        {
            osg::Group *ep_group = dynamic_cast<osg::Group*>(rootGroup->getChild(i));
            int ep_id;
            bool ep_res = ep_group->getUserValue("ep_id", ep_id);
            if(ep_res==true)
            {
                ep = new SceneEpisode;
                ep_group->getUserValue("ep_id", ep->m_id);                
                ep_group->getUserValue("ep_name", ep->m_title);
                ep_group->getUserValue("ep_unlocked", ep->m_unlocked);
                m_episodes.push_back(ep);
            }
            for(j=0;j<ep_group->getNumChildren();j++)
            {
                osg::Group *lvl_group = dynamic_cast<osg::Group*>(ep_group->getChild(j));
                int lvl_id;
                bool lvl_res = lvl_group->getUserValue("lvl_id", lvl_id);
                if(lvl_res==true)
                {
                    lvl=new SceneLevel;
                    lvl->m_epId = ep_id;
                    lvl_group->getUserValue("lvl_id", lvl->m_id);
                    lvl_group->getUserValue("lvl_unlocked", lvl->m_unlocked);
                    lvl_group->getUserValue("lvl_score", lvl->m_score);
                    lvl_group->getUserValue("lvl_data", lvl->m_file);
                    ep->m_levels.push_back(lvl);
                }                         
            }
        }
    }
    return true;
}

bool LevelManager::writeLevelData()
{
    return true;
}

SceneEpisode* LevelManager::getEpisode(unsigned int ep_id)
{
    if(ep_id<m_episodes.size())
        return m_episodes[ep_id];
    return NULL;
}

SceneLevel* LevelManager::getLevel(unsigned int ep_id, unsigned int lvl_id)
{
    if( (ep_id < m_episodes.size()) && ( lvl_id < m_episodes[ep_id]->m_levels.size()))  
        return m_episodes[ep_id]->m_levels[lvl_id];
    return NULL;
}

SceneLevel* LevelManager::getCurrentLevel()
{
    return getLevel(m_epId, m_lvlId);
}

void LevelManager::setCurrent(unsigned int ep_id, unsigned int lvl_id)
{
    m_epId  = ep_id;
    m_lvlId = lvl_id;    
}

void LevelManager::getCurrent(unsigned int& ep_id, unsigned int& lvl_id)
{
    ep_id = m_epId;
    lvl_id = m_lvlId;
}

void LevelManager::dump()
{    
    unsigned int i,j;
    SceneEpisode *ep;
    SceneLevel *lvl;
    for(i=0;i<m_episodes.size();i++)
    {
        ep=m_episodes[i];
        printf("Episode %d %s %d\n",ep->m_id,ep->m_title.c_str(),ep->m_unlocked);
        for(j=0; j<ep->m_levels.size(); j++)
        {
            lvl=ep->m_levels[j];
            printf("    Level %d %d %s \n",lvl->m_id,lvl->m_unlocked,lvl->m_file.c_str());
        }
    }
}

void LevelManager::unlockNextLevel(unsigned int& ep_id, unsigned int &lvl_id)
{    
    ep_id = m_epId;
    lvl_id = m_lvlId+1;
    SceneLevel *sl = getLevel(ep_id,lvl_id);
    if(sl!=NULL)
        sl->m_unlocked=true;
}