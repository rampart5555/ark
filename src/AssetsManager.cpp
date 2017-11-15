
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Bone>
#include "Logging.h"
#include "AssetsManager.h"


typedef struct { 
    const char* name;
    const char* vert;
    const char* frag;
}ShaderEntry;

typedef struct { 
    const char* name;
    const char* filename;
}ImageEntry;

typedef struct { 
    const char* name;
    const char* filename;
}FontEntry;

class FindAnimation : public osg::NodeVisitor
{
    public:
        osg::ref_ptr<osgAnimation::BasicAnimationManager> _am;    
        FindAnimation(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
                        
        }     
        void apply(osg::Node& node) 
        {
            if (_am.valid())
            {                
                return;
            }
            if (node.getUpdateCallback()) 
            {
                osgAnimation::AnimationManagerBase* b = dynamic_cast<osgAnimation::AnimationManagerBase*>(node.getUpdateCallback());
                if (b) 
                {
                    _am = new osgAnimation::BasicAnimationManager(*b);
                    printf("****************************found animation\n");
                    for (osgAnimation::AnimationList::const_iterator animIter = _am->getAnimationList().begin();
                    animIter != _am->getAnimationList().end(); ++animIter)
                    {
                        (*animIter)->setPlayMode(osgAnimation::Animation::LOOP);
                    }                    
                    return;
                }
            }
            traverse(node);
        }                       
};   

class FindModel : public osg::NodeVisitor
{
    public:            
        FindModel(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
                        
        } 
        void apply(osg::MatrixTransform& tr)
        {              
            AssetsManager::instance().addModelAsset(tr);            
            //traverse(tr);
        }
};

extern char* readBinaryFile(const char* filename, unsigned int *filesize);

AssetsManager::AssetsManager()
{    
    m_rootPath = "/home/dancioata/g_dev/ark/assets/";    
    m_etypeToString.insert(std::make_pair(ENTITY_BRICK, "entity_brick"));
    m_etypeToString.insert(std::make_pair(ENTITY_BALL, "entity_ball"));
    m_etypeToString.insert(std::make_pair(ENTITY_PADDLE, "entity_paddle"));
    m_etypeToString.insert(std::make_pair(ENTITY_POWERUP, "entity_powerup"));
    m_etypeToString.insert(std::make_pair(ENTITY_WALL_BOTTOM, "entity_wall_bottom"));
    m_etypeToString.insert(std::make_pair(ENTITY_WALL_TOP, "entity_wall_top"));
    m_etypeToString.insert(std::make_pair(ENTITY_WALL_LEFT, "entity_wall_left"));
    m_etypeToString.insert(std::make_pair(ENTITY_WALL_RIGHT, "entity_wall_right"));
    m_etypeToString.insert(std::make_pair(ENTITY_BACKGROUND, "entity_background"));
    m_etypeToString.insert(std::make_pair(ENTITY_TURRET, "entity_turret"));
    m_etypeToString.insert(std::make_pair(ENTITY_BULLET, "entity_bullet"));
    
}

AssetsManager& AssetsManager::instance()
{
    static AssetsManager instance;
    return instance; 
}

void AssetsManager::setRootPath(const char *path)
{
    m_rootPath = path;
}

