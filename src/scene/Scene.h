#ifndef __SCENE__
#define __SCENE__

#include <vector>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include "EntityManager.h"
#include "Entity.h"
#include "SceneDefs.h"
#include "BasicScene.h"

typedef struct
{
    osg::Vec3 m_pos;    
    osg::ref_ptr<Entity> m_entity;
    
}EntitySlot;


class Scene: public BasicScene
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
        void addSparePaddle(osg::Vec3);
        void loadScene(const char*, const char*);
        bool loadLevel(const char*,const char*);
        osg::MatrixTransform *getSceneNode();
        void loadShaders();
        osg::ref_ptr <Entity> createEntity(EntityType etype);
        osg::ref_ptr <Entity> createEntity(std::string);
        bool addEntity(osg::ref_ptr <Entity>);
        bool removeEntity(osg::ref_ptr <Entity>);
        virtual void handleRemove(osg::ref_ptr <Entity>);
        void resetEntities();        
        osg::ref_ptr<Entity> getEntity(EntityType);
        void addEntityProps(Entity *ent);
        bool loadTMXMap( const char* );
        EntityManager& getEntityManager() { return m_entityMgr; }
        void update(float);
        void levelContinue();  
        unsigned int getSparePaddleNumber(){ return m_sparePaddles.size(); }       
        //animation
        void animationStart(std::string);    
        void animationEnd(std::string);            
        void animLevelNew(std::string);
        void animLevelContinue(std::string);
        void animLevelCleared(std::string);

    protected:
        osg::MatrixTransform *m_sceneNode;
        EntityManager m_entityMgr;
        int m_levelScore;
        EntityProps *m_entityProps;        
        std::list < osg::ref_ptr<Entity> > m_sparePaddles;
        bool m_sceneLoaded;
        std::list<osg::ref_ptr<Entity> >m_entityList;
};
#endif
