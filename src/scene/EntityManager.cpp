#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/NodeVisitor>
#include <osgDB/WriteFile>
#include "Logging.h"
#include "Entity.h"
#include "Scene.h"
#include "EntityManager.h"
#include "EngineCallback.h"

#define SDL_Log printf

class EntityManagerCallback : public osg::NodeCallback
{
    public:
        EntityManagerCallback(EntityManager *em)
        {
            m_entityMgr = em;
            m_active = false;
        }
        ~EntityManagerCallback()
        {
            
        }
        void operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
            if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR)
            {
                /* skip first frame */
                float dt;
                const osg::FrameStamp* fs = nv->getFrameStamp();
                if(m_active==false)
                {
                    dt = 0.016;
                    m_active=true;
                }
                else                
                    dt = fs->getSimulationTime() - m_passedTime;
                m_passedTime = fs->getSimulationTime();
                //printf("%f\n",dt);
                m_entityMgr->update(dt);
            }
        }        
    private:
        EntityManager *m_entityMgr;
        bool m_active;
        float m_passedTime;
};

b2World *EntityManager::m_world = NULL;

EntityManager::EntityManager()
{
    m_sceneNode = new osg::MatrixTransform();
    m_paddle = NULL;
    m_entitiesNum = 0;
    
}

void EntityManager::setSceneNode(osg::MatrixTransform *node)
{
    m_sceneNode = node;
}

void EntityManager::startPhysics()
{
    LOG_DEBUG("%s","*** EntityManager::enablePhysics() ***\n");
    if(m_physicsActive==true)
    {
        LOG_INFO("%s", "Physics already enabled\n");
        return;
    }   
    
    m_world = new b2World(b2Vec2(0.0, 0.0));

   
    m_physicsActive = true;        
    std::list<osg::ref_ptr<Entity> >::iterator it;
    for(it=m_entityList.begin(); it!=m_entityList.end(); it++)
    {
        (*it)->enablePhysics();
    }   
    m_contactListener = new ContactListener();     
    m_world->SetContactListener(m_contactListener);
    /*add prismatic joint to paddle */
    b2BodyDef bodyDef;
    m_groundBody = m_world->CreateBody(&bodyDef);    
    m_paddle->setJoint(m_groundBody);
    //m_paddle->getPhyBody()->Dump();
    //m_bottomWall->getPhyBody()->Dump();
    LOG_INFO("Number of entities:%lu", m_entityList.size());
    m_sceneNode->setUpdateCallback(new EntityManagerCallback(this) );
    
}

void EntityManager::stopPhysics()
{
    LOG_DEBUG("%s", "*** EntityManager::disablePhysics() *** \n");
    if(m_physicsActive==false)
    {
        LOG_INFO("%s", "Physics already disabled\n");
        return;
    }
    m_physicsActive = false; 
    std::list<osg::ref_ptr<Entity> >::iterator it;
    for(it=m_entityList.begin(); it!=m_entityList.end(); it++)
    {
        (*it)->disablePhysics();
    }   
    if (m_mouseJoint != NULL)
    {
        EntityManager::m_world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
    }                          
    if (m_groundBody != NULL)
    {
        EntityManager::m_world->DestroyBody(m_groundBody);
        m_groundBody = NULL;
    }
    m_sceneNode->setUpdateCallback(NULL);
}

osg::ref_ptr <Entity> EntityManager::createEntity(EntityType etype)
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
    else if(etype==ENTITY_PADDLE)
        ent = new EntityPaddle();
    else if(etype==ENTITY_BULLET)
        ent = new EntityBullet();
    else
        ent = new Entity();
    ent->setType(etype);
    ent->setModel(*model);
    ent->setName(model->getName().c_str());    
    return ent;
}

