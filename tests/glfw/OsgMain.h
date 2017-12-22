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

#include "scenes/Scene.h"

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
        void loadScene(const char*);
        Scene* getCurrentScene() { return m_currentScene; }
        osg::Group* getSceneNode() { return m_sceneNode; }
    private:
        osgViewer::Viewer *m_viewer;
        osg::observer_ptr<osgViewer::GraphicsWindow> m_window;    
        osg::ref_ptr<osg::Group> m_root;
        osg::ref_ptr<osg::Group> m_sceneNode;
        osg::Camera* createOrthoCamera(int,int);
        void initScene();
        Scene *m_currentScene;
};
#endif
