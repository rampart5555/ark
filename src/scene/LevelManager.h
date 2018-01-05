#ifndef __LEVEL_MANAGER__
#define __LEVEL_MANAGER__

#include <string>
#include <vector>

typedef struct 
{    
    int m_id;
    int m_epId;
    int m_score;
    bool m_unlocked;
    unsigned int m_widgetId;
    std::string m_name;

}SceneLevel;

typedef struct
{
    int m_id;
    bool m_unlocked;
    std::string m_title;
    std::string m_file;
    std::vector <SceneLevel*> m_levels;

}SceneEpisode;

class LevelManager
{
    public:
        static LevelManager& instance();
        std::string& getOsgtFile();
        bool readLevelData();
        bool writeLevelData();
        void dump();
        SceneEpisode* getEpisode(unsigned int);
        SceneLevel* getLevel(unsigned  int, unsigned int);
        SceneLevel* getCurrentLevel();
        SceneLevel* getNextLevel();
        void setCurrent(unsigned int, unsigned int);
        void getCurrent(unsigned int&, unsigned int&);
        void unlockNextLevel(unsigned int&, unsigned int&);
        void setEpisodeData(osg::Group *, SceneEpisode*);
        void setLevelData(osg::Group *, SceneLevel*);
        void getEpisodeData(osg::Group *, SceneEpisode*);
        void getLevelData(osg::Group *, SceneLevel*);
        //score
        void updateScore(int);
        int getScore();
        void resetScore();
        /*lifes - a negative number will decrese the lives */        
        void updateLives(int);
        int  getLives();
    private:        
        LevelManager();
        std::vector<SceneEpisode*> m_episodes;
        std::string m_osgFile;
        unsigned int m_epId;
        unsigned int m_lvlId;
        int m_levelScore;
        int m_lives;                
};

void build_scene_data(const char *);
#endif