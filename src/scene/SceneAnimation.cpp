#include <stdio.h>
#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Vec3>
#include "SceneAnimation.h"

PaddleAnimationCallback::PaddleAnimationCallback() : osg::AnimationPathCallback()
{
}

void PaddleAnimationCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    if (_animationPath.valid() &&
        nv->getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR &&
        nv->getFrameStamp())
    {
        double time = nv->getFrameStamp()->getSimulationTime();
        _latestTime = time;

        if (!_pause)
        {
            // Only update _firstTime the first time, when its value is still DBL_MAX
            if (_firstTime==DBL_MAX) _firstTime = time;
            update(*node);
            if( getAnimationTime() > _animationPath->getLastTime() )
            {
                //printf("animation complete\n");
            }
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    osg::NodeCallback::traverse(node,nv);
}       

PaddleAnimation::PaddleAnimation()
{
    m_animSpawnCb = NULL;
}

PaddleAnimationCallback* PaddleAnimation::getAnimation(const char *anim_name)
{
    return m_animSpawnCb;
    //return m_animSpawn;
}

void PaddleAnimation::createSpawnAnim(osg::Vec3 start_pos, osg::Vec3 end_pos)
{
    osg::AnimationPath *animPath = new osg::AnimationPath;
    animPath->setLoopMode(osg::AnimationPath::LOOP);    
    animPath->insert(0.0,osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z())));
    animPath->insert(1.0,osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z()+1.0)));
    animPath->insert(2.0,osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), start_pos.z()+1.0)));
    animPath->insert(3.0,osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), start_pos.z()+1.0)));    
    m_animSpawnCb = new PaddleAnimationCallback();
    m_animSpawnCb->setAnimationPath(animPath);    
}