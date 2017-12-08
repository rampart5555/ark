#ifndef __SCENE_DEFS__
#define __SCENE_DEFS__
struct EntityProp
{
    int m_id;
    int m_color;
    int m_hits;
    int m_value;      
};

struct EntityProps
{
    std::vector<EntityProp> m_brick;
    std::vector<EntityProp> m_powerup;    
};

struct LevelData
{
    std::string m_name;
    int m_width;
    int m_height;
    std::vector<int> m_brick;
    std::vector<int> m_powerup;      
};
#endif


