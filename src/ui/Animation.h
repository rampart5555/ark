#ifndef __ANIMATION__
#define __ANIMATION__

#include <osg/Node>
#include <osgAnimation/EaseMotion>
#include <osg/MatrixTransform>
#include "Config.h"
#include "EngineCallback.h"

class Menu;

typedef enum 
{
    STOP     = 0,
    START    = 1,
    COMPLETE = 2,
    RUNNING  = 3
}AnimStatus;

class AnimTranslate:public osg::NodeCallback
{
    public:
        AnimTranslate(float, float, float);
        ~AnimTranslate();
        void start(bool);        
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
        
    private:
        osgAnimation::Motion *m_motion;
        double m_passedTime;
        AnimStatus m_status;
        float m_duration;
        float m_startValue;
        float m_endValue;        
        bool  m_reverse;
};

class AnimFade: public osg::UniformCallback
{
    public:      
        AnimFade(Menu*, float,float,float); 
        ~AnimFade();
        void start(bool);
        virtual void operator() ( osg::Uniform* uniform, osg::NodeVisitor* nv );        
        void setCallback(EngineCallback, void*);        
        
    private:        
        osgAnimation::Motion *m_motion;
        double m_passedTime;       
        AnimStatus m_status;
        float m_startValue;
        float m_endValue;
        float m_reverse;
        float m_duration;  
        EngineCallback m_callback;
        void *m_callbackArgs;      
        Menu *m_menu;
};
#endif
