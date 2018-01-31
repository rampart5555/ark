#ifndef __ENTITY_ANIMATION__
#define __ENTITY_ANIMATION__

#include <osg/AnimationPath>
#include "SceneDefs.h"
#include "Logging.h"
#include "EngineCallback.h"

class Entity;
typedef enum
{       
    RUNNING ,
    COMPLETE

} AnimationStatus;

typedef enum 
{
    ANIM_NONE,
    PADDLE_MOVE_FROM_SLOT,
    DOOR_OPEN,
    DOOR_CLOSE

}AnimType;

class EntityAnimation: public osg::AnimationPathCallback
{
    public:
        EntityAnimation();       
        void setEntity(Entity*);
        void createAnimation(osg::Vec3, osg::Vec3);
        void setCallback(EngineCallback);        
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
        void setAnimationType(AnimType atype) { m_animType = atype; }
        void createAnimation(Animation *anim);

    private:
        ~EntityAnimation()
        {
            LOG_INFO("EntiytAnimation destructor:%s\n","");
        }
        AnimationStatus m_status;
        EngineCallback m_callback;
        Entity *m_entity;
        AnimType m_animType;
};

#endif