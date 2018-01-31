#ifndef ASSETS_MANAGER
#define ASSETS_MANAGER

#include <map>
#include <string>
#include <vector>
#include <osg/Program>
#include <osg/MatrixTransform>
#include <osg/Shader>
#include <osgDB/ReadFile>
#include <osgText/Font>
#include "Config.h"
#include "AssetsManagerLua.h"

class AssetsManager
{
    public:
        AssetsManager();
        static AssetsManager& instance();
        void setRootPath(const char*);
        std::string& getRootPath();
        osg::ref_ptr<osg::Object> loadAsset(AssetType, const char*);
        //char* readBinaryFile(const char*, unsigned int*);
        osg::Object* loadObject(const char*, const char*);
        void writeNode(osg::Node&,const char*);
        void addModelAsset(osg::MatrixTransform&);
        osg::MatrixTransform* getEntityModel(EntityType);
        osg::MatrixTransform* getEntityModel(const char*);
        osg::Vec3f getEntityModelPosition(const char* model_name);
        osg::MatrixTransform* getWidgetModel(const char*);
        osg::Program* getProgram(const char*);
        osg::Texture2D* getTexture(const char*);
        osgText::Font*  getFont(const char*);        
        bool getLevelData(const char *, const char*, LevelData *);
        void getEntityProps(const char *, EntityProps *);
        std::vector<MenuItem>*getMenuItems(const char*);
        Animation* getAnimation(const char*);

        bool loadAnimations();
        bool loadAssets();
        bool loadTextures();        
        bool loadMenuEntries();    
        
    protected:
       std::map<std::string, std::vector<MenuItem>* > m_menuEntries;
       std::map<EntityType, std::string> m_etypeToString;
       std::map<std::string,  osg::ref_ptr<osg::Object> > m_assetList;
       std::vector<Animation> m_animationList;
       std::string m_rootPath;
       
        
};

#endif
