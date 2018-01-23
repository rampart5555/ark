#ifndef __SCENE__
#define __SCENE__

#include <vector>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include "EntityManager.h"
#include "Entity.h"
#include "SceneDefs.h"

typedef struct
{
    osg::Vec3 m_pos;    
    osg::ref_ptr<Entity> m_entity;
    
}EntitySlot;

class Scene
{
    public:
        Scene();
        ~Scene();
        enum EntityPos{
            FIRST,
            LAST
        };
        static Scene& instance()
        {
            static Scene instance;
            return instance;
        }
        void clear();
        void loadStaticScene(); // no physics for this scene
        void loadScene(const char*, const char*);
        bool loadLevel(const char*,const char*);
        osg::MatrixTransform *getSceneNode();
        void loadShaders();
        osg::ref_ptr <Entity> createEntity(EntityType etype);

        bool addEntity(osg::ref_ptr <Entity>);
        bool removeEntity(unsigned int);
        const EntitySlot* getEntitySlot(EntityPos, bool);

        void addEntityProps(Entity *ent);
        bool loadTMXMap( const char* );
        EntityManager& getEntityManager() { return m_entityMgr; }
        void update(float);
        void levelContinue();
        void playAnimation(Entity *ent, osg::Vec3 , EngineCallback , AnimType );

    protected:
        osg::MatrixTransform *m_sceneNode;
        EntityManager m_entityMgr;
        int m_levelScore;
        EntityProps *m_entityProps;        
        std::vector <EntitySlot> m_paddleSlots;
        bool m_sceneLoaded;
        osg::ref_ptr<Entity> m_doorLeft;
        osg::ref_ptr<Entity> m_doorRight;
};
#endif
