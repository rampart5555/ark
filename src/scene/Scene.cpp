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
    
                       
    loadLevel(ep_file, lvl_name);
    loadShaders();        
}

bool Scene::loadLevel(const char *ep_file, const char* lvl_name)
{
    //add create entity in Scene to entity manager for physiscs
    std::list < osg::ref_ptr<Entity> >::iterator it;
    for ( it=m_entityList.begin(); it!=m_entityList.end(); ++it)
        if((*it)->getPhysics()==true)
            m_entityMgr.addEntity(*it);

    //load level bricks and power up
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

osg::ref_ptr <Entity> Scene::createEntity(std::string ename)
{
    
    osg::ref_ptr <Entity> ent;
    osg::MatrixTransform *model = AssetsManager::instance().getEntityModel(ename.c_str());
    if(model == NULL)
    {
        LOG_ERROR("Entity model not found for entity type:%s", ename.c_str());
        return NULL;
    }
    ent = new Entity();
    ent->setType(ENTITY_NONE);
    ent->setModel(*model);
    ent->setName(model->getName().c_str());  
    ent->setSceneParent(this);
    return ent;
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
    switch(etype)
    {
        case ENTITY_BALL:
            ent = new EntityBall();
            break;
        case ENTITY_PADDLE:        
            ent = new EntityPaddle();
            break;
        default:
            ent = new Entity();
            break;
    }    
    ent->setType(etype);
    ent->setModel(*model);
    ent->setName(model->getName().c_str());  
    ent->setSceneParent(this);
  
    return ent;
}

bool Scene::addEntity(osg::ref_ptr<Entity> entity)
{
    if(entity->getType()==ENTITY_PADDLE_SPARE)
        m_sparePaddles.push_back(entity);            
    m_sceneNode->addChild(entity->getEntityNode());    
    m_entityList.push_back(entity);
    return true;
}

bool Scene::removeEntity(osg::ref_ptr<Entity> entity)
{
    entity->disablePhysics();
    // if(entity->getSceneParent()==this) 
    m_sceneNode->removeChild(entity->getEntityNode());
    if(entity->getType()==ENTITY_PADDLE)
    {
       ;
    }    
    return true;
}

/*
For the moment entitymanager call this function for removing ball and paddle
removing means hiding for Scene objects
*/
void Scene::handleRemove(osg::ref_ptr<Entity> entity)
{
    LOG_INFO("Scene::handleRemove:%s\n",entity->getName().c_str());
    entity->getEntityNode()->setNodeMask(0);

}

osg::ref_ptr <Entity> Scene::getEntity(EntityType etype)
{
    std::list<osg::ref_ptr<Entity> >::iterator it;

    for ( it=m_entityList.begin(); it!=m_entityList.end(); ++it)
        if((*it)->getType()==etype)        
            return (*it);

    return NULL;
}

void Scene::resetEntities()
{
    std::list<osg::ref_ptr<Entity> >::iterator it;
    for ( it=m_entityList.begin(); it!=m_entityList.end(); ++it)            
        if((*it)->getPhysics()==true)
            (*it)->reset();    
    
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

        ent = createEntity(etype);                    

        if(ent==NULL)
        {
            LOG_ERROR("Entity not found type:%d\n", ent_env_arr[i]);
            continue;
        }
        addEntity(ent); //add entity transform node to scene node        
        if(ent->getType() == ENTITY_BALL)
        {
            EntityBall *eball = ent->asEntityBall();
            eball->setSpeed(0.0);
            eball->setDir(osg::Vec2(0.5, 0.5));
        }
        
    }

    //padle support    
    ent = createEntity("entity_paddle_support");
    addEntity(ent);
    ent->setPhysics(false);
    //spare paddle
    osg::Vec3f pos = ent->getPosition();        
    for(int i=0;i<3;i++)
    {
        ent = createEntity("entity_paddle");                
        ent->setType(ENTITY_PADDLE_SPARE);
        float x = pos.x() -1 + 0.5*i;
        ent->setPosition(osg::Vec3(x, pos.y(), pos.z()));
        addEntity(ent);
        ent->setPhysics(false);
    }    
    ent = createEntity("entity_corner_left");
    ent->setPhysics(false);
    addEntity(ent);
    ent = createEntity("entity_corner_right");
    ent->setPhysics(false);
    addEntity(ent);    
}

void Scene::addSparePaddle(osg::Vec3 pos)
{
    LOG_INFO("Scene::addSparePaddle:%s\n","");
    osg::Vec3 s_pos = AssetsManager::instance().getEntityModelPosition("entity_paddle_support");
    osg::ref_ptr<Entity> spare_paddle = createEntity(ENTITY_PADDLE);                
    spare_paddle->setType(ENTITY_PADDLE_SPARE);
    spare_paddle->setPhysics(false);
    float x = s_pos.x() - 1 + 0.5*m_sparePaddles.size();
    spare_paddle->setPosition(osg::Vec3(x, s_pos.y(), s_pos.z()));
    addEntity(spare_paddle);
}

