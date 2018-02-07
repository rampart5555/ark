#ifndef __ENTITY_PADDLE__
#define __ENTITY_PADDLE__
#include "Entity.h"
class EntityPaddle : public Entity
{
     public:
        EntityPaddle();   
        EntityPaddle(Entity& ent);
        virtual void update(float);
        virtual void beginContact(Entity*, b2Contact*);
        virtual void endContact(Entity*, b2Contact*);
        virtual bool enablePhysics();
        virtual bool disablePhysics();
        virtual void reset();
        virtual EntityPaddle* asEntityPaddle(){ return this; }                
        void setJoint(b2Body* );

    private:
        osg::ref_ptr<osg::MatrixTransform> m_turret;
        osg::Vec3 m_cannon_left_pos;
        osg::Vec3 m_cannon_right_pos;
        bool m_hasTurret;
        int m_index;        
};

#endif
