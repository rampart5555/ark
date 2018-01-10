#ifndef __SCENE_ANIMATION__
#define __SCENE_ANIMATION__

#include <osg/AnimationPath>

typedef enum
{   
    NONE, 
    RUNNING ,
    FINISED 

} AnimationStatus;

class PaddleAnimationCallback: public osg::AnimationPathCallback
{
    public:
        PaddleAnimationCallback();
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};


class PaddleAnimation
{
    public:
        PaddleAnimation();
        PaddleAnimationCallback* getAnimation(const char *anim_name);
        void createSpawnAnim(osg::Vec3, osg::Vec3);
    private:
        
        PaddleAnimationCallback *m_animSpawnCb;
        AnimationStatus m_status;
        
                
};
#endif