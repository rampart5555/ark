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
/* alias for World Manager for entities */
class Scene;
class EntityManager
{
    public:
        static b2World *m_world;
        
        EntityManager();      
        void setSceneNode(osg::MatrixTransform*);
        void startPhysics();
        void stopPhysics();
        osg::ref_ptr <Entity> createEntity(EntityType etype);
        void addEntity(osg::ref_ptr<Entity>);        
        void removeEntity(osg::ref_ptr<Entity>);
        void clear();
        void update(float passedTime);
        std::list < osg::ref_ptr<Entity> >* getEntityList();
        void levelComplete(); 
        void paddleSelect(void *args);
        void paddleMove(void *args);       
        void paddleUnselect(void *args);
        void spawnPowerup(Entity*);
        void spawnEntity(EntityType etype, osg::Vec3 pos);
        void setPowerup(PowerupType);
        
     private:        
        std::list < osg::ref_ptr<Entity> > m_entityList;
        bool m_physicsActive;
        osg::MatrixTransform *m_sceneNode;
        EntityPaddle *m_paddle;
        int m_entitiesNum;
        int m_brickNumber;
        ContactListener *m_contactListener;
        b2MouseJoint *m_mouseJoint;
        b2Body* m_groundBody;
        
};
#endif

