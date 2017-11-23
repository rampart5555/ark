#ifndef __WIDGET__
#define __WIDGET__

#include <map>
#include <stdio.h>
#include <osgText/Text>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include "scene/SceneData.h"
#include "Config.h"
#include "EngineCallback.h"
#include "UIDefs.h"

class Widget : public osg::Referenced
{
    public:
        Widget();
        Widget(MenuItem);
        void setPosition(float, float, float);
        void setScale(float, float, float);
        void setLabel(const char*);
        void setNode(osg::MatrixTransform*);
        void setUniform(osg::Uniform*);
        osg::MatrixTransform* getNode();        
        float getWidth();
        float getHeight();
        void setCallback(EngineCallback cb);
        virtual void runCallback(void*);
    protected:        
        osg::MatrixTransform *m_transform;
        osg::MatrixTransform *m_geomNode;
        osg::Geode *m_textGeode;
        osgText::Text *m_label;
        
        float m_width;
        float m_height;
        
        EngineCallback m_callback;
};

class LevelSelectButton : public Widget
{
    public:
        LevelSelectButton();
        virtual void runCallback(void*);
        void setSceneData(int, int); 
    private:
        int m_epId;
        int m_lvlId;
        
};

#endif


