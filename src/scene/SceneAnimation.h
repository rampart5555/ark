#ifndef __SCENE_ANIMATION__
#define __SCENE_ANIMATION__

#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Channel>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedRotateAxisElement>
#include <osgAnimation/Keyframe>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osgDB/ReadFile>


class SceneAnimation
{
    public:
        SceneAnimation();        
        void createPaddleAnimation();
        void playPaddleAnimation(osg::MatrixTransform *);
        osg::Group* getAnimNode(){ return m_animNode; }
        osgAnimation::BasicAnimationManager* getAnimMgr(){ return m_animMgr; }
    private:
        osgAnimation::BasicAnimationManager *m_animMgr;
        osgAnimation::Animation *m_animPaddle;
        osg::Group *m_animNode;

};
#endif