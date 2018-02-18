#ifndef __BASIC_SCENE__
#define __BASIC_SCENE__

#define SCENE 1
#define ENTITY_MANAGER 2
#include <osg/ref_ptr>
#include "Entity.h"
class BasicScene
{
    public:
        BasicScene() { m_sceneId = -1; }
        void setSceneId(int sid) { m_sceneId=sid; }
        int getSceneId() { return m_sceneId; }
        virtual void handleRemove(osg::ref_ptr<Entity>){}
    protected:
        int m_sceneId;
};
#endif