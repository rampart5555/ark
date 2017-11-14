#include <stdio.h>
#include "Logging.h"
//#include "AssetsManager.h"
#include "AssetsManagerLua.h"

static void stackDump (lua_State *L) 
{
      int i;
      int top = lua_gettop(L);
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
    
          case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;
    
          case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
    
          case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
    
          default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;
    
        }
        printf("  ");  /* put a separator */
      }
      printf("\n");  /* end the listing */
}

AssetsManagerLua::AssetsManagerLua()
{
    L=0;
}

void AssetsManagerLua::getString(lua_State* L, const char *key, std::string &data)
{
    lua_getfield(L,-1,key);
    if(!lua_isnil(L,-1))
        data = lua_tostring(L, -1);
    else
        printf("Fail to get data for key: %s\n",key);
    lua_pop(L, 1);            
}

void AssetsManagerLua::getFloat(lua_State* L, const char *key, float &data)
{
    lua_getfield(L,-1,key);
    if(!lua_isnil(L,-1))
        data = lua_tonumber(L, -1);
    else
        printf("Fail to get data for key: %s\n",key);
    lua_pop(L, 1);            
    
}

void AssetsManagerLua::getInt(lua_State* L, const char *key, int &data)
{
    lua_getfield(L,-1,key);
    if(!lua_isnil(L,-1))
        data = (int)lua_tonumber(L, -1);
    else
        printf("Fail to get data for key: %s\n",key);
    lua_pop(L, 1);            
    
}
    
void AssetsManagerLua::getIntVector(lua_State* L, const char *table_name, std::vector<int> *vect)
{    
    lua_pushstring(L, table_name);    
    lua_gettable(L, -2);
    
    if(!lua_istable(L,-1))
    {
        printf("Fail to get table: %s\n", table_name);
        return;
    }
   
    lua_pushnil(L);

    while(lua_next(L, -2) != 0)
    {
        int val;                
        val = (int)lua_tonumber(L, -1);
        vect->push_back(val);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

void AssetsManagerLua::getPropVector(lua_State* L, const char *table_name, std::vector<EntityProp> *items)
{
    lua_pushstring(L, table_name);    
    lua_gettable(L, -2);
    
    if(!lua_istable(L,-1))
    {
        printf("Fail to get table: %s\n", table_name);
        return;
    }
   
    lua_pushnil(L);
    while(lua_next(L, -2) != 0)
    {
        EntityProp prop;                
        getInt(L,"id",prop.m_id);
        getInt(L,"color",prop.m_color);
        getInt(L,"hits",prop.m_hits);
        getInt(L,"value",prop.m_value);
        lua_pop(L, 1);  
        items->push_back(prop);
    }
    lua_pop(L, 1);
    
}

bool AssetsManagerLua::loadScript(const char *script)
{
    if(script==NULL)
    {
        printf("%s", "Invalid LUA resource file\n");
        return false;
    }
    L = luaL_newstate();
    if (luaL_loadstring(L, script) || lua_pcall(L, 0, 0, 0)) 
    {
        printf("%s","Fail to load file\n");
        L = 0;
        return false ;
    }
    return true;
}

bool AssetsManagerLua::close()
{
    if(L!=0)
    {
        lua_close (L);
        L=0;
    }
    return true;
}

bool AssetsManagerLua::loadMenuItems(const char *menu_name, std::vector<MenuItem> *items)
{
       
    lua_getglobal(L, menu_name);   
    if(!lua_istable(L,-1))
    {
        printf("Fail to get table: %s\n", menu_name);
        return false;
    }
    lua_pushnil(L);

    while(lua_next(L, -2) != 0)
    {
        MenuItem item;                
        getString(L,"name",item.m_name);
        getString(L,"label",item.m_label);
        getString(L,"model",item.m_model);
        getString(L,"cb",item.m_cb);
        getFloat(L,"x",item.m_x);
        getFloat(L,"y",item.m_y);
        getFloat(L,"width",item.m_width);        
        getFloat(L,"height",item.m_height);                
        //dumpMenuItem(item);
        lua_pop(L, 1);  
        items->push_back(item);
    }
    lua_pop(L, 1);
    return true;
}

void AssetsManagerLua::dumpMenuItem(MenuItem& item)
{
    printf("******** Item *********\n");
    printf("name: %s\n", item.m_name.c_str());
    printf("m_label: %s\n", item.m_label.c_str());
    printf("m_model: %s\n", item.m_model.c_str());
    printf("m_cb: %s\n", item.m_cb.c_str());
    printf("x: %f\n", item.m_x);
    printf("y: %f\n", item.m_y);
    printf("width: %f\n", item.m_width);
    printf("height: %f\n", item.m_height);
}

bool AssetsManagerLua::loadEntityProps(const char* table_name, EntityProps *props)
{
    lua_getglobal(L, table_name);   
    if(!lua_istable(L,-1))
    {
        printf("Fail to get table: %s\n", table_name);
        return false;
    } 
    getPropVector(L, "brick", &props->m_brick);
    getPropVector(L, "powerup", &props->m_powerup);      

    lua_pop(L, 1);
    return true;
}

bool AssetsManagerLua::loadResources(const char *resource_table, std::vector<ResourceItem> *resources)
{
    lua_getglobal(L, resource_table);   
    if(!lua_istable(L,-1))
    {
        printf("Fail to get table: %s\n", resource_table);
        return false;
    }
    lua_pushnil(L);

    while(lua_next(L, -2) != 0)
    {
        ResourceItem item;                
        getString(L,"rtype",item.m_type);
        getString(L,"rid",item.m_id);
        getString(L,"file1_path",item.m_path1);
        getString(L,"file2_path",item.m_path2);
        //printf("%s %s %s\n",item.m_type.c_str(),item.m_path1.c_str(),item.m_path2.c_str());
        //dumpMenuItem(item);
        lua_pop(L, 1);  
        resources->push_back(item);
    }
    lua_pop(L, 1);
    return true;
}

bool AssetsManagerLua::loadLevelData(const char *table_name, LevelData *data)
{
    lua_getglobal(L, table_name);   
    if(!lua_istable(L,-1))
    {
        printf("Fail to get table: %s\n", table_name);
        return false;
    }
    getString(L,"name", data->m_name);
    getInt(L,"width",data->m_width);
    getInt(L,"height",data->m_height);
    printf("name:%s\n", data->m_name.c_str());    
    getIntVector(L,"bricks", &data->m_bricks);
    getIntVector(L,"powerup", &data->m_powerup);
    return true;
}



