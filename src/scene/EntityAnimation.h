#ifndef __ENTITY_ANIMATION__
#define __ENTITY_ANIMATION__

#include <osg/AnimationPath>
#include "Logging.h"
typedef enum
{   
    NONE, 
    RUNNING ,
    FINISED 

} AnimationStatus;

class EntityAnimation: public osg::AnimationPathCallback
{
    public:
        EntityAnimation();       
        void createAnimation(osg::Vec3, osg::Vec3);
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
    private:
        ~EntityAnimation()
        {
            LOG_INFO("EntiytAnimation destructor:%s\n","");
        }
};

#endif