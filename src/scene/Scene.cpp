#include <osg/Program>
#include "AssetsManager.h"
#include "Scene.h"
#include "Entity.h"
#include "EntityBall.h"
#include "Logging.h"

Scene::Scene()
{    
    m_sceneId=SCENE;
    m_sceneNode = new osg::MatrixTransform();
    m_entityProps = new EntityProps;
    AssetsManager::instance().getEntityProps("lua/entity_props.lua", m_entityProps);  
    m_sceneNode->addChild(m_entityMgr.getNodeEntMgr());
    m_entityMgr.loadShaders();    
    m_sceneLoaded = false;
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
        ENTITY_DOOR_LEFT,
        ENTITY_DOOR_RIGHT,
        ENTITY_DOOR_LEFT_SENSOR,
        ENTITY_DOOR_RIGHT_SENSOR,
        ENTITY_NONE //last entry        
    };    
            
    osg::ref_ptr <Entity> ent;
    EntityType etype;
    for(unsigned int i=0; ; i++ )
    {
        etype = ent_env_arr[i];
        if (etype == ENTITY_NONE) 
            break;
        if(m_entityList.count(etype)==0)
        {
            ent = m_entityMgr.createEntity(etype);
            m_entityList.insert(std::make_pair(etype, ent));    
        }
        else
            ent = m_entityList[etype];
        if(ent==NULL)
        {
            LOG_ERROR("Entity not found type:%d\n", ent_env_arr[i]);
            continue;
        }    
        m_entityMgr.addEntity(ent);
        if(ent->getType() == ENTITY_BALL)
        {
            EntityBall *eball = ent->asEntityBall();
            eball->setSpeed(0.0);
            eball->setDir(osg::Vec2(0.5, 0.5));
        }
        
    }
                   
    //loadTMXMap(tmx_file);
    loadLevel(ep_file, lvl_name);
    loadShaders();
    //playAnimation("animation_level_new");
    //playAnimation("animation_level_continue");
    //playAnimation("animation_level_cleared");
    //m_entityMgr.startPhysics();
    
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
    ent = new Entity();
    ent->setType(etype);
    ent->setModel(*model);
    ent->setName(model->getName().c_str());    
    return ent;
}

bool Scene::addEntity(osg::ref_ptr<Entity> entity)
{
    if(entity->getType()==ENTITY_PADDLE)
    {
        for(unsigned i=0;i<m_paddleSlots.size();i++)
        {
            if(m_paddleSlots[i].m_entity.valid()==false)
            {
                m_paddleSlots[i].m_entity = entity;
                m_paddleSlots[i].m_entity->setPosition(m_paddleSlots[i].m_pos);
                m_sceneNode->addChild(entity->getEntityNode());
                break;
            }
        }
    }
    return true;
}

bool Scene::removeEntity(unsigned int slot_id)
{
    EntitySlot *es = &m_paddleSlots[slot_id];
    m_sceneNode->removeChild(es->m_entity->getEntityNode());
    es->m_entity = NULL;
    return true;
}

