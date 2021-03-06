#ifndef __SCENE_DATA__
#define __SCENE_DATA__

#define EPISODE_NR  3
#define SCENE_NR    15

#include <osg/Group>

/*new*/
typedef struct 
{
    int ep_id;
    std::string ep_name;
    bool ep_unlocked;        

}EpisodeInfo;

typedef struct 
{
    int lvl_id;
    int lvl_ep_id;
    bool lvl_unlocked;
    int lvl_score;
    std::string lvl_data;

}LevelInfo;

class SceneLoader
{
    public:
        static SceneLoader& instance()
        {
            static SceneLoader instance;
            return instance;
        }                         
        ~SceneLoader();
        void writeSceneData();
        void readSceneData();    
        void dumpSceneData();
        bool getLevelInfo(int , int, LevelInfo&);
        bool getEpisodeInfo(int, EpisodeInfo&);
        int getEpNr()  { return m_epNr; }
        int getLvlNr() { return m_lvlNr; }
        void setCurrentScene(int ep_id, int lvl_id);
        void getCurrentScene(int& ep_id, int& lvl_id);
        bool unlockNextLevel();
    private:
        SceneLoader();
        osg::Group *m_rootGroup;
        std::string m_osgtFile;
        int m_epNr; 
        int m_lvlNr;
        int m_currentEp;
        int m_currentLvl;

};

/* old */
typedef struct 
{
    int  m_id;
    bool m_unlocked;
    int  m_score;
    const char *m_mapFile;    
    
} SceneData;

typedef struct 
{
    int m_id;
    bool m_unlocked;
    const char *m_name;
    SceneData m_scene[SCENE_NR];

} EpisodeData;

extern EpisodeData Episode[EPISODE_NR];

void episode_dump(int ep_id);

#endif
