#include <stdio.h>
#include "Logging.h"
#include "EntityBullet.h"

EntityBullet::EntityBullet():Entity()
{

}

EntityBullet::EntityBullet(Entity &ent):Entity(ent)
{
    
}

void EntityBullet::update(float passedTime)
{
    Entity::update(passedTime);
    
}

void EntityBullet::beginContact(Entity *ent, b2Contact *contact)
{       
    if((ent->getType()==ENTITY_BRICK)||(ent->getType()==ENTITY_WALL_TOP))
    {         
         m_valid = false;
    }
}

void EntityBullet::endContact(Entity *ent, b2Contact *contact)
{    
    
}

void EntityBullet::setSpeed(float speed)
{
    m_bulletSpeed = speed;
    if(m_phyActive == true)
    {
        b2Vec2 ball_vel = m_dir;
        ball_vel *= m_bulletSpeed;
        m_phyBody->SetLinearVelocity(ball_vel);
    }    
}

float EntityBullet::getSpeed()
{
    return m_bulletSpeed;
}

void EntityBullet::setDir(osg::Vec2 dir)
{
    m_dir.x = dir.x();
    m_dir.y = dir.y();
    m_dir.Normalize();
}



bool EntityBullet::enablePhysics()
{
    if(m_phyActive == true)
    {
        LOG_WARN("%s", "EntityBall::enablePhysics(): Physics already active\n");
        return true;
    }    
    osg::Geode *geo = dynamic_cast<osg::Geode*>(m_transform->getChild(0));
    if(geo==NULL)
    {
        LOG_WARN("EntityBall::enablePhysics(): dynamic cast to geode fail %s\n","");
        return false;
    }
    setCircleShape(geo);
    m_phyActive = true;   
    updateInitialTransform();   
    setCategoryBits(m_type); 
    
    b2Vec2 vel = m_dir;
    vel *= m_bulletSpeed;
    m_phyBody->SetLinearVelocity(vel);
        
    return m_phyActive;
}

bool EntityBullet::disablePhysics()
{
    return Entity::disablePhysics();
    
}
