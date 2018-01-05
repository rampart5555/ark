#include <stdio.h>
#include <osgDB/ReadFile>
#include <osg/UserDataContainer>
#include <osg/ValueObject>
#include "AssetsManager.h"
#include "LevelManager.h"
#include "Logging.h"

void build_scene_data(const char *osgt_file)
{    
    char buf[20];    
    SceneEpisode ep_data;
    SceneLevel lvl_data;
    osg::Group *root = new osg::Group();

    for(int i=0;i<3;i++)
    {
        ep_data.m_id = i;
        ep_data.m_title="Episode ";        
        sprintf(buf,"%d",i+1);  
        ep_data.m_title += buf;        
        ep_data.m_file = "levels/episode_";
        ep_data.m_file += buf;
        ep_data.m_file += ".lua";
        ep_data.m_unlocked = false;
        osg::Group *episode = new osg::Group();

        LevelManager::instance().setEpisodeData(episode,&ep_data);
        
        for(int j=0;j<15;j++)
        {   
            lvl_data.m_id = j;         
                        
            if((i==0)&&(j==0))
                lvl_data.m_unlocked = true;
            else 
                lvl_data.m_unlocked = false;
            lvl_data.m_score=0;
            lvl_data.m_name = "Level_";            
            sprintf(buf,"%02d",j+1);
            lvl_data.m_name += buf;  
            lvl_data.m_epId = i;
            osg::Group *level = new osg::Group();
            LevelManager::instance().setLevelData(level, &lvl_data);
            episode->addChild(level);
        }
        root->addChild(episode);
    }
    
    AssetsManager::instance().writeNode(*root, osgt_file);
}

LevelManager::LevelManager()
{
    m_osgFile = AssetsManager::instance().getRootPath() + "levels/levels.osgt";    
    m_levelScore = 0;
    m_lives = 3;    
    readLevelData();
    //dump();
}

LevelManager& LevelManager::instance()
{
    static LevelManager instance;
    return instance;
}

std::string& LevelManager::getOsgtFile()
{
    return m_osgFile;
}

void LevelManager::setEpisodeData(osg::Group *group, SceneEpisode* ep_data)
{
    group->setUserValue("ep_id", ep_data->m_id);
    group->setUserValue("ep_title", ep_data->m_title);
    group->setUserValue("ep_file", ep_data->m_file);
    group->setUserValue("ep_unlocked", ep_data->m_unlocked);
}

void LevelManager::getEpisodeData(osg::Group *group, SceneEpisode* ep_data)
{
    group->getUserValue("ep_id", ep_data->m_id);
    group->getUserValue("ep_title", ep_data->m_title);
    group->getUserValue("ep_file", ep_data->m_file);
    group->getUserValue("ep_unlocked", ep_data->m_unlocked);
}

void LevelManager::setLevelData(osg::Group *group, SceneLevel *lvl_data)
{
    group->setUserValue("lvl_id", lvl_data->m_id);
    group->setUserValue("lvl_score", lvl_data->m_score);
    group->setUserValue("lvl_unlocked", lvl_data->m_unlocked);
    group->setUserValue("lvl_name", lvl_data->m_name);
}

void LevelManager::getLevelData(osg::Group *group, SceneLevel *lvl_data)
{
    group->getUserValue("lvl_id", lvl_data->m_id);
    group->getUserValue("lvl_score", lvl_data->m_score);
    group->getUserValue("lvl_unlocked", lvl_data->m_unlocked);
    group->getUserValue("lvl_name", lvl_data->m_name);
}

bool LevelManager::writeLevelData()
{
    unsigned int i,j;
    SceneEpisode *ep;
    SceneLevel *lvl;
    LOG_INFO("LevelManager::writeLevelData=> %s\n",m_osgFile.c_str());
    osg::ref_ptr<osg::Group> rootGroup = new osg::Group;
    for(i=0;i<m_episodes.size();i++)
    {
        osg::ref_ptr<osg::Group> ep_group = new  osg::Group;
        ep=m_episodes[i];
        setEpisodeData(ep_group,ep);                
        rootGroup->addChild(ep_group);
        for(j=0; j<ep->m_levels.size(); j++)
        {
            osg::ref_ptr<osg::Group> lvl_group = new osg::Group;
            lvl=ep->m_levels[j];
            setLevelData(lvl_group ,lvl);
            ep_group->addChild(lvl_group);
        }        
    }
    AssetsManager::instance().writeNode(*rootGroup, m_osgFile.c_str());        
    return true;
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
                getEpisodeData(ep_group,ep);
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
                    getLevelData(lvl_group,lvl);
                    ep->m_levels.push_back(lvl);
                }                         
            }
        }
    }
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

SceneLevel* LevelManager::getNextLevel()
{
    m_lvlId += 1;
    return getLevel(m_epId, m_lvlId);
}

void LevelManager::dump()
{    
    unsigned int i,j;
    SceneEpisode *ep;
    SceneLevel *lvl;
    for(i=0;i<m_episodes.size();i++)
    {
        ep=m_episodes[i];
        printf("Episode id:%d\n title:%s\n file:%s\n unlocked %d\n",
            ep->m_id, ep->m_title.c_str(),ep->m_file.c_str(), ep->m_unlocked);
        for(j=0; j<ep->m_levels.size(); j++)
        {
            lvl=ep->m_levels[j];
            printf("    Level %d %d %s \n",lvl->m_id,lvl->m_unlocked,lvl->m_name.c_str());
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

/* score */
void LevelManager::updateScore(int  sc)
{
    m_levelScore += sc;
}
int LevelManager::getScore()
{
    return m_levelScore;
}
void LevelManager::resetScore()
{
    m_levelScore = 0;
}
/*lives*/
void LevelManager::updateLives(int life)
{
    m_lives += life;
}
int LevelManager::getLives()
{
    return m_lives;
}