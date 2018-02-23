#include <stdio.h>
#include <osgDB/ReadFile>
#include <osg/UserDataContainer>
#include <osg/ValueObject>
#include "AssetsManager.h"
#include "Logging.h"
#include "LevelManager2.h"

void test_level_manager_2()
{
    std::string osgt_file = LevelManager2::instance().getSceneFile();
    build_scene_data_2(osgt_file.c_str());
    LevelManager2::instance().readSceneInfo();
    LevelManager2::instance().dump();
}

void build_scene_data_2(const char *osgt_file)
{
    char buf[20];            
    SceneInfo scene_info;
    LevelInfo level_info;

    osg::Group *scene_group = new osg::Group();
    scene_info.m_name="Ark Scene";
    scene_info.m_score=0;
    scene_info.m_lives=3;
    scene_info.m_currentLevel=0;
    
    LevelManager2::instance().setSceneData(scene_group, &scene_info);

    for(int i=0; i<30; i++)
    {
        osg::Group *level_group = new osg::Group();
        level_info.m_id = i;
        level_info.m_name="Level_";                    
        sprintf(buf,"%02d",i + 1);
        level_info.m_name += buf; 
        level_info.m_unlocked=false;
        level_info.m_score=0;        
        LevelManager2::instance().setLevelData(level_group,&level_info);
        scene_group->addChild(level_group);
    }
    AssetsManager::instance().writeNode(*scene_group, osgt_file);    
}

LevelManager2::LevelManager2()
{
    m_osgFile = AssetsManager::instance().getRootPath() + "levels/levels_info.osgt";    
    m_levelScore = 0;
    m_lives = 3;    
    readSceneInfo();
    //dump();
}

LevelManager2& LevelManager2::instance()
{
    static LevelManager2 instance;
    return instance;
}

std::string& LevelManager2::getSceneFile()
{
    return m_osgFile;
}

bool LevelManager2::setSceneData(osg::Group *group, SceneInfo *sc_data)
{    
    group->setUserValue("scene_score", sc_data->m_name);    
    group->setUserValue("scene_score", sc_data->m_score);    
    group->setUserValue("scene_lives", sc_data->m_lives);  
    group->setUserValue("scene_level", sc_data->m_currentLevel);  
    return true;      
}

bool LevelManager2::getSceneData(osg::Group *group, SceneInfo *sc_data)
{
    group->getUserValue("scene_score", sc_data->m_name);    
    group->getUserValue("scene_score", sc_data->m_score);    
    group->getUserValue("scene_lives", sc_data->m_lives);   
    group->getUserValue("scene_level", sc_data->m_currentLevel);  
    return true;
}

bool LevelManager2::setLevelData(osg::Group *group, LevelInfo *lvl_data)
{    
    group->setUserValue("level_id", lvl_data->m_id);
    group->setUserValue("level_score", lvl_data->m_score);
    group->setUserValue("level_unlocked", lvl_data->m_unlocked);
    group->setUserValue("level_name", lvl_data->m_name);
    return true;
}

bool LevelManager2::getLevelData(osg::Group *group, LevelInfo *lvl_data)
{
    group->getUserValue("level_id", lvl_data->m_id);
    group->getUserValue("level_score", lvl_data->m_score);
    group->getUserValue("level_unlocked", lvl_data->m_unlocked);
    group->getUserValue("level_name", lvl_data->m_name);
    return true;
}

bool LevelManager2::readSceneInfo()
{
    LOG_INFO("LevelManager::readSceneData=> %s\n",m_osgFile.c_str());

    osg::Object *obj = AssetsManager::instance().loadObject(m_osgFile.c_str(),"osgt");
    osg::ref_ptr<osg::Group> scene_group = dynamic_cast<osg::Group*>(obj);

    if(scene_group==NULL)
    {
        LOG_ERROR("LevelManager::readSceneData=> file not found %s\n",m_osgFile.c_str());
        return false;
    }    
    getSceneData(scene_group, &m_sceneData);

    unsigned int i;    
    for(i = 0; i<scene_group->getNumChildren(); i++)
    {
        osg::Group *level_group = dynamic_cast<osg::Group*>(scene_group->getChild(i));
        LevelInfo lvl;            
        getLevelData(level_group, &lvl);
        m_sceneData.m_levels.push_back(lvl);            
    }    

    return true;
}

bool LevelManager2::writeSceneInfo()
{
    unsigned int i;
 
    LOG_INFO("LevelManager::writeLevelData=> %s\n",m_osgFile.c_str());
    osg::ref_ptr<osg::Group> scene_group = new osg::Group;        
    setSceneData(scene_group, &m_sceneData);                    
    for(i=0; i<m_sceneData.m_levels.size(); i++)
    {
        osg::ref_ptr<osg::Group> level_group = new osg::Group;        
        setLevelData(level_group, &m_sceneData.m_levels[i]);
        scene_group->addChild(level_group);
    }                
    return true;
}

LevelInfo* LevelManager2::getCurrentLevel()
{
    if(m_currentLevel < m_sceneData.m_levels.size())
    {
        return &m_sceneData.m_levels[m_currentLevel];
    }
    return NULL;
}

LevelInfo* LevelManager2::getNextLevel()
{
    m_currentLevel++;
    if(m_currentLevel < m_sceneData.m_levels.size())
    {
        return &m_sceneData.m_levels[m_currentLevel];
    }
    return NULL;
}

void LevelManager2::dump()
{    
    unsigned int i;
    printf("Scene tile:%s\n lives:%d\n score:%d\n \n",
            m_sceneData.m_name.c_str(), m_sceneData.m_lives, m_sceneData.m_score);
    for(i=0;i<m_sceneData.m_levels.size();i++)
    {
        LevelInfo *lvl = &m_sceneData.m_levels[i];
        printf("Level id:%d\n title:%s\n unlocked %d\n",
            lvl->m_id, lvl->m_name.c_str(), lvl->m_unlocked);
        
    }
}