
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Bone>
#include "Logging.h"
#include "AssetsManager.h"
#include <Utils.h>

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
extern bool writeFile(const char *filename, const char *buf, unsigned int filesize);

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

std::string& AssetsManager::getRootPath()
{
    return m_rootPath;
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
        if(item->m_type == "osgt")
        {
            osg::Object *obj = loadObject(item->m_path.c_str(),"osgt");
            osg::Node *node = dynamic_cast<osg::Node*>(obj);
            if(node != NULL)
            {                   
                FindModel fm;
                node->accept(fm);
                
            }
        }
        else if(item->m_type == "ttf")
        {
            osg::Object *obj = loadObject(item->m_path.c_str(),"ttf");
            osg::ref_ptr<osgText::Font> font = dynamic_cast<osgText::Font*>(obj);
            m_assetList.insert(std::make_pair(item->m_id, font.get()));
        }   

        else if(item->m_type=="glsl")
        {            
            LOG_INFO("AssetsManager::loadShaders=> %s\n", item->m_id.c_str(), item->m_path.c_str());
            osg::ref_ptr<CharBuffer> glsl = new CharBuffer;            
            
            bool res = glsl->createFromFile(item->m_path.c_str());
            if(res == false)
            {
                LOG_WARN("AssetsManager::loadShaders=> Shader sources not found %s\n",item->m_id.c_str());
                return false;
            }                          
            m_assetList.insert(std::make_pair(item->m_id, glsl));
        }
    }     
    return true;
}

void AssetsManager::writeNode(osg::Node& node, const char *file_path)
{
    std::string ext="osgt";
    osgDB::ReaderWriter *wr = osgDB::Registry::instance()->getReaderWriterForExtension(ext);    
    if(wr==NULL)
    {
        LOG_WARN("AssetsManager::writeObject=> Invalid plugin for extension %s\n",ext.c_str());
        return;
    }
    LOG_INFO("AssetsManager::writeObject=> name: %s\n", file_path);

    osg::ref_ptr<osgDB::Options> local_opt = new osgDB::Options;    
    local_opt->setPluginStringData( "fileType", "Ascii" );
    std::stringstream ss;
        
    wr->writeNode(node, ss, local_opt);        
    ss.seekg(0, std::ios::end);
    unsigned int buf_len = ss.tellg();    
    ss.seekg(0, std::ios::beg);    
    
    writeFile(file_path, ss.str().c_str(), buf_len);
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
    unsigned int filesize;
    AssetsManagerLua lua_mgr;
    std::string lua_file = m_rootPath + "/lua/menudef.lua";
    LOG_INFO("AssetsManager::loadMenuEntries()=> %s\n",lua_file.c_str());

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
osg::Vec3f AssetsManager::getEntityModelPosition(const char* model_name) 
{
    osg::MatrixTransform *model = NULL;
    if(m_assetList.count(model_name)>0)
        model = dynamic_cast<osg::MatrixTransform*>(m_assetList[model_name].get());    
    else
        LOG_WARN("AssetsManager::getEntityModelPosition=> Entity model not found: :%s\n", model_name);
    
    osg::Vec3f pos, scale;
    osg::Quat rot, so;
    osg::Matrix matrix = model->getMatrix();
    matrix.decompose(pos, rot, scale, so);     
        
    return pos;
}

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
    LOG_DEBUG("AssetsManager::getProgram: %s\n",shader_name);
    if(m_assetList.count(shader_name)>0)
    {
        osg::Program *program = dynamic_cast<osg::Program*>(m_assetList[shader_name].get());
        return program;
    }
    else
    {
        std::string vert_str = shader_name; vert_str += "_vert";
        std::string frag_str = shader_name; frag_str += "_frag";
        CharBuffer *vert_obj = dynamic_cast<CharBuffer*>(m_assetList[vert_str].get());
        CharBuffer *frag_obj = dynamic_cast<CharBuffer*>(m_assetList[frag_str].get());
        if((vert_obj==NULL)||(frag_obj==NULL))
        {
            LOG_ERROR("Fail to create shader: %s\n", shader_name);
            return NULL;
        }
        osg::ref_ptr<osg::Program> program = new osg::Program();        
        osg::Shader *shader=NULL;
        shader = new osg::Shader(osg::Shader::VERTEX, vert_obj->getData());
        shader->setName(vert_str);
        program->addShader(shader);
            
        shader = new osg::Shader(osg::Shader::FRAGMENT, frag_obj->getData());
        program->addShader(shader);
        shader->setName(frag_str);
            
        program->setName(shader_name);
        m_assetList.insert(std::make_pair(shader_name, program));
        return program.get();
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

void AssetsManager::getEntityProps(const char *file_name, EntityProps *data)
{
    LOG_INFO("Getting entity props from: %s\n",file_name);
    AssetsManagerLua lua_mgr;
    unsigned int buf_size;
    std::string file_path = m_rootPath + file_name;
    const char *script = readBinaryFile(file_path.c_str(), &buf_size);
    if(script==NULL)
    {
        LOG_WARN("Invalid entity props file:%s\n",file_path.c_str());
        return;
    }
    bool res = lua_mgr.loadScript(script);
    if(res)
    {
        lua_mgr.loadEntityProps("EntityProps", data);
        lua_mgr.close();
    }
}

bool AssetsManager::getLevelData(const char *ep_file, const char *lvl_name, LevelData *data)
{   
    AssetsManagerLua lua_mgr;      
    std::string level_path = m_rootPath + ep_file;
    osg::ref_ptr<CharBuffer> char_buf;
    if(m_assetList.count(ep_file)==0)
    {
        char_buf = new CharBuffer();
        bool rf = char_buf->createFromFile(level_path.c_str());
        if(rf==true)
            m_assetList.insert(std::make_pair(ep_file, char_buf));
    } 
    else
    {
        LOG_INFO("Found level key for: %s\n", ep_file);
    }
    char_buf = dynamic_cast<CharBuffer*>(m_assetList[ep_file].get());
    if(char_buf==NULL) 
    {
        LOG_ERROR("Invalid cast for CharBuffer, key: %s\n", ep_file);
        return false;
    }
    const char *script = char_buf->getData();
    bool res = lua_mgr.loadScript(script);
    if (res==false)
    {
        LOG_ERROR("Level file not loaded: %s\n",level_path.c_str());
        return false;       
    }
        
    LOG_INFO("Loading level name:%s from:%s \n", lvl_name, ep_file);
    res = lua_mgr.loadLevelData(lvl_name, data);
    lua_mgr.close();
    
    if(res==false)
        LOG_ERROR("Level name:%s from:%s not found \n", lvl_name, ep_file);    
    else
        LOG_INFO("Loading level name:%s from:%s \n", lvl_name, ep_file);
            
    return res;
}

std::vector<MenuItem>* AssetsManager::getMenuItems(const char* menu_name)
{
    if(m_menuEntries.count(menu_name) > 0)
        return m_menuEntries[menu_name];
    else
        LOG_ERROR("AssetsManager::getMenuEntries=> Menu items not found form menu: %s\n", menu_name);
    return NULL;
}


