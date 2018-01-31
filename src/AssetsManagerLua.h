#ifndef ASSETS_MANAGER_LUA
#define ASSETS_MANAGER_LUA

#include <string>
#include <vector>

extern "C" 
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "Config.h"
#include "ui/UIDefs.h"
#include "scene/SceneDefs.h"

class AssetsManagerLua
{
    public:
        AssetsManagerLua();
        static AssetsManagerLua& instance();
        bool loadScript(const char*);
        bool loadMenuItems(const char*, std::vector<MenuItem>* ); 
        bool loadEntityProps(const char*, EntityProps * );
        bool loadLevelData(const char*, LevelData*);
        bool loadResources(const char*, std::vector<ResourceItem>*);
        bool loadAnimations(std::vector<Animation>*);
        bool close();
        void dumpMenuItem(MenuItem&);
    private:
        lua_State* L;
        void getString(lua_State* , const char*, std::string&);
        void getFloat(lua_State* , const char*, float&);
        void getInt(lua_State* , const char*, int&);
        void getIntVector(lua_State*, const char*, std::vector<int>*);
        void getFloatVector(lua_State*, const char*, std::vector<float>*);
        void getPropVector(lua_State*, const char*, std::vector<EntityProp>*);
};

#endif
