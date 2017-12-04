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
    std::string m_file;        
}SceneLevel;

typedef struct
{
    int m_id;
    bool m_unlocked;
    std::string m_title;
    std::vector <SceneLevel*> m_levels;

}SceneEpisode;

class LevelManager
{
    public:
        static LevelManager& instance();
        bool readLevelData();
        bool writeLevelData();
        void dump();
        SceneEpisode* getEpisode(unsigned int);
        SceneLevel* getLevel(unsigned  int, unsigned int);
        void setCurrent(unsigned int, unsigned int);
        void getCurrent(unsigned int&, unsigned int&);
        void unlockNextLevel();
    private:        
        LevelManager();
        std::vector<SceneEpisode*> m_episodes;
        std::string m_osgFile;
        unsigned int m_epId;
        unsigned int m_lvlId;                
};
#endif