const EntitySlot* Scene::getEntitySlot(EntityPos epos, bool valid)
{    
    unsigned int i;
    if(epos == FIRST)
    {
        for(i = 0; i<m_paddleSlots.size(); i++)
        {
            if(m_paddleSlots[i].m_entity.valid()==valid)
                return &m_paddleSlots[i];
        }
    }
    if(epos == LAST)
    {
        for(i = m_paddleSlots.size(); i>0; i--)
        {
            int j = i-1;
            if(m_paddleSlots[j].m_entity.valid()==valid)
                return &m_paddleSlots[j];
        }
    }    
    return NULL;    
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
    if(m_sceneLoaded == true)
        return;
    m_sceneLoaded = true;

    osg::MatrixTransform *model = AssetsManager::instance().getEntityModel("entity_paddle_support");
    osg::Vec3f pos, scale;
    osg::Quat rot, so;
    osg::Matrix m = model->getMatrix();    
    m.decompose(pos, rot, scale, so);      
    m_sceneNode->addChild(model);
        
    /* init paddle support slots */
    for(int i=0;i<3; i++)
    {
        EntitySlot es;
        float x = pos.x() -1 + 0.5*i;
        es.m_pos.set(x, pos.y(), pos.z());
        m_paddleSlots.push_back(es);        
    }
    /* add paddle to paddle slots*/
    for(int i=0;i<3;i++)
    {
        osg::ref_ptr<Entity> paddle = m_entityMgr.createEntity(ENTITY_PADDLE);                
        addEntity(paddle);
    }

#if 0    
    osg::ref_ptr<Entity> paddle = m_entityMgr.createEntity(ENTITY_PADDLE);
    paddle->setPosition(pos);
    m_sceneNode->addChild(paddle->getEntityNode());
    m_sceneEntityList.push_back(paddle);
#endif

}
void Scene::levelContinue()
{   
#if 0    
    const EntitySlot *es;
    es = getEntitySlot(LAST, true);
    if(es != NULL)
    {
        Entity *paddle = es->m_entity->asEntityPaddle();        
        osg::Vec3 end_pos  = AssetsManager::instance().getEntityModelPosition("spawn_entity_paddle");        
        playAnimation(paddle, end_pos, Scene_level_continue, PADDLE_MOVE_FROM_SLOT);
        
    }
    else
    {
        LOG_INFO("No more paddle in paddle slots: %s\n","");
    }
#endif 
}
#if 0 
void Scene::playAnimation(Entity *ent, osg::Vec3 end_pos, EngineCallback cb, AnimType atype)
{
   
    osg::Vec3 start_pos = ent->getPosition();
    EntityAnimation *ea = new EntityAnimation;    
    ea->setCallback(cb);
    ea->setAnimationType(atype);
    ea->createAnimation(start_pos, end_pos);
    ea->setEntity(ent);
    ent->setAnimation(ea);
    ent->playAnimation();
}
#endif

void Scene::update(float passedTime)
{
    m_entityMgr.update(passedTime);
    /* remove invalid entity from paddle slots */
    for(unsigned i = 0; i<m_paddleSlots.size(); i++)
    {
        if(m_paddleSlots[i].m_entity.valid() && m_paddleSlots[i].m_entity->isValid()==false)
            removeEntity(i);
    }
}

void Scene::playAnimation(std::string anim_name)
{    
    if(anim_name=="animation_level_new")
    {   
        animLevelNew(anim_name);
    }  
    else if(anim_name=="animation_level_continue")  
    {
        animLevelContinue(anim_name);
    }
    else if(anim_name=="animation_level_cleared")
    {
        animLevelCleared(anim_name);
    }
}

void Scene::endAnimation(std::string anim_name)
{
    LOG_INFO("Scene::endAnimation: %s\n",anim_name.c_str());
    if(anim_name=="animation_level_new")
    {
        m_entityMgr.startPhysics();
    }
}