void EntityManager::addEntity(osg::ref_ptr<Entity> entity)
{   
    LOG_DEBUG("EntityManager::addEntity=> %s\n",entity->getName().c_str());
    if(entity  == NULL)
    {
        LOG_WARN("%s", "EntityManager::addEntity: NULL entity\n");
        return;
    }
    if(m_sceneNode != NULL)
    {        
        m_entityList.push_back(entity);
        m_sceneNode->addChild(entity->getEntityNode());
        if(entity->getType() == ENTITY_PADDLE)
            m_paddle = entity->asEntityPaddle();
        else if((entity->getType() == ENTITY_BRICK)||(entity->getType() == ENTITY_POWERUP))
            m_entitiesNum++;     
        /* enable physics for spawned entities */       
        if(m_physicsActive == true)
            entity->enablePhysics();
        entity->setUniforms();
    }
}

void EntityManager::spawnPowerup(Entity *entity)
{
    LOG_INFO("SPAWN POWERUP: %d\n", entity->getPowerup());
    //m_level->spawnPowerup(entity.get());
    osg::ref_ptr<Entity> pu_ent = createEntity(ENTITY_POWERUP);
    if(pu_ent == NULL)
    {
        LOG_WARN("%s", "Powerup entity not created.\n");
        return;
    }
    pu_ent->setPosition( entity->getPosition() );
    pu_ent->setSubType( entity->getPowerup() );
    Scene::instance().addEntityProps(pu_ent);
    addEntity(pu_ent);
}

void EntityManager::spawnEntity(EntityType etype, osg::Vec3 pos)
{
    if(etype==ENTITY_BULLET)
    {
        osg::ref_ptr<Entity> ent = createEntity(ENTITY_BULLET);
        osg::ref_ptr<EntityBullet> ent_b = ent->asEntityBullet();
        ent_b->setPosition(pos);
        ent_b->setSpeed(2.0);
        ent_b->setDir(osg::Vec2(0.0,1.0));
        addEntity(ent_b);
    }
}

void EntityManager::setPowerup(PowerupType ptype)
{

    switch(ptype)
    {
        case POWERUP_BALLS:
            {
                LOG_INFO("Powerup BALLS %d \n", ptype);
                
                std::list< osg::ref_ptr<EntityBall> > new_ball_list;
                std::list< osg::ref_ptr<Entity> >::iterator it;                           
                for (it = m_entityList.begin(); it != m_entityList.end(); ++it)
                {
                    if((*it)->getType() == ENTITY_BALL)
                    {
                        for(int i=0;i<3;i++)
                        {
                            //EntityBall *ball = dynamic_cast<EntityBall*> ((*it).get());
                            EntityBall *ball = (*it)->asEntityBall();
                            osg::ref_ptr<Entity> ent = createEntity(ENTITY_BALL);
                            EntityBall *new_ball = ent->asEntityBall();
                            osg::Vec3 &ent_pos = ball->getPosition();
                            new_ball->setPosition(ent_pos);
                            new_ball->setSpeed(2.0);
                            if(i==0)
                                new_ball->setDir(osg::Vec2(-0.5, 0.5) );
                            if(i==1)    
                                new_ball->setDir(osg::Vec2( 0.5, -0.5) );
                            if (i==2)
                                new_ball->setDir( osg::Vec2( 0.0, 1.0) );
                            new_ball_list.push_back(new_ball);    
                        }
                    }                    
                }
                /* avoid infinite loop if new ball is added to m_entityList*/
                std::list< osg::ref_ptr<EntityBall> >::iterator it_ball;    
                for(it_ball = new_ball_list.begin(); it_ball != new_ball_list.end(); ++it_ball)
                    addEntity(*it_ball);
                new_ball_list.clear();    
            }         
            break;
        case POWERUP_SLOW:    
        case POWERUP_FAST:
            {            
                LOG_INFO("Powerup FAST/SLOW %d\n", ptype);            
                std::list< osg::ref_ptr<Entity> >::iterator it;
                for (it = m_entityList.begin(); it != m_entityList.end(); ++it)
                {                    
                    if((*it)->getType() == ENTITY_BALL)
                    {
                        EntityBall *ball = (*it)->asEntityBall();
                        int speed = ball->getSpeed();
                        if(ptype == POWERUP_SLOW)
                            ball->setSpeed(speed - 1.0);
                        else
                            ball->setSpeed(speed + 1.0);
                            
                    }                        
                }                                
            }    
            break;
        case POWERUP_LIFE:
            LOG_INFO("Powerup LIFE %d\n", ptype);
            break;  
                  
        case POWERUP_CANNON:
            LOG_INFO("Powerup CANNON %d\n", ptype);
            break;
            
        default:
            break;
        
    }
}

