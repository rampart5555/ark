#ifndef __SCENE_DATA__
#define __SCENE_DATA__

#define EPISODE_NR  3
#define SCENE_NR    15

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
