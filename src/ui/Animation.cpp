#include "Animation.h"
#include <stdio.h>
#include "ui/Menu.h"
/* 
Example:
Animate from 5.0f to 12.0f in 4 seconds
AnimTranslate *at = new AnimTranslate(5.0,7.0,4.0)
Animate from 15.0f to 5.0f in 4 seconds
AnimTranslate *at = new AnimTranslate(15.0, -10.0, 4.0)
Animate from -15.0f to -25.0f in 4 seconds
AnimTranslate *at = new AnimTranslate(-15.0, -10.0, 4.0)
Animate from -15.0f to 5.0f in 4 seconds
AnimTranslate *at = new AnimTranslate(-15.0, 20.0, 4.0)
*/

AnimTranslate::AnimTranslate(float start, float end, float duration)
{
    m_startValue = start;
    m_endValue = end;    
    m_duration = duration;
    m_passedTime = 0.0;
    m_status = STOP;  
    //m_motion = new osgAnimation::OutCubicMotion(start_value, m_duration, end_value);
    //m_motion = new osgAnimation::OutElasticMotion(m_startValue, m_duration, m_endValue);
    //m_motion = new osgAnimation::OutSineMotion(start_value, m_duration, end_value);
    m_motion = new osgAnimation::OutBackMotion(m_startValue, m_duration, m_endValue);
    m_reverse = false;
    printf(" AnimTranslate %f <->%f\n", m_startValue,m_endValue);
}

AnimTranslate::~AnimTranslate()
{
    printf("AnimNode::~AnimNode()\n");
}

void AnimTranslate::start(bool rev)
{
    m_reverse = rev;
    m_status  = START;
    m_motion->reset();
}
        
void AnimTranslate::operator()(osg::Node* node, osg::NodeVisitor* nv)
{       
 
    if((m_status==COMPLETE) || (m_status==STOP))
    {     
        ;            
    }    
    else if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR)
    {

        double dt;
        double value;
        
        const osg::FrameStamp* fs = nv->getFrameStamp();
        if(m_status==START)
        {
            dt = 0.0;
            m_status = RUNNING;
        }
        else
            dt = fs->getSimulationTime() - m_passedTime;

        m_passedTime = fs->getSimulationTime();            
        m_motion->update(dt);
        value = m_motion->getValue();        
        
        osg::MatrixTransform *tr = dynamic_cast<osg::MatrixTransform*>(node);
        osg::Matrix m;
        m.makeTranslate(value, 0.0, 0.0);        
        if(tr!=NULL)
            tr->setMatrix(m);
        else
            printf("Invalid transfrom\n");
        
        if(m_motion->getTime() >= m_duration)
        {
            m_status = COMPLETE;
            printf("Animation complete:\n");                        
        }
    }
    /* Continue traversing so that OSG can process any other nodes with callback */
    traverse(node,nv);
}

/* animation fade */
AnimFade::AnimFade(Menu *menu, float start, float end, float duration)
{
    m_startValue = start;
    m_endValue = end;    
    m_duration = duration;
    m_passedTime = 0.0;
    m_status = STOP;  
    m_motion = new osgAnimation::LinearMotion(m_startValue, m_duration, m_endValue);
    m_reverse = false;
    printf(" AnimFade %f <->%f\n", m_startValue,m_endValue);    
    m_menu = menu;
    m_callback = NULL;
    m_callbackArgs = NULL;
}

AnimFade::~AnimFade()
{
    printf("AnimFade::~AnimFade()\n");
}

void AnimFade::start(bool rev)
{
    m_reverse = rev;
    m_status  = START;
    m_motion->reset();
}

void AnimFade::operator() ( osg::Uniform* uniform, osg::NodeVisitor* nv )
{
    if((m_status==COMPLETE) || (m_status==STOP))
    {
        ;
    }
    else 
    {               
        float dt;
        float value;

        const osg::FrameStamp* fs = nv->getFrameStamp();
        if(m_status==START)
        {
            dt = 0.0;
            m_status = RUNNING;
        }
        else
        {
            dt = fs->getSimulationTime() - m_passedTime;
        }
        m_passedTime = fs->getSimulationTime();            
        m_motion->update(dt);    
        value = m_motion->getValue();          
        if(m_motion->getTime() >= m_duration)
        {
            m_status = COMPLETE;
            if(m_callback != NULL)
                m_callback(m_callbackArgs);
            if(m_menu != NULL)
                m_menu->animationComplete();
            printf("Animation complete\n");            
            
        }
        if(value<0.0) 
            value = 0.0;          
        uniform->set( value );
    }
    /* Continue traversing so that OSG can process any other nodes with callback */
    traverse(uniform,nv);                            
}

void AnimFade::setCallback(EngineCallback cb, void* args)
{
    m_callback = cb;
    m_callbackArgs = args;
}
