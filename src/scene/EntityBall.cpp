#include <stdio.h>
#include "Logging.h"
#include "EntityBall.h"

EntityBall::EntityBall():Entity()
{
    m_ajustBallDir=false;
}

EntityBall::EntityBall(Entity &ent):Entity(ent)
{
    m_ajustBallDir=false;
}

void EntityBall::update(float passedTime)
{
    Entity::update(passedTime);
    if(m_ajustBallDir == true)
    {
        m_ajustBallDir = false;
        b2Vec2 vel = m_ballSpeed*m_dir;
        m_phyBody->SetLinearVelocity(vel);
    }
    /* 
        minimum velocity threshold for elastic collision 
        changed constant of Box2D b2_velocityThreshold = 0.01 
    */
#if 0    
    if(m_ajustBallSpeed == true)
    {
        float threshold = 1.1;
        m_ajustBallSpeed=false;
        b2Vec2 vel = m_phyBody->GetLinearVelocity();
        vel.x = vel.x < 0.0 ? -threshold:threshold;        
        vel.y = vel.y < 0.0 ? -threshold:threshold;
        vel.Normalize();
        vel *= m_ballSpeed;
        m_phyBody->SetLinearVelocity(vel);
        printf("Velocity changed:%f,%f\n",vel.x, vel.y);
    }
#endif    
}

void EntityBall::beginContact(Entity *ent, b2Contact *contact)
{    
#if 0
    b2Vec2 vel = m_phyBody->GetLinearVelocity();
    printf("begin_contact: %s (%f,%f)\n",ent->getName().c_str(),vel.x,vel.y);
    float th = 0.2;
    if( (abs(vel.x) < th)||(abs(vel.y) < th) )
    { 
        m_ajustBallSpeed = true;                  
    }
#endif
    if(ent->getType()==ENTITY_PADDLE)
    {
        int numPoints = contact->GetManifold()->pointCount;
        b2WorldManifold worldManifold;
        contact->GetWorldManifold( &worldManifold );
        b2Vec2 pos = ent->getPhyBody()->GetPosition();
        float local_x = worldManifold.points[0].x - pos.x;
        //printf("EntityBall::endContact %d %f %f\n",numPoints, local_x,worldManifold.points[0].x);
        if((local_x>=-0.1)&&(local_x<=0.1))
        {
            //printf("HIT center\n");
        }
        else if(local_x < -0.1)
        {            
            m_dir.x = cos(2*3.14/3);
            m_dir.y = sin(2*3.14/3);
            m_ajustBallDir = true;
        }
        else if(local_x > 0.1)
        {            
            m_dir.x = cos(3.14/3);
            m_dir.y = sin(3.14/3);
            m_ajustBallDir = true;
        }
    }
}
void EntityBall::setSpeed(float speed)
{
    m_ballSpeed = speed;
    if(m_phyActive == true)
    {
        b2Vec2 ball_vel = m_dir;
        ball_vel *= m_ballSpeed;
        m_phyBody->SetLinearVelocity(ball_vel);
    }    
}

float EntityBall::getSpeed()
{
    return m_ballSpeed;
}

void EntityBall::setDir(osg::Vec2 dir)
{
    m_dir.x = dir.x();
    m_dir.y = dir.y();
    m_dir.Normalize();
}

void EntityBall::endContact(Entity *ent, b2Contact *contact)
{    
    
}

bool EntityBall::enablePhysics()
{
    if(m_phyActive == true)
    {
        LOG_WARN("%s", "EntityBall::enablePhysics(): Physics already active\n");
        return true;
    }
    printf("EntityBall::enablePhysics(): Physics enable for entity:%s\n",m_name.c_str());
    osg::Geode *geo = dynamic_cast<osg::Geode*>(m_transform->getChild(0));
    if(geo==NULL)
    {
        printf("EntityBall::enablePhysics(): dynamic cast to geode fail\n");
        return false;
    }
    setCircleShape(geo);
    m_phyActive = true;   
    updateInitialTransform();   
    setCategoryBits(m_type);            
#if 0    
    m_ballSpeed = 2.0;
    b2Vec2 ball_vel(1.0f,1.0f);
    ball_vel.Normalize();
    ball_vel = m_dir * m_ballSpeed;
#endif    
    b2Vec2 ball_vel = m_dir;
    ball_vel *= m_ballSpeed;
    m_phyBody->SetLinearVelocity(ball_vel);
    
    return m_phyActive;
}

bool EntityBall::disablePhysics()
{
    return Entity::disablePhysics();
    
}
