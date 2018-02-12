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

const float DEFAULT_BALL_SPEED = 1.2;

typedef struct 
{
    std::string m_type;
    std::string m_id;
    std::string m_path;    
    
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
    ENTITY_NONE        ,
    ENTITY_WALL        ,
    ENTITY_BALL        ,
    ENTITY_BRICK       ,
    ENTITY_PADDLE      ,
    ENTITY_PADDLE_SPARE,
    ENTITY_POWERUP     ,
    ENTITY_WALL_TOP    ,
    ENTITY_WALL_BOTTOM ,
    ENTITY_WALL_LEFT   ,
    ENTITY_WALL_RIGHT  ,
    ENTITY_BACKGROUND  ,
    ENTITY_TURRET      ,
    ENTITY_CANNON      ,
    ENTITY_BULLET      ,
    ENTITY_DOOR_LEFT   ,
    ENTITY_DOOR_RIGHT  ,
    ENTITY_DOOR_LEFT_SENSOR,
    ENTITY_DOOR_RIGHT_SENSOR               
}EntityType;

/* map powerup from 17 to 25 */
typedef enum
{
    POWERUP_NONE   = 0,
    POWERUP_BALLS  = 17,
    POWERUP_FAST   = 18,
    POWERUP_SLOW   = 19,
    POWERUP_LIFE   = 20,
    POWERUP_CANNON = 21
    
}PowerupType;

#endif