void Scene::animLevelNew(std::string anim_name)
{
    float x,y,z;                
    osg::ref_ptr <Entity> ent;
    //left door
    {            
        EntityAnimation *ea = new EntityAnimation;  
        ent=m_entityList[ENTITY_DOOR_LEFT];
        if(ent==NULL)       
        {
            LOG_ERROR("Scene::playAnimation: Invalid entity %s\n", "");
            return;
        }
        osg::Vec3 pos = ent->getPosition();
        x=pos.x(); y=pos.y(); z=pos.z();        
        ea->addTranslate(0.0, x, y, z); 
        ea->addTranslate(2.0, x, y+0.25, z); 
        ea->addTranslate(3.0, x, y+0.25, z);
        ea->addTranslate(4.0, x, y, z); 
        ent->getEntityNode()->setUpdateCallback(ea);
    }
    // paddle
    {
        EntityAnimation *ea = new EntityAnimation;        
        ent=m_entityList[ENTITY_PADDLE];
        if(ent==NULL)       
        {
            LOG_ERROR("Scene::playAnimation: Invalid entity %s\n", "");
            return;
        }
        osg::Vec3 pos = ent->getPosition();            
        x = pos.x()-1.5; y=pos.y(); z=pos.z();        
        ea->addTranslate(0.0, x, y, z);
        ea->addTranslate(2.0, x, y, z);
        ea->addTranslate(3.0, 0.0, y, z);
        ent->getEntityNode()->setUpdateCallback(ea);
    }
    //ball
    {
        EntityAnimation *ea = new EntityAnimation;        
        ent=m_entityList[ENTITY_BALL];
        if(ent==NULL)       
        {
            LOG_ERROR("Scene::playAnimation: Invalid entity %s\n", "");
            return;
        }
        osg::Vec3 pos = ent->getPosition();            
        x = pos.x(); y=pos.y(); z=pos.z()-0.5;        
        ea->addTranslate(0.0, x, y, z);
        ea->addTranslate(3.0, x, y, z);
        ea->addTranslate(4.0, x, y, z + 0.5);
        ea->setEventName(anim_name);
        ent->getEntityNode()->setUpdateCallback(ea);
       
    }                
}

void Scene::animLevelContinue(std::string anim_name)
{
    float x,y,z;                
    osg::ref_ptr<Entity> ent;        
    //paddle slot
    {
        osg::Vec3 end_pos  = AssetsManager::instance().getEntityModelPosition("spawn_entity_paddle");
        const EntitySlot* es = getEntitySlot(LAST, true);        
        ent = es->m_entity;
        if(ent.valid()==false)
        {
            LOG_ERROR("Scene::animLevelContinue: Invalid entity %s\n", "");
            return;
        }
        EntityAnimation *ea = new EntityAnimation;
        osg::Vec3 pos = ent->getPosition();            
        x = pos.x(); y=pos.y(); z=pos.z();        
        ea->addTranslate(0.0, x, y, z );
        ea->addTranslate(1.0, x, y, z + 1.0);
        ea->addTranslate(2.0, end_pos.x(), end_pos.y(), z+1.0);
        ea->addTranslate(3.0, end_pos.x(), end_pos.y(), end_pos.z());
        ent->getEntityNode()->setUpdateCallback(ea);
    }
    //ball    
    {
        EntityAnimation *ea = new EntityAnimation;        
        ent=m_entityList[ENTITY_BALL];
        if(ent==NULL)       
        {
            LOG_ERROR("Scene::animLevelContinue: Invalid entity %s\n", "");
            return;
        }
        osg::Vec3 pos = ent->getPosition();            
        x = pos.x(); y=pos.y(); z=pos.z()-0.5;        
        ea->addTranslate(0.0, x, y, z);
        ea->addTranslate(3.0, x, y, z);
        ea->addTranslate(4.0, x, y, z + 0.5);
        ea->setEventName(anim_name);
        ent->getEntityNode()->setUpdateCallback(ea);        
    }                
}

void Scene::animLevelCleared(std::string anim_name)
{
    float x,y,z;                
    osg::ref_ptr <Entity> ent;
    //right door
    {            
        EntityAnimation *ea = new EntityAnimation;  
        ent = m_entityList[ENTITY_DOOR_RIGHT];        
        if(ent==NULL)       
        {
            LOG_ERROR("Scene::playAnimation: Invalid entity %s\n", "");
            return;
        }
        ent->disablePhysics();
        osg::Vec3 pos = ent->getPosition();
        x=pos.x(); y=pos.y(); z=pos.z();        
        ea->addTranslate(0.0, x, y, z); 
        ea->addTranslate(2.0, x, y+0.25, z);
        ea->setEventName(anim_name);         
        ent->getEntityNode()->setUpdateCallback(ea);        
    }
}