#include <stdio.h>
#include "Logging.h"
#include "EntityPaddle.h"
#include "EntityManager.h"
#include "Scene.h"

class FindBulletSpawn : public osg::NodeVisitor
{
    public:
        osg::Vec3 cannon_left_pos;
        osg::Vec3 cannon_right_pos;                        
        FindBulletSpawn(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
                        
        } 
        void apply(osg::MatrixTransform& tr)
        {              
            if((tr.getName()=="spawn_entity_bullet_right")||(tr.getName()=="spawn_entity_bullet_left"))
            {
                osg::Vec3f pos,scale;
                osg::Quat rot, so;
                osg::Matrix m = tr.getMatrix();
                m.decompose(pos, rot, scale, so); 
                if(tr.getName()=="spawn_entity_bullet_right")  
                    cannon_left_pos.set(pos);
                else
                    cannon_right_pos.set(pos);                                   
            }
            traverse(tr);
        }
};

EntityPaddle::EntityPaddle():Entity()
{
    m_hasTurret = false;
    m_index = 0;
}

EntityPaddle::EntityPaddle(Entity &ent):Entity(ent)
{
    m_hasTurret = false;
    m_index = 0;
}

void EntityPaddle::update(float passedTime)
{
    Entity::update(passedTime);
    if((m_hasTurret == true) && (m_index > 60))
    {
        m_index = 0;
        LOG_INFO("%s\n","spwnEntityBullet bullet");
        /* 
            compute paddle position and canon_left/cannon right 
        */
        if(m_phyActive==true)
        {
            const b2Vec2& pos = m_phyBody->GetPosition();
            osg::Vec3 bullet_pos_r;
            bullet_pos_r += osg::Vec3(pos.x,pos.y,0.0);
            bullet_pos_r += m_cannon_right_pos;
            osg::Vec3 bullet_pos_l;
            bullet_pos_l += osg::Vec3(pos.x,pos.y,0.0);
            bullet_pos_l += m_cannon_left_pos;
            Scene::instance().getEntityManager().spawnEntity(ENTITY_BULLET, bullet_pos_r);
            Scene::instance().getEntityManager().spawnEntity(ENTITY_BULLET, bullet_pos_l);
        }
    }
    m_index++;        
}

void EntityPaddle::beginContact(Entity *ent, b2Contact *contact)
{
    if((ent->getType()==ENTITY_POWERUP) && (ent->getSubType() == POWERUP_CANNON))
    { 
        if(m_hasTurret==true)
            return;
        LOG_INFO("%s","EntityPaddle::beginContact==> Attach cannon\n");
        m_turret = AssetsManager::instance().getEntityModel(ENTITY_TURRET);
        m_turret->setMatrix(osg::Matrix::translate(0.0,0.0,0.0));
        m_transform->addChild(m_turret);
        AssetsManager::instance().getEntityModel(ENTITY_TURRET);
        
        /* get bullet spawn position */
        FindBulletSpawn fbs;
        m_turret->accept(fbs);
        m_cannon_left_pos.set(fbs.cannon_left_pos);
        m_cannon_right_pos.set(fbs.cannon_right_pos);
        
        /* has turret */
        m_hasTurret = true;
    }
}

void EntityPaddle::endContact(Entity *ent, b2Contact *contact)
{    
    
}

void EntityPaddle::setJoint(b2Body *background)
{     
    b2PrismaticJointDef pjd;
    b2Vec2 axis(1.0f, 0.0f);
    pjd.Initialize(background, m_phyBody, b2Vec2(0.0f, 0.0f), axis);
    pjd.enableMotor = false;    
    m_phyJoint = (b2PrismaticJoint*)EntityManager::m_world->CreateJoint(&pjd);
}

bool EntityPaddle::enablePhysics()
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
    setBoxShape(geo);
    m_phyActive = true;   
    updateInitialTransform();   
    setCategoryBits(m_type);            
    
    return m_phyActive;
}    

bool EntityPaddle::disablePhysics()
{
    return Entity::disablePhysics();
    
}
