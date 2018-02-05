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
        void setCallback(EngineCallback);        
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);      
        void addTranslate(float, osg::Vec3);
        void addTranslate(float, float, float, float);
        void setEventName(std::string);
    private:
        ~EntityAnimation()
        {
            LOG_INFO("EntiytAnimation destructor:%s\n","");
        }
        AnimationStatus m_status;
        EngineCallback m_callback;
        Entity *m_entity;
        AnimType m_animType;
        std::string m_eventName;
};

#endif