bool AssetsManager::loadAssets()
{
    AssetsManagerLua assets_lua;
    std::vector<ResourceItem> res_items;
    ResourceItem *item;
    unsigned int file_size = 0;
    //Resources
    std::string resources_lua = m_rootPath + "/lua/resources.lua";
    LOG_INFO("Loading lua resource file: %s\n",resources_lua.c_str());    
    const char *script = readBinaryFile(resources_lua.c_str(), &file_size);
    bool res = assets_lua.loadScript(script);    
    if(res==false)
    {
        LOG_ERROR("%s","Failed to read file\n");
        return false;
    }
    assets_lua.loadResources("Resources",&res_items);
    assets_lua.close();
    //Menu entries
    loadMenuEntries();
    
    for (unsigned i=0;i<res_items.size();i++)
    {
        item=&res_items[i];
        printf("%s\n", item->m_path1.c_str());
        if(item->m_type == "osgt")
        {
            osg::Object *obj = loadObject(item->m_path1.c_str(),"osgt");
            osg::Node *node = dynamic_cast<osg::Node*>(obj);
            if(node != NULL)
            {                
                FindModel fm;
                node->accept(fm);                  
            }
        }
        else if(item->m_type == "ttf")
        {
            osg::Object *obj = loadObject(item->m_path1.c_str(),"ttf");
            osg::ref_ptr<osgText::Font> font = dynamic_cast<osgText::Font*>(obj);
            m_assetList.insert(std::make_pair(item->m_id, font.get()));
        }
        
        else if(item->m_type=="glsl")
        {
            unsigned int buf_size;
            LOG_INFO("AssetsManager::loadShaders=> %s %s %s\n",
                item->m_id.c_str(), item->m_path1.c_str(), item->m_path2.c_str());
            char *vert_shader = (char*)readBinaryFile(item->m_path1.c_str(), &buf_size);                
            char *frag_shader = (char*)readBinaryFile(item->m_path2.c_str(), &buf_size);          
                
            if((vert_shader==NULL) || (frag_shader==NULL))
            {
                LOG_WARN("AssetsManager::loadShaders=> Shader sources not found %s\n",item->m_id.c_str());
                return false;
            }  
            //vert_shader[buf_size-1] = 0; 
            //frag_shader[buf_size-1] = 0;
            osg::ref_ptr<osg::Program> program = new osg::Program();
            osg::Shader *shader=NULL;
            shader = new osg::Shader(osg::Shader::VERTEX, vert_shader);
            shader->setName(item->m_path1);
            program->addShader(shader);
            
            shader = new osg::Shader(osg::Shader::FRAGMENT, frag_shader);
            program->addShader(shader);
            shader->setName(item->m_path2);
            
            program->setName(item->m_id);
            m_assetList.insert(std::make_pair(item->m_id, program));
        }
    }     
    return true;
}

osg::Object* AssetsManager::loadObject(const char *filename, const char *ext)
{
    char *buf = NULL;
    unsigned int buf_size = 0;
           
    LOG_INFO("AssetsManager::loadObject=> name: %s, type:%s\n", filename, ext);
    buf = readBinaryFile(filename, &buf_size);
    
    if(buf==NULL)
        return NULL;
        
    std::stringstream ss(std::string(reinterpret_cast<char const*>(buf), buf_size)); 
    
    osgDB::ReaderWriter *wr = osgDB::Registry::instance()->getReaderWriterForExtension(std::string(ext));    
    if(wr==NULL)
    {
        LOG_WARN("AssetsManager::loadObject=> Invalid plugin for extension %s\n",ext);
        return NULL;
    }
    osgDB::ReaderWriter::ReadResult rr = wr->readObject(ss);        
    
    if (rr.validObject())
    {         
    }
    if (rr.error()) 
    {
        LOG_WARN("%s\n", rr.message().c_str());
        return NULL;
    }
    if (rr.notEnoughMemory()) 
    {
        LOG_WARN("%s","AssetsManager::loadObject=> Not enought memory to load file\n");
        return NULL;
        
    }  
    return rr.takeObject();  
}    

void AssetsManager::addModelAsset(osg::MatrixTransform &model)
{
    LOG_INFO("AssetsManager::addModelAsset=> %s\n", model.getName().c_str());
    osg::ref_ptr <osg::Object> new_tr = new osg::MatrixTransform(model);
    m_assetList.insert(std::make_pair(model.getName(), new_tr));  
}

bool AssetsManager::loadTextures()
{
    ImageEntry imageList[]={
        {"entity_brick_texture", "images/brick.jpg"},
        {NULL, NULL}
    };
    for(int i=0; ; i++)
    {
        if(imageList[i].name == NULL)
            break;
        osg::Object *obj = loadObject(imageList[i].filename,"jpeg");
        osg::ref_ptr<osg::Image> image = dynamic_cast<osg::Image*>(obj);
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
        texture->setDataVariance(osg::Object::DYNAMIC);
        texture->setImage(image.get());
        texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
        texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
        m_assetList.insert(std::make_pair(imageList[i].name, texture.get()));    
    }
    return true;
}

