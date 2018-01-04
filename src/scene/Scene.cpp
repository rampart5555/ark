#include <osg/Program>
#include "AssetsManager.h"
#include "Scene.h"
#include "Entity.h"
#include "EntityBall.h"
#include "Logging.h"

Scene::Scene()
{
    m_sceneNode = new osg::MatrixTransform();
    m_entityProps = new EntityProps;
    AssetsManager::instance().getEntityProps("lua/entity_props.lua", m_entityProps);  
    m_sceneNode->addChild(m_entityMgr.getNodeEntMgr());
    m_entityMgr.loadShaders();
}

Scene::~Scene()
{        
    delete m_entityProps;
}

void Scene::clear()
{    
    m_levelScore = 0;
    m_entityMgr.clear();
}

void Scene::loadScene(const char *ep_file, const char* lvl_name)
{    
    clear();

    loadStaticScene();

    EntityType ent_env_arr[] = {
        ENTITY_WALL_TOP,
        ENTITY_WALL_LEFT,
        ENTITY_WALL_RIGHT,
        ENTITY_WALL_BOTTOM,
        ENTITY_BALL,
        ENTITY_PADDLE,
        ENTITY_BACKGROUND,
        ENTITY_NONE //last entry        
    };    
            
    osg::ref_ptr <Entity> ent;
    for(unsigned int i=0; ; i++ )
    {
        if (ent_env_arr[i] == ENTITY_NONE) 
            break;
        ent = m_entityMgr.createEntity(ent_env_arr[i]);
        m_entityMgr.addEntity(ent);
        if(ent->getType() == ENTITY_BALL)
        {
            EntityBall *eball = ent->asEntityBall();
            eball->setSpeed(2.0);
            eball->setDir(osg::Vec2(0.5, 0.5));
        }
    }
    
               
    //loadTMXMap(tmx_file);
    loadLevel(ep_file, lvl_name);
    loadShaders();
    
}

bool Scene::loadLevel(const char *ep_file, const char* lvl_name)
{
    LevelData ldata;

    bool res = AssetsManager::instance().getLevelData(ep_file, lvl_name, &ldata);    
    if(res==false)
    {
        LOG_ERROR("Scene::loadLevel=>level not loaded:%s\n", lvl_name);
        return false;
    }
            
    int  cols = ldata.m_width;
    int  rows = ldata.m_height;        
    /* 
       tiled rendering order should be left down (map propertiess in tiled editor
       osg dimensions from tmx 
       32px == 0.1 in 3D coordinates
       tile size width:64px height:32px
    */
    float osg_ox = 0.2*(cols-1)/2.0;
    float osg_oy = 0.1*(rows-1)/2.0;
    int tindex, brick_val, powerup_val;
    for(int y = 0; y< rows; y++)
    {
        for(int x=0; x<cols;x++)
        {            
            tindex = y*cols + x;
            brick_val = ldata.m_brick[tindex];
            powerup_val = ldata.m_powerup[tindex];
            if(brick_val > 0)
            {
                //printf("%2d ",ldata.m_bricks[tindex]);
                osg::ref_ptr<Entity> ent = m_entityMgr.createEntity(ENTITY_BRICK);
                if(ent==NULL)
                    return false;
                    
                ent->setPosition(osg::Vec3(-osg_ox + x*0.2, osg_oy - y*0.1, 0.0));
                ent->setPowerup((PowerupType)powerup_val);
                ent->setSubType(brick_val);
                addEntityProps(ent);

                m_entityMgr.addEntity(ent);
            }
            //printf("%2d ",ldata.m_bricks[tindex]);
        }
        //printf("\n");
    }
    
    return true;
}

void Scene::addEntityProps(Entity *ent)
{
    unsigned int i;
    EntityProp *prop;
    if(ent->getType()==ENTITY_BRICK)
    {
        for( i=0;i<m_entityProps->m_brick.size();i++)
        {
            prop=&m_entityProps->m_brick[i];
            if( prop->m_id == ent->getSubType() )
            {                
                ent->setHits(prop->m_hits);
                ent->setValue(prop->m_value);
                ent->setColor(prop->m_color);
                //printf("setting entity prop: %d color:%8x\n",prop->m_id, prop->m_color);
                break;                
            }
        }
    }
    else if(ent->getType()==ENTITY_POWERUP)
    {
        for( i=0;i<m_entityProps->m_powerup.size();i++)
        {
            prop=&m_entityProps->m_powerup[i];
            if(prop->m_id == ent->getSubType())
            {
                ent->setColor(prop->m_color);
                break;
            }
        }
    }
}

osg::ref_ptr <Entity> Scene::createEntity(EntityType etype)
{
    osg::MatrixTransform *model;
    osg::ref_ptr <Entity> ent;
    model = AssetsManager::instance().getEntityModel(etype);
    if(model == NULL)
    {
        LOG_ERROR("Entity model not found for entity type:%d", etype);
        return NULL;
    }
    if(etype==ENTITY_BALL)
        ent = new EntityBall();
    else
        ent = new Entity();
    ent->setType(etype);
    ent->setModel(*model);
    ent->setName(model->getName().c_str());    
    return ent;
}

osg::MatrixTransform* Scene::getSceneNode()
{
    return m_sceneNode;
}

void Scene::loadShaders()
{
    //AssetsManager::loadShaders();
    osg::Program *program = AssetsManager::instance().getProgram("default_color");
    osg::StateSet *ss = m_sceneNode->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON );
#if 0    
    osg::Texture *texture=AssetsManager::instance().getTexture("texture_wall");
    ss->setTextureAttribute(0, texture);
    osg::Uniform* baseTextureSampler = new osg::Uniform("wall_sampler",0);
    ss->addUniform(baseTextureSampler);     
#endif    
    
}

/* for static scene physics is not enabled */
void Scene::loadStaticScene()
{
    osg::MatrixTransform *model = AssetsManager::instance().getEntityModel("entity_paddle_support");
    osg::Vec3f pos, scale;
    osg::Quat rot, so;
    osg::Matrix m = model->getMatrix();
    m.decompose(pos, rot, scale, so);       
    m_sceneNode->addChild(model);
    osg::MatrixTransform *pm = AssetsManager::instance().getEntityModel("entity_paddle");
    float px=0;
    for(int i=0;i<3;i++)
    {        
        osg::MatrixTransform *mt = new osg::MatrixTransform(*pm);        
        osg::Matrix new_tr; 
        new_tr.makeTranslate(pos.x()-1.0+px,pos.y(),pos.z());
        mt->setMatrix(new_tr);
        m_sceneNode->addChild(mt);
        px+=0.5;
    }
}
