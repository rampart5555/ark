#ifndef __ENTITY_MANAGER__
#define __ENTITY_MANAGER__
#include <osg/PositionAttitudeTransform>
#include <Box2D/Box2D.h>
#include "AssetsManager.h"
#include "Entity.h"
#include "EntityBall.h"
#include "EntityPaddle.h"
#include "EntityBullet.h"
#include "Config.h"
#include "BasicScene.h"
/* alias for World Manager for entities */
class Scene;

class EntityManager: public BasicScene
{
    public:
        static b2World *m_world;
        
        EntityManager();              
        void startPhysics();
        void stopPhysics();
        void pausePhysics();
        void resumePhysics();
        osg::ref_ptr <Entity> createEntity(EntityType etype);
        void addEntity(osg::ref_ptr<Entity>);        
        void removeEntity(osg::ref_ptr<Entity>);        
        void clear();
        void update(float passedTime);
        std::list < osg::ref_ptr<Entity> >* getEntityList();
        
        void paddleSelect(void *args);
        void paddleMove(void *args);       
        void paddleUnselect(void *args);        
        void spawnPowerup(Entity*);
        void spawnEntity(EntityType etype, osg::Vec3 pos);
        void setPowerup(PowerupType);
        
        //debug utilities
        const char* powerupToStr(PowerupType);
        osg::MatrixTransform* getNodeEntMgr();
        void loadShaders();
        
     private:        
        std::list < osg::ref_ptr<Entity> > m_entityList;
        /* no ref_ptr for ball list */
        std::list < EntityBall *> m_ballList;
        std::list < osg::ref_ptr <Entity> > m_paddleList;
        bool m_physicsActive;
        bool m_paddleSelected;        
        osg::ref_ptr<osg::MatrixTransform> m_nodeEntMgr;
        EntityPaddle *m_paddle;
        int m_entitiesNum;
        int m_brickNumber;
        int m_powerupNumber;
        ContactListener *m_contactListener;
        b2MouseJoint *m_mouseJoint;
        b2WeldJoint  *m_ballJoint;
        b2Body* m_groundBody;
        
        
};
#endif

