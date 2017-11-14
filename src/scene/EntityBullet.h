#ifndef __ENTITY_BULLET__
#define __ENTITY_BULLET__
#include "Entity.h"
class EntityBullet : public Entity
{
     public:
        EntityBullet();   
        EntityBullet(Entity& ent);
        virtual void update();
        virtual void beginContact(Entity*, b2Contact*);
        virtual void endContact(Entity*, b2Contact*);
        virtual bool enablePhysics();
        virtual bool disablePhysics();
        virtual EntityBullet* asEntityBullet(){ return this; }        
        
        void setSpeed(float);
        float  getSpeed();  
        void setDir(osg::Vec2 dir);
                
    private:
        float m_bulletSpeed;
        b2Vec2 m_dir;
};
#endif
