#include <stdio.h>
#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Vec3>
#include "Entity.h"
#include "EntityAnimation.h"


EntityAnimation::EntityAnimation() : osg::AnimationPathCallback()
{
    m_status = RUNNING;
    m_callback = NULL;
    m_entity = NULL;
    m_animType = ANIM_NONE;
}

void EntityAnimation::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    if (_animationPath.valid() &&
        nv->getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR &&
        nv->getFrameStamp() && (m_status==RUNNING) ) 
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
                m_status = COMPLETE;
                if(m_callback != NULL)
                    m_callback(NULL);
                if(m_entity != NULL)
                    m_entity->setValid(false);
            }
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    osg::NodeCallback::traverse(node,nv);
}       

void EntityAnimation::createAnimation(Animation *anim)
{
    if(anim==NULL)
    {
        LOG_WARN("EntityAnimation::playAnimation: null animation:%s\n","");
        return;
    }
    
    if(anim->m_frames.size() % 4 != 0)
    {
        LOG_WARN("EntityAnimation::playAnimation: animation invalid number of frames:%s\n","");
        return;
    }
    _animationPath = new osg::AnimationPath;
    _animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);    
    for(unsigned int i=0;i<anim->m_frames.size();i+=4)
    {
        osg::Vec3 pos;
        osg::AnimationPath::ControlPoint cp;
        pos.set(anim->m_frames[i+1], anim->m_frames[i+2], anim->m_frames[i+3]);
        cp.setPosition(pos);
        _animationPath->insert(anim->m_frames[i],cp);
    }    
}

void EntityAnimation::setEntity(Entity *ent)
{
    m_entity = ent;
}


void EntityAnimation::createAnimation(osg::Vec3 start_pos, osg::Vec3 end_pos)
{
    switch(m_animType)
    {
        case PADDLE_MOVE_FROM_SLOT:
            {
                _animationPath = new osg::AnimationPath;
                _animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);    
                _animationPath->insert(0.0, osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z())));
                _animationPath->insert(1.0, osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z()+1.0)));
                _animationPath->insert(2.0, osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), start_pos.z()+1.0)));
                _animationPath->insert(3.0, osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), end_pos.z())));        
            }
            break;
        case DOOR_CLOSE:
        case DOOR_OPEN:
            {                                
                _animationPath = new osg::AnimationPath;
                _animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
                _animationPath->insert(0.0, osg::AnimationPath::ControlPoint(osg::Vec3(start_pos.x(), start_pos.y(), start_pos.z())));
                _animationPath->insert(3.0, osg::AnimationPath::ControlPoint(osg::Vec3(end_pos.x(), end_pos.y(), end_pos.z())));                            
            } 
            break;
        default:
            break;
    }
}

void EntityAnimation::setCallback(EngineCallback cb)
{
    m_callback=cb;
}
