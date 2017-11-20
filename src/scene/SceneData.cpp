#include <stdio.h>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/io_utils>
#include <osg/UserDataContainer>

#include <osg/ValueObject>
#include "SceneData.h"
#include "AssetsManager.h"
#include "Logging.h"

static void build_scene_data(const char *osgt_file)
{
    char buf[20];
    osg::Group *root = new osg::Group();
    for(int i=0;i<EPISODE_NR;i++)
    {
        std::string ep_name="Episode_";
        osg::Group *episode = new osg::Group();
        episode->setUserValue("ep_id",i);
        sprintf(buf,"%d",i+1);
        ep_name += buf;
        episode->setUserValue("ep_name",ep_name);
        episode->setUserValue("ep_unlocked",false);
        for(int j=0;j<SCENE_NR;j++)
        {            
            osg::Group *level = new osg::Group();
            level->setUserValue("lvl_id",i);
            level->setUserValue("lvl_unlocked",false);
            level->setUserValue("lvl_score",0);
            std::string lvl_data = "levels/Level_";
            sprintf(buf,"%02d",j+1);
            lvl_data += buf;
            lvl_data +=".lua";
            level->setUserValue("lvl_data",lvl_data);
            episode->addChild(level);
        }
        root->addChild(episode);
    }
    AssetsManager::instance().writeNode(*root, "levels.osgt");
}

static void dump_episode_info(EpisodeInfo ei)
{
    printf("Episode\n");
    printf("ei_id: %d\n",ei.ep_id);
    printf("ei_name: %s\n",ei.ep_name.c_str());
    printf("ei_unlocked: %d\n",ei.ep_unlocked);
}

static void dump_level_info(LevelInfo li)
{
    printf("  Level\n");
    printf("    lvl_id: %d\n",li.lvl_id);
    printf("    lvl_unlocked: %d\n",li.lvl_unlocked);
    printf("    lvl_score: %d\n",li.lvl_score);
    printf("    lvl_data: %s\n",li.lvl_data.c_str());
}

SceneLoader::SceneLoader()
{
    m_osgtFile=AssetsManager::instance().getRootPath()+"models/levels.osgt";    
    //build_scene_data(m_osgtFile.c_str());
}

void SceneLoader::writeSceneData()
{
    AssetsManager::instance().writeNode(*m_rootGroup, m_osgtFile.c_str());    
}

void SceneLoader::readSceneData()
{
    LOG_INFO("SceneLoader::readSceneData=> %s\n",m_osgtFile.c_str());
    osg::Object *obj = AssetsManager::instance().loadObject(m_osgtFile.c_str(),"osgt");
    m_rootGroup = dynamic_cast<osg::Group*>(obj);    
}

void SceneLoader::dumpSceneData()
{
    unsigned int i,j;
    LevelInfo li;  
    EpisodeInfo ei;
    bool ep_res,lvl_res;
    if(m_rootGroup!=NULL)
    {
        for(i=0;i<m_rootGroup->getNumChildren();i++)
        {
            osg::Group *ep_group = dynamic_cast<osg::Group*>(m_rootGroup->getChild(i));            
            ep_res = ep_group->getUserValue("ep_id",ei.ep_id);
            if(ep_res==true)
            {
                ep_group->getUserValue("ep_name", ei.ep_name);
                ep_group->getUserValue("ep_unlocked", ei.ep_unlocked);
                dump_episode_info(ei);                
            }            
            for(j=0;j<ep_group->getNumChildren();j++)
            {
                osg::Node *lvl_group = dynamic_cast<osg::Group*>(ep_group->getChild(j));
                lvl_res = lvl_group->getUserValue("lvl_id",li.lvl_id);
                if(lvl_res==true)
                {
                    lvl_group->getUserValue("lvl_unlocked",li.lvl_unlocked);
                    lvl_group->getUserValue("lvl_score",li.lvl_score);
                    lvl_group->getUserValue("lvl_data",li.lvl_data);        
                    dump_level_info(li);
                }                         
            }
        }
    }
}

void SceneLoader::getLevelInfo(int ep_id,int lvl_id, LevelInfo& linfo)
{

}

EpisodeData Episode[EPISODE_NR]={

    {1, true, "Episode 1",
        {
            { 1,  true,  0, "levels/level_01.lua"},
            { 2,  false, 0, "levels/level_02.lua"},
            { 3,  false, 0, "levels/level_03.lua"},
            { 4,  false, 0, "levels/level_04.lua"},
            { 5,  false, 0, "levels/level_05.lua"},
            { 6,  false, 0, "levels/level_06.lua"},
            { 8,  false, 0, "levels/level_07.lua"},
            { 9,  false, 0, "levels/level_08.lua"},
            { 9,  false, 0, "levels/level_09.lua"},
            { 10, false, 0, "levels/level_10.lua"},
            { 11, false, 0, "levels/level_11.lua"},
            { 12, false, 0, "levels/level_12.lua"},
            { 13, false, 0, "levels/level_13.lua"},
            { 14, false, 0, "levels/level_14.lua"},
            { 15, false, 0, "levels/level_15.lua"}
         }
    },
    {2, false, "Episode 2",
        {
            { 1,  false, 0, "levels/level_01.lua"},
            { 2,  false, 0, "levels/level_01.lua"},
            { 3,  false, 0, "levels/level_01.lua"},
            { 4,  false, 0, "levels/level_01.lua"},
            { 5,  false, 0, "levels/level_01.lua"},
            { 6,  false, 0, "levels/level_01.lua"},
            { 8,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 10, false, 0, "levels/level_01.lua"},
            { 11, false, 0, "levels/level_01.lua"},
            { 12, false, 0, "levels/level_01.lua"},
            { 13, false, 0, "levels/level_01.lua"},
            { 14, false, 0, "levels/level_01.lua"},
            { 15, false, 0, "levels/level_01.lua"}
        }
    },
    {3, false, "Episode 3",
        {
            { 1,  true,  0, "levels/level_01.lua"},
            { 2,  false, 0, "levels/level_01.lua"},
            { 3,  false, 0, "levels/level_01.lua"},
            { 4,  false, 0, "levels/level_01.lua"},
            { 5,  false, 0, "levels/level_01.lua"},
            { 6,  false, 0, "levels/level_01.lua"},
            { 8,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 10, false, 0, "levels/level_01.lua"},
            { 11, false, 0, "levels/level_01.lua"},
            { 12, false, 0, "levels/level_01.lua"},
            { 13, false, 0, "levels/level_01.lua"},
            { 14, false, 0, "levels/level_01.lua"},
            { 15, true , 0, "levels/level_01.lua"}
        }
    }
};

void episode_dump(int ep_id)
{
    if(ep_id >= EPISODE_NR)
    {
        return;
    }
    EpisodeData *ep = &Episode[ep_id];
    printf("id: %d unlocked: %d name: %s\n", ep->m_id, ep->m_unlocked, ep->m_name);
}