void Scene::levelContinue()
{   
    
}

void Scene::update(float passedTime)
{
    m_entityMgr.update(passedTime);
    /* remove invalid entity from paddle slots */
    std::list < osg::ref_ptr<Entity> >::iterator it = m_sparePaddles.begin();
    while (it != m_sparePaddles.end())
    {
        (*it)->update(passedTime);        
        if ((*it)->isValid()==false)        
        {
            removeEntity(*it);
            m_sparePaddles.erase(it++);  // alternatively, i = items.erase(i);
        }
        else
        {            
            ++it;
        }
    }    
}

void Scene::animationStart(std::string anim_name)
{   
    LOG_INFO("Scene::startAnimation: %s\n",anim_name.c_str()); 
    std::list < osg::ref_ptr<Entity> >::iterator it;
    for ( it=m_entityList.begin(); it!=m_entityList.end(); ++it)            
        (*it)->getEntityNode()->setNodeMask(0xffffffff);

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

void Scene::animationEnd(std::string anim_name)
{
    LOG_INFO("Scene::endAnimation: %s\n", anim_name.c_str());
    if(anim_name=="animation_level_new")
    {
        m_entityMgr.startPhysics();
    }
    if(anim_name=="animation_level_continue")
    {
        //add ball to entity manager           
        m_entityMgr.addEntity(getEntity(ENTITY_BALL));
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
        ent = getEntity(ENTITY_DOOR_LEFT);
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
        ent = getEntity(ENTITY_PADDLE);                
        if(ent==NULL)       
        {
            LOG_ERROR("Scene::playAnimation: Invalid entity %s\n", "");
            return;
        }
        osg::Vec3 pos = ent->getPosition();            
        x = pos.x()-1.5; y=pos.y(); z=pos.z(); 
        printf("paddle pos: %f %f %f\n", x,y,z);
        ea->addTranslate(0.0, x, y, z);
        ea->addTranslate(2.0, x, y, z);
        ea->addTranslate(3.0, 0.0, y, z);
        ent->getEntityNode()->setUpdateCallback(ea);
    }
    //ball
    {
        EntityAnimation *ea = new EntityAnimation;        
        ent = getEntity(ENTITY_BALL);
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
    osg::ref_ptr<Entity> entity;
    //paddle slot
    {
        osg::Vec3 end_pos  = AssetsManager::instance().getEntityModelPosition("spawn_entity_paddle");
        entity = m_sparePaddles.back();        
        if(entity.valid()==false)
        {
            LOG_ERROR("Scene::animLevelContinue: Invalid entity %s\n", "");
            return;
        }
        EntityAnimation *ea = new EntityAnimation;
        osg::Vec3 pos = entity->getPosition();            
        x = pos.x(); y=pos.y(); z=pos.z();        
        ea->addTranslate(0.0, x, y, z );
        ea->addTranslate(1.0, x, y, z + 1.0);
        ea->addTranslate(2.0, end_pos.x(), end_pos.y(), z+1.0);
        ea->addTranslate(3.0, end_pos.x(), end_pos.y(), end_pos.z());
        ea->setEntity(entity);
        entity->getEntityNode()->setUpdateCallback(ea);
    }
    //ball    
    {
        EntityAnimation *ea = new EntityAnimation;        
        entity=getEntity(ENTITY_BALL);
        if(entity.valid()==false)       
        {
            LOG_ERROR("Scene::animLevelContinue: Invalid entity %s\n", "");
            return;
        }
        osg::Vec3 pos = entity->getPosition();            
        x = pos.x(); y=pos.y(); z=pos.z()-0.5;        
        ea->addTranslate(0.0, x, y, z);
        ea->addTranslate(3.0, x, y, z);
        ea->addTranslate(4.0, x, y, z + 0.5);
        ea->setEventName(anim_name);
        entity->getEntityNode()->setUpdateCallback(ea);        
    } 
    //paddle 
    {
        EntityAnimation *ea = new EntityAnimation;        
        entity=getEntity(ENTITY_PADDLE);
        if(entity.valid()==false)       
        {
            LOG_ERROR("Scene::animLevelContinue: Invalid entity %s\n", "");
            return;
        }
        //entity->reset();
        osg::Vec3 pos = entity->getPosition();
        x = pos.x(); y=pos.y(); z=pos.z()-0.5;        
        ea->addTranslate(0.0, x, y, z);
        ea->addTranslate(3.0, x, y, z);
        ea->addTranslate(3.01, x, y, z + 0.5);
        ea->setEventName(anim_name);
        entity->getEntityNode()->setUpdateCallback(ea);        
    }              
}

void Scene::animLevelCleared(std::string anim_name)
{
    float x,y,z;                
    osg::ref_ptr <Entity> ent;
    //right door
    {            
        EntityAnimation *ea = new EntityAnimation;  
        ent = getEntity(ENTITY_DOOR_RIGHT);        
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