#ifndef __ENTITY_ANIMATION__
#define __ENTITY_ANIMATION__

#include <osg/AnimationPath>
#include "Logging.h"
#include "EngineCallback.h"

typedef enum
{       
    RUNNING ,
    COMPLETE

} AnimationStatus;

class EntityAnimation: public osg::AnimationPathCallback
{
    public:
        EntityAnimation();       
        void createAnimation(osg::Vec3, osg::Vec3);
        void setCallback(EngineCallback);
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

    private:
        ~EntityAnimation()
        {
            LOG_INFO("EntiytAnimation destructor:%s\n","");
        }
        AnimationStatus m_status;
        EngineCallback m_callback;
};

#endif