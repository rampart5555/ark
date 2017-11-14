#ifndef __OSG_MAIN__
#define __OSG_MAIN__

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Node>

class OsgMain
{
    public:
        OsgMain();
        ~OsgMain();
        static OsgMain& instance();
        bool init(int, int, int,int );
        void cleanup();
        void draw();
        void run();
        void reshape(int, int);
        void mouseButtonPress(float x, float y, unsigned int button);
        void mouseButtonRelease(float x, float y, unsigned int button);
        void mouseMotion(float x, float y);        
        void keyPress(int key);        
    private:
        osgViewer::Viewer *m_viewer;
        osg::observer_ptr<osgViewer::GraphicsWindow> m_window;    
        osg::ref_ptr<osg::Group> m_rootNode;
        osg::ref_ptr<osg::Group> m_sceneNode;
        osg::ref_ptr<osg::Group> m_menuNode;
        osg::Camera* createOrthoCamera(int,int);
        void addDemoScene();        
};
#endif
