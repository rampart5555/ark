#include <stdio.h>
#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Vec3>
#include "EntityAnimation.h"

EntityAnimation::EntityAnimation() : osg::AnimationPathCallback()
{
}

void EntityAnimation::operator()(osg::Node* node, osg::NodeVisitor* nv)
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

void EntityAnimation::createAnimation(osg::Vec3 start_pos, osg::Vec3 end_pos)
{
    _animationPath = new osg::AnimationPath;
    _animationPath->setLoopMode(osg::AnimationPath::LOOP);    
    _animationPath->insert(0.0, osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z())));
    _animationPath->insert(1.0, osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z()+1.0)));
    _animationPath->insert(2.0, osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), start_pos.z()+1.0)));
    _animationPath->insert(3.0, osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), end_pos.z())));        
}