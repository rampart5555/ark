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

b2World *EntityManager::m_world = NULL;

EntityManager::EntityManager()
{
    m_sceneId = ENTITY_MANAGER;
    m_nodeEntMgr = new osg::MatrixTransform();
    m_paddle = NULL;
    m_entitiesNum = 0;
    m_brickNumber = 0;
    m_powerupNumber = 0;
    m_paddleSelected=false;
}

osg::MatrixTransform* EntityManager::getNodeEntMgr()
{
    return m_nodeEntMgr.get();
}

void EntityManager::loadShaders()
{
    osg::Program *program = AssetsManager::instance().getProgram("model_color");
    osg::StateSet *ss = m_nodeEntMgr->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON );
}

void EntityManager::startPhysics()
{
    LOG_STATE("*** EntityManager::enablePhysics() ***%s\n","");
    if(m_physicsActive==true)
    {
        LOG_INFO("%s", "Physics already enabled\n");
        return;
    }   
    
    LOG_INFO("Number of entities:%lu\n", m_entityList.size());    
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
    

    /*create a joint between paddle and ball */
    EntityBall *ball = m_ballList.front();
    b2WeldJointDef jointDef;
    jointDef.bodyA = m_paddle->getPhyBody();
    jointDef.bodyB = ball->getPhyBody();       
    jointDef.localAnchorA.y = 0.05;        
    jointDef.localAnchorB.y = -0.05;
    m_ballJoint = (b2WeldJoint*) EntityManager::m_world->CreateJoint(&jointDef);
    
}

void EntityManager::stopPhysics()
{
    LOG_DEBUG("%s", "*** EntityManager::disablePhysics() *** \n");

    paddleUnselect(NULL); //avoid crash here

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
}

void EntityManager::pausePhysics()
{
    m_physicsActive = false;
}

void EntityManager::resumePhysics()
{
    m_physicsActive = true;
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
    ent->setSceneParent(this);
    return ent;
}

void EntityManager::addEntity(osg::ref_ptr<Entity> entity)
{   
    
    if(entity  == NULL)
    {
        LOG_WARN("%s", "EntityManager::addEntity: NULL entity\n");
        return;
    }
    LOG_INFO("EntityManager::addEntity=> %s type: %d\n",
        entity->getName().c_str(),entity->getType());
    m_entityList.push_back(entity);

    if(entity->getSceneParent()==this)
        m_nodeEntMgr->addChild(entity->getEntityNode());

    if(entity->getType() == ENTITY_PADDLE)
        m_paddle = entity->asEntityPaddle();
    else if(entity->getType() == ENTITY_BRICK)                    
        m_brickNumber++;        
    else if(entity->getType() == ENTITY_POWERUP)        
        m_powerupNumber++;
    else if(entity->getType()==ENTITY_BALL)     
        m_ballList.push_back(entity->asEntityBall());
    m_entitiesNum++;
    /* enable physics for spawned entities */       
    if(m_physicsActive == true)
        entity->enablePhysics();
    entity->setUniforms();
    
}

void EntityManager::spawnPowerup(Entity *entity)
{
    const char *ptype;
    ptype = powerupToStr(entity->getPowerup());
    LOG_INFO("Spawn powerup: %s\n", ptype);
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
    LOG_INFO("Apply powerup:%s\n", powerupToStr( ptype));
    switch(ptype)
    {
        case POWERUP_BALLS:
            {                
                unsigned int blst_size = m_ballList.size();
                std::list< EntityBall* >::iterator it = m_ballList.begin();
                for(unsigned int i=0; i<blst_size; i++)
                {
                    EntityBall *ball = *it++;                    
                    for(int j=0;j<3;j++)
                    {
                        osg::ref_ptr<Entity> ent = createEntity(ENTITY_BALL);
                        EntityBall *new_ball = ent->asEntityBall();
                        const osg::Vec3d& ent_pos = ball->getPosition();
                        new_ball->setPosition(ent_pos);
                        new_ball->setSpeed(DEFAULT_BALL_SPEED);
                        if(j==0)
                            new_ball->setDir(osg::Vec2(-0.5, 0.5) );
                        if(j==1)    
                            new_ball->setDir(osg::Vec2( 0.5, -0.5) );
                        if (j==2)
                            new_ball->setDir( osg::Vec2( 0.0, 1.0) );                           
                        addEntity(ent);
                        new_ball->start();
                    }
                }                                  
            }         
            break;
        case POWERUP_SLOW:    
        case POWERUP_FAST:
            {            
                
                std::list<EntityBall*>::iterator it;
                for (it = m_ballList.begin(); it != m_ballList.end(); it++)
                {                                        
                    EntityBall *ball = (*it);
                    float speed = ball->getSpeed();
                    if(ptype == POWERUP_SLOW)
                        ball->changeSpeed(speed * 0.8);
                    else
                        ball->changeSpeed(speed * 1.2);
                            
                }                                
            }    
            break;
        case POWERUP_LIFE:            
            break;  
                  
        case POWERUP_CANNON:            
            break;
            
        default:
            break;
        
    }
}

