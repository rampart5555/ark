#include <osgText/Text>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Node>
#include "Callbacks.h"
class Widget : public osg::Referenced
{
    public:
        Widget();
        void setPosition(float, float, float);        
        void setLabel(const char*);
        osg::MatrixTransform* getNode() {return m_node;}
        void setCallback(EngineCallback);
        bool runCallback();
    private:        
        osg::MatrixTransform *m_node;
        osg::MatrixTransform *m_geom;
        osgText::Text *m_label;
        osg::Program* createProgram();
        EngineCallback m_callback;
        
        
};
