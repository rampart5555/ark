#ifndef __SCENE__
#define __SCENE__

#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include "EntityManager.h"
#include "Entity.h"
#include "SceneDefs.h"

class Scene
{
    public:
        Scene();
        ~Scene();
        static Scene& instance()
        {
            static Scene instance;
            return instance;
        }
        void clear();
        void loadScene(const char*);
        bool loadLevel(const char*);
        osg::MatrixTransform *getSceneNode();
        void loadShaders();
        osg::ref_ptr <Entity> createEntity(EntityType etype);
        void addEntityProps(Entity *ent);
        bool loadTMXMap( const char* );
        EntityManager& getEntityManager() { return m_entityMgr; }
        void updateLevelScore(int);
        int getLevelScore();
        //bool handleEvent(EngineEvent, void*);
    protected:
        osg::MatrixTransform *m_sceneNode;
        EntityManager m_entityMgr;
        int m_levelScore;
        EntityProps *m_entityProps;
};
#endif