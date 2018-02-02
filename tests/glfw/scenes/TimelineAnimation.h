#include <map>
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Channel>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedRotateAxisElement>

#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osgDB/ReadFile>
#include "Scene.h" 

class TimelineAnimation: public Scene
{
    public:
        TimelineAnimation();
        virtual ~TimelineAnimation();
        virtual void createScene(); 
        virtual void handleSceneEvent();  
        void addEntity(osg::MatrixTransform&);
        
    private:
        std::map < std::string, osg::ref_ptr<osg::MatrixTransform> > m_entityMap ;
               
};

