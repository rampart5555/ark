#ifndef __CONFIG__
#define __CONFIG__
#include <string>
const unsigned int MASK_2D = 0xF0000000;
const unsigned int MASK_3D = 0x0F000000;
const unsigned int MASK_FF = 0xFFFFFFFF;
const unsigned int MASK_00 = 0x00000000;

#define ORTHO_WIDTH 8.0

#define WIN_X 700.0
#define WIN_y 500.0
#define WIN_WIDTH 480.0
#define WIN_HEIGHT 640.0

typedef struct 
{
    std::string m_type;
    std::string m_id;
    std::string m_path1;
    std::string m_path2;
}ResourceItem;

typedef enum
{
    ASSET_TYPE_OSGT,
    ASSET_TYPE_VERTEX_SHADER,
    ASSET_TYPE_FRAGMENT_SHADER,
    ASSET_TYPE_JPEG,
    ASSET_TYPE_FREETYPE
    
}AssetType;

typedef struct 
{
    float x;
    float y;
    
}Event_XY;

typedef enum 
{
    ENTITY_NONE         = 0,
    ENTITY_WALL         = 1,
    ENTITY_BALL         = 2,
    ENTITY_BRICK        = 3,
    ENTITY_PADDLE       = 4,
    ENTITY_POWERUP      = 5,
    ENTITY_WALL_TOP     = 6,
    ENTITY_WALL_BOTTOM  = 7,
    ENTITY_WALL_LEFT    = 8,
    ENTITY_WALL_RIGHT   = 9,
    ENTITY_BACKGROUND   = 10,
    ENTITY_TURRET       = 11,
    ENTITY_CANNON       = 12,
    ENTITY_BULLET       = 13
    
    
}EntityType;

typedef enum
{
    POWERUP_NONE  = 0,
    POWERUP_BALLS = 1,
    POWERUP_FAST  = 2,
    POWERUP_SLOW  = 3,
    POWERUP_LIFE  = 4,
    POWERUP_CANNON = 5
}PowerupType;

#endif
