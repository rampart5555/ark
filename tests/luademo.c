#include <stdlib.h>
#include "../src/AssetsManagerLua.h"

char* read_file(const char *filename)
{
    char *buffer = 0;
    long length;
    FILE *fp = fopen (filename, "rb");
    
    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = (char*)malloc (length);
        fread (buffer, 1, length, fp);
        fclose (fp);
    }
    else
    {
        printf("File error: %s\n", filename);
    }
    return buffer;
}    

void test_menu_items()
{
    AssetsManagerLua assets_lua;
    const char *script = read_file("../../../android/assets/lua/menudef.lua");
    std::vector<MenuItem> vect;
    assets_lua.loadScript(script);
    assets_lua.loadMenuItems("MenuLevelComplete",&vect);
    for (unsigned i=0;i<vect.size();i++)
         assets_lua.dumpMenuItem(vect.at(i));
    assets_lua.close();
}

void test_entity_props()
{
    AssetsManagerLua assets_lua;
    const char *script = read_file("../../../android/assets/lua/entity_props.lua");
    EntityProps data;
    bool res = assets_lua.loadScript(script);
    if(res)
    {
        assets_lua.loadEntityProps("EntityProps",&data);
        assets_lua.close();
    }
#if 0    
    unsigned int i=0;
    printf("brick_sze_size:%d\n", data.m_brick.size());
    for(i=0; i<data.m_brick.size();i++)
    {
        printf("%3d\n", data.m_brick[i].m_id);
    }
    printf("powerup_size:%d\n", data.m_powerup.size());
    for(i=0; i<data.m_powerup.size();i++)
    {
        printf("%3d\n", data.m_powerup[i].m_id);
    }
#endif    
}

void test_level_data()
{
    AssetsManagerLua assets_lua;
    const char *script = read_file("../assets/levels/Episode_1.lua");
    LevelData data;
    bool res = assets_lua.loadScript(script);
    if(res)
    {
        assets_lua.loadLevelData("Level_02",&data);
        assets_lua.close();
        printf("%s bricks_size: %lu \n",data.m_name.c_str(),data.m_brick.size());
        for(unsigned int i=0; i<data.m_brick.size(); i++)
            printf("%d ", data.m_brick[i]);
        printf("\n");
    }
}

/* TODO: fix this */
void test_resource_data()
{
    AssetsManagerLua assets_lua;
    const char *script = read_file("../../../android/assets/lua/resources.lua");
    std::vector<ResourceItem> res_items;
    bool res = assets_lua.loadScript(script);
    if(res)
    {
        assets_lua.loadResources("Resources",&res_items);
        assets_lua.close();
    }
    
}

void test_animation_data()
{
    AssetsManagerLua assets_lua;
    const char *script = read_file("../assets/lua/animations.lua");
    std::vector<Animation> anims;
    bool res = assets_lua.loadScript(script);
    if(res)
    {
        assets_lua.loadAnimations("Animations",&anims);
        assets_lua.close();
    }
    
}

int main()
{
    //test_resource_data();
    //test_entity_props();
    //test_level_data();
    test_animation_data();
}
