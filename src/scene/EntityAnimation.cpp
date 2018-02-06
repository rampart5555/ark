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
                if(m_eventName.empty()==false)
                {
                    EngineEvent *event=new EngineEvent;
                    event->m_eventId=LEVEL_ANIMATION_COMPLETE;
                    event->m_eventName=m_eventName;
                    EngineEventQueue::instance().setEvent(event);
                }
                if(m_entity != NULL)
                    m_entity->setValid(false);
            }
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    osg::NodeCallback::traverse(node,nv);
}   
   
void EntityAnimation::setEventName(std::string event)
{
    m_eventName=event;
}

void EntityAnimation::addTranslate(float frame, osg::Vec3 pos)
{
    EntityAnimation::addTranslate(frame, pos.x(), pos.y(), pos.z());
}

void EntityAnimation::addTranslate(float frame, float x, float y, float z)
{
    if(_animationPath.valid()==false)
    {
        _animationPath = new osg::AnimationPath;  
        _animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
    }
    _animationPath->insert(frame, osg::AnimationPath::ControlPoint(osg::Vec3(x,y,z)));
}

void EntityAnimation::setEntity(Entity *ent)
{
    m_entity = ent;
}

void EntityAnimation::setCallback(EngineCallback cb)
{
    m_callback=cb;
}