void EntityManager::removeEntity(osg::ref_ptr<Entity> entity)
{
    entity->disablePhysics();
    if(entity->getSceneParent()==this)
        m_nodeEntMgr->removeChild(entity->getEntityNode());
    else
    {
        BasicScene *bs=entity->getSceneParent();
        if(bs!=NULL)
            bs->handleRemove(entity);
    }
    if(entity->getType() == ENTITY_BRICK)
    {
        m_entitiesNum--;
        m_brickNumber--;
        // update score   
        int score =  entity->getValue();        
        Level_update_score(&score);                
        if(entity->getPowerup() > 0)
        {
            spawnPowerup(entity);
        }
        if((m_brickNumber <= 0))
        {               
            LOG_STATE("EntityManager: %s\n","*** LEVEL CLEARED ***");
            EngineEvent *ev = new EngineEvent;
            ev->m_eventId = LEVEL_CLEARED;
            EngineEventQueue::instance().setEvent(ev);            
        }        
    }
    else if(entity->getType() == ENTITY_POWERUP)
    {
        m_entitiesNum--;
        m_powerupNumber--;
        PowerupType ptype = static_cast<PowerupType>(entity->getSubType());
        setPowerup(ptype);
    }
    else if(entity->getType() == ENTITY_BALL)
    {        
        m_ballList.remove(entity->asEntityBall());
        if(m_ballList.size()==0)
        {
            LOG_STATE("EntityManager: %s\n","*** LEVEL FAILED ***");
            EngineEvent *ev = new EngineEvent;
            if(m_brickNumber>0)
                ev->m_eventId = LEVEL_FAILED;
            else //all bricks are removed so move to the next level
                ev->m_eventId = LEVEL_COMPLETED;
            EngineEventQueue::instance().setEvent(ev);            
        }
    }    
}

void EntityManager::clear()
{
    if(m_physicsActive==true)
    {
        LOG_WARN("%s", "Disable physics first\n");
        return;
    }    
    m_nodeEntMgr->removeChildren(0, m_entityList.size());       
    m_entityList.clear();
    m_ballList.clear();
    m_entitiesNum = 0;
    m_brickNumber = 0;
    m_powerupNumber = 0;    
}

void EntityManager::update(float passedTime)
{    
    if(m_physicsActive==false)
        return;

    m_world->Step( passedTime, 8, 3);
    
    std::list<osg::ref_ptr<Entity> >::iterator it, tmp_it;
    for(it=m_entityList.begin(); it!=m_entityList.end(); it++)
    {
        (*it)->update(passedTime);
        if ((*it)->isValid()==false)        
        {
            tmp_it=it++;
            removeEntity(*tmp_it);
            m_entityList.erase(tmp_it);            
        }
    }   
}

void EntityManager::paddleSelect(void *args)
{
    //debug_log(ENTRY, "LEVEL_PADDLE_SELECT\n");
    if( (m_paddle==NULL) || (m_paddle->phyActive()==false)|| (args==NULL) )
        return;
    m_paddleSelected=true;
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
    if(m_paddleSelected==false)
        return;

    LOG_DEBUG("%s\n", "LEVEL_PADDLE_UNSELECT");
    if (m_mouseJoint != NULL)
    {
        EntityManager::m_world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
    }
    if(m_ballJoint!=NULL)
    {
        EntityManager::m_world->DestroyJoint(m_ballJoint);
        m_ballJoint=NULL;
        EntityBall *ball = m_ballList.front();
        ball->setDir(osg::Vec2(0.11,1.0));
        ball->setSpeed(DEFAULT_BALL_SPEED);
        ball->start();
       
    }
    m_paddleSelected=false;
}

/* debug function*/
const char* EntityManager::powerupToStr(PowerupType ptype)
{
    switch(ptype)
    {
        case POWERUP_BALLS:
            return "POWERUP_BALLS";
        case POWERUP_FAST:  
            return "POWERUP_FAST";
        case POWERUP_SLOW:  
            return "POWERUP_SLOW";
        case POWERUP_LIFE:  
            return "POWERUP_LIFE";
        case POWERUP_CANNON:
            return "POWERUP_CANNON";
        default:
            return "POWERUP_NONE";
    }
}
