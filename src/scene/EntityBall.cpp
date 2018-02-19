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
 
}

void EntityBall::beginContact(Entity *ent, b2Contact *contact)
{    
    if(ent->getType()==ENTITY_PADDLE)
    {
        //int numPoints = contact->GetManifold()->pointCount;
        b2WorldManifold worldManifold;
        contact->GetWorldManifold( &worldManifold );
        b2Vec2 pos = ent->getPhyBody()->GetPosition();
        float local_x = worldManifold.points[0].x - pos.x;
        float local_y = 0.25 - abs(local_x);

        float res = atan2 (local_y,local_x) * 180 / osg::PI;
        if(res >90.0) res=180.0-res;
        
        m_dir.x = local_x;
        m_dir.y = local_y;
        m_dir.Normalize();
        m_ajustBallDir = true;
#if  0
        b2Vec2 vel = m_phyBody->GetLinearVelocity();
        //printf("EntityBall::endContact %d %f %f\n",numPoints, local_x,worldManifold.points[0].x);
        //R = 2*(V dot N)*N - V -reflexion vector
        // define dx=x2-x1 and dy=y2-y1, then the normals are (-dy, dx) and (dy, -dx).
        printf("local_x:%f\n",local_x);
        b2Vec2 n(local_x, 0.25-local_x);
        n.Normalize();
        const b2Vec2& r = -2*b2Dot(vel,n)*n + vel;
        m_phyBody->SetLinearVelocity(r);
        return;
      
        if((local_x>=-0.13)&&(local_x<=0.13))
        {
            m_dir.x = cos(3.14/2);
            m_dir.y = sin(3.14/2);
            m_ajustBallDir = true;
        }
        else if(local_x < -0.13)
        {            
            m_dir.x = cos(2*3.14/3);
            m_dir.y = sin(2*3.14/3);
            m_ajustBallDir = true;
        }
        else if(local_x > 0.13)
        {            
            m_dir.x = cos(3.14/3);
            m_dir.y = sin(3.14/3);
            m_ajustBallDir = true;
        }        
#endif          
    }
    if(ent->getType()==ENTITY_WALL_BOTTOM)
    {
        m_valid = false;
    }
}

void EntityBall::setSpeed(float speed)
{
    m_ballSpeed = speed;    
}

void EntityBall::changeSpeed(float new_speed)
{
    m_ballSpeed = new_speed;
    if(m_phyActive == true)
    {
        b2Vec2 s_vec = m_phyBody->GetLinearVelocity();
        s_vec *= new_speed;
        m_phyBody->SetLinearVelocity(s_vec);
    }
}

void EntityBall::start()
{
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
    LOG_INFO("EntityBall::enablePhysics(): Physics enable for entity:%s\n",m_name.c_str());
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
    b2Vec2 ball_vel = m_dir;
    ball_vel *= m_ballSpeed;
    m_phyBody->SetLinearVelocity(ball_vel);
#endif       
    return m_phyActive;
}

bool EntityBall::disablePhysics()
{
    return Entity::disablePhysics();
    
}
