#ifndef __LEVEL_MANAGER_2__
#define __LEVEL_MANAGER_2__
#include <string>
#include <vector>
#include <osg/Group>

typedef struct 
{    
    int m_id;
    int m_score;
    bool m_unlocked;    
    std::string m_name;

}LevelInfo;

typedef struct 
{        
    int m_score;   
    int m_lives;     
    std::string m_name;
    std::vector<LevelInfo> m_levels;

}SceneInfo;

class LevelManager2
{
    public:
        static LevelManager2& instance();
        std::string& getSceneFile();
        bool setSceneData(osg::Group*, SceneInfo*);
        bool getSceneData(osg::Group*, SceneInfo*);
        bool setLevelData(osg::Group*, LevelInfo*);
        bool getLevelData(osg::Group*, LevelInfo*);
        LevelInfo* getCurrentLevel();
        LevelInfo* getNextLevel();
        void setCurrentLevel(int);
        bool readSceneInfo();
        bool writeSceneInfo();
        void updateScore(int);
        int getScore();
        void resetScore();
        /*lives - a negative number will decrese the lives */        
        void updateLives(int);
        int  getLives();
        void dump();
    private:
        LevelManager2();
        SceneInfo m_sceneData;
        std::string m_osgFile;
        int m_currentLevel;
        int m_levelScore;
        int m_lives;   
};

void test_level_manager_2();
void build_scene_data_2(const char *);

#endif