void EntityManager::removeEntity(osg::ref_ptr<Entity> entity)
{
    entity->disablePhysics();
    m_sceneNode->removeChild(entity->getEntityNode());
    if(entity->getType() == ENTITY_BRICK)
    {
        m_entitiesNum--;
        // update score   
        int score =  entity->getValue();        
        MenuSceneHud_update_score(&score);
                
        if(entity->getPowerup() > 0)
        {
            spawnPowerup(entity);
        }
    }
    else if(entity->getType() == ENTITY_POWERUP)
    {
        m_entitiesNum--;
        PowerupType ptype = static_cast<PowerupType>(entity->getSubType());
        setPowerup(ptype);
    }
}

void EntityManager::clear()
{
    if(m_physicsActive==true)
    {
        LOG_WARN("%s", "Disable physics first\n");
        return;
    }
    std::list< osg::ref_ptr<Entity> >::iterator it;
    for (it=m_entityList.begin(); it!=m_entityList.end(); ++it)
    {
        m_sceneNode->removeChild((*it)->getEntityNode());
    }    
    m_entityList.clear();
    m_entitiesNum = 0;
}

void EntityManager::update(float passedTime)
{    
    if(m_physicsActive==false)
        return;

    m_world->Step( passedTime, 8, 3);
    
    std::list<osg::ref_ptr<Entity> >::iterator it, tmp_it;
    for(it=m_entityList.begin(); it!=m_entityList.end(); it++)
    {
        (*it)->update();
        if ((*it)->isValid()==false)        
        {
            tmp_it=it++;
            removeEntity(*tmp_it);
            m_entityList.erase(tmp_it);
            
        }
    }
    if(m_entitiesNum <= 0)
    {        
        levelComplete();
    }
}

void EntityManager::paddleSelect(void *args)
{
    //debug_log(ENTRY, "LEVEL_PADDLE_SELECT\n");
    if( (m_paddle==NULL) || (m_paddle->phyActive()==false)|| (args==NULL) )
        return;
    
    Event_XY *event_xy = static_cast<Event_XY*>(args);        
    b2Body *body = m_paddle->getPhyBody();    
    b2Vec2 p(event_xy->x, event_xy->y);
    b2MouseJointDef md;
    md.bodyA = m_groundBody;
    md.bodyB = body;
    md.target = p;
    md.maxForce = 1000.0f * body->GetMass();
    m_mouseJoint = (b2MouseJoint*)EntityManager::m_world->CreateJoint(&md);
    body->SetAwake(true);
}

void EntityManager::paddleMove(void *args)
{
    //debug_log(ENTRY, "LEVEL_PADDLE_MOVE:(%f,%f)\n", pargs->x, pargs->y);
    if( (m_paddle==NULL) || (m_paddle->phyActive()==false)|| (args==NULL) )
        return;
    Event_XY *event_xy = static_cast<Event_XY*>(args);
    b2Vec2 p(event_xy->x, event_xy->y);
    if (m_mouseJoint != NULL)
    {
        m_mouseJoint->SetTarget(p);
    }
}

void EntityManager::paddleUnselect(void *args)
{
    LOG_DEBUG("%s", "LEVEL_PADDLE_UNSELECT\n");
    if (m_mouseJoint != NULL)
    {
        EntityManager::m_world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
    }
}

void EntityManager::levelComplete()
{
    LOG_INFO("%s", "*** LEVEL COMPLETE ***\n");
    m_physicsActive = false;
    Scene_level_complete(NULL);
    //EngineEventArgs args;
    //args.arg1 = GameManager::instance()->getLevelScore();
    //GameManager::instance()->handleMenuEvent(MENU_LEVEL_COMPLETE_SHOW, &args);
}
