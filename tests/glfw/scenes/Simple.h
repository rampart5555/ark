
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osgDB/ReadFile>
#include "Scene.h" 

class Simple: public Scene
{
    public:
        Simple();
        virtual ~Simple();
        virtual void createScene();                
    private:        
        osg::ref_ptr<osg::Geode> createShape(int type);
};

