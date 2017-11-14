#ifndef __ENTITY_BALL__
#define __ENTITY_BALL__
#include "Entity.h"
class EntityBall : public Entity
{
     public:
        EntityBall();   
        EntityBall(Entity& ent);
        virtual void update();
        virtual void beginContact(Entity*, b2Contact*);
        virtual void endContact(Entity*, b2Contact*);
        virtual bool enablePhysics();
        virtual bool disablePhysics();
        virtual EntityBall* asEntityBall(){ return this; }
        void setSpeed(float);
        float  getSpeed();  
        void setDir(osg::Vec2 dir);
        //osg::Vec2& getDir();
        
     private:
         float m_ballSpeed;
         bool  m_ajustBallDir;
         b2Vec2 m_dir;         
};
#endif