bool AssetsManager::loadMenuEntries()
{
    LOG_INFO("%s", "AssetsManager::loadMenuEntries()");
    
    unsigned int filesize;
    AssetsManagerLua lua_mgr;
    std::string lua_file = m_rootPath + "/lua/menudef.lua";
    
    char *script = readBinaryFile(lua_file.c_str(), &filesize);
    
    bool result = lua_mgr.loadScript(script);
    if(result==false )
    {
        LOG_ERROR("Error Loading script:%s\n", lua_file);
        return false;
    }
    const char menu_names_size=3;  
    const char *menu_names[menu_names_size]={"MenuLevelComplete","MenuSceneHud","MenuLevelFailed"};
    for(int i=0;i<menu_names_size;i++)
    {
        std::vector<MenuItem> *vect = new std::vector<MenuItem>(); 
        if(lua_mgr.loadMenuItems(menu_names[i], vect) == true)
            m_menuEntries.insert(std::make_pair(menu_names[i], vect));
        else
            LOG_ERROR("Error Loading items for menu:%s\n", menu_names[i]);
            
    }
    lua_mgr.close();
    
    return true;
}         
/// get ///
osg::MatrixTransform* AssetsManager::getEntityModel(EntityType etype) 
{
    osg::MatrixTransform *model = NULL;
    if( m_etypeToString.count(etype)  >0 ) 
        model = dynamic_cast<osg::MatrixTransform*>(m_assetList[m_etypeToString[etype]].get());    
    else
        LOG_WARN("AssetsManager::getEntityModel=> Entity model not found: :%d\n", etype);
            
    return model;
}

osg::MatrixTransform* AssetsManager::getEntityModel(const char* model_name) 
{
    osg::MatrixTransform *model = NULL;
    if(m_assetList.count(model_name)>0)
        model = dynamic_cast<osg::MatrixTransform*>(m_assetList[model_name].get());    
    else
        LOG_WARN("AssetsManager::getEntityModel=> Entity model not found: :%s\n", model_name);
    return model;
}

osg::MatrixTransform* AssetsManager::getWidgetModel(const char* widget_model_name)
{
    if(m_assetList.count(widget_model_name)>0)
    {                
        osg::MatrixTransform *model = dynamic_cast<osg::MatrixTransform*>(m_assetList[widget_model_name].get());
        return model;
    }
    else
    {
        LOG_WARN("AssetsManager::getWidgetModel=> Widget model not found: :%s\n",widget_model_name);
    }
    return NULL;
}

osg::Program* AssetsManager::getProgram(const char* shader_name)
{
    if(m_assetList.count(shader_name)>0)
    {
        osg::Program *program = dynamic_cast<osg::Program*>(m_assetList[shader_name].get());
        return program;
    }
    else
    {
        LOG_WARN("AssetsManager::getProgram=> not tound %s\n", shader_name);
    }
    return NULL;
}

osg::Texture2D* AssetsManager::getTexture(const char* texture_name)
{
    if(m_assetList.count(texture_name)>0)
    {        
        osg::Texture2D *texture = dynamic_cast<osg::Texture2D*>(m_assetList[texture_name].get());
        return texture;
    }
    else
    {
        LOG_WARN("AssetsManager::getTexture=> not tound%s\n", texture_name);
    }
    return NULL;
}

osgText::Font*  AssetsManager::getFont(const char *font_name)
{
    
    if(m_assetList.count(font_name)>0)
    {
        osgText::Font *font = dynamic_cast<osgText::Font*>(m_assetList[font_name].get());
        return font;
    }
    else
    {
        LOG_WARN("AssetsManager::getFont=> not tound%s\n", font_name);
    }
    return NULL;
}

char* AssetsManager::getLevelData(const char *tmx_level)
{
    char *buf=NULL;
    unsigned int bufsize;
    buf = readBinaryFile(tmx_level, &bufsize);
    if(buf==NULL)
    {
        LOG_ERROR("AssetsManager::getLevel: Level file not found %s", tmx_level);
    }
    return buf;
}

void AssetsManager::getEntityProps(const char *file_name, EntityProps *data)
{
    AssetsManagerLua lua_mgr;
    unsigned int buf_size;
    const char *script = readBinaryFile(file_name, &buf_size);
    bool res = lua_mgr.loadScript(script);
    if(res)
    {
        lua_mgr.loadEntityProps("EntityProps", data);
        lua_mgr.close();
    }
}

bool AssetsManager::getLevelData(const char *level_file, LevelData *data)
{
    AssetsManagerLua lua_mgr;
    unsigned int buf_size;
    std::string level_path = m_rootPath + level_file;
    const char *script = readBinaryFile(level_path.c_str(),&buf_size);
    bool res = lua_mgr.loadScript(script);
    if (res==false)
    {
        LOG_ERROR("Level file not loaded: %s\n",level_path.c_str());
        return false;       
    }
    lua_mgr.loadLevelData("Level", data);
        lua_mgr.close();
        
    return true;
}

std::vector<MenuItem>* AssetsManager::getMenuItems(const char* menu_name)
{
    if(m_menuEntries.count(menu_name) > 0)
        return m_menuEntries[menu_name];
    else
        LOG_ERROR("AssetsManager::getMenuEntries=> Menu items not found form menu: %s\n", menu_name);
    return NULL;
}


