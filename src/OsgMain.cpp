#include "Config.h"
#include "OsgMain.h"
#include "ui/Widget.h"
#include "MouseHandler.h"
#include "ui/MenuManager.h"
#include "scene/Scene.h"
#include "scene/LevelManager.h"
#include "scene/LevelManager2.h"
#include "EngineCallback.h"

#if 0 //use 1 for static build
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(freetype)
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
//USE_GRAPHICSWINDOW()
#endif

static const char *microshaderVertSource = 
{
    "// microshader - colors a fragment based on its position\n"
    "varying vec4 color;\n"
    "void main(void)\n"
    "{\n"
    "    color = gl_Vertex;\n"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};

static const char *microshaderFragSource = 
{
    //"precision mediump float;\n"
    "varying vec4 color;\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = clamp( color, 0.0, 1.0 );\n"
    "}\n"
};


OsgMain::OsgMain()
{
    m_viewer = NULL;    
}

OsgMain::~OsgMain()
{
    
}

OsgMain& OsgMain::instance()
{
    static OsgMain instance;
    return instance; 
}

void OsgMain::cleanup()
{
    
}

osg::Camera* OsgMain::createOrthoCamera(int width, int height)
{
    osg::Camera* camera = new osg::Camera();
    camera->getOrCreateStateSet()->setMode(
        GL_LIGHTING,
        osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
    );
    
    float ratio = (float)height/(float)width;
    float half_w = 4;
    float half_h = half_w * ratio;
    
    camera->setProjectionMatrixAsOrtho(-half_w, half_w, -half_h, half_h, -1.0, 1.0); 
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setNodeMask(MASK_2D);
    
    Widget *w = new Widget();
    w->setPosition(-3.0,-4.0,0.0);
    w->setLabel("Stats");
    //w->setCallback(show_statistics);
    camera->addChild(w->getNode());
    //m_root->addChild(w->getNode());
    
    return camera;
}

bool OsgMain::init(int x, int y, int width, int height)
{
    //in StatsHandler.cpp change gupu stats from true to false collectStats("gpu",false);
    //osg::setNotifyLevel(osg::INFO);
    m_viewer = new osgViewer::Viewer;
    m_window = m_viewer->setUpViewerAsEmbeddedInWindow(x, y, width, height);
    m_window->getEventQueue()->windowResize(x, y, width, height);
    //m_viewer->setUpViewInWindow(x,y,width,height);
    //AssetsManager::instance().setRootPath("../assets");
    //AssetsManager::instance().loadMenuEntries();

    AssetsManager::instance().loadAssets();    
    
    m_rootNode = new osg::Group();    
    m_rootNode->setNodeMask(MASK_2D); 
    
    /* Menu */ 
    m_menuNode = createOrthoCamera(width, height);
    m_menuNode->addChild(MenuManager::instance().getMenuNode());
    m_rootNode->addChild(m_menuNode);  
    
    /*Scene*/    
    m_sceneNode = new osg::Group();
    m_sceneNode->addChild(Scene::instance().getSceneNode());
    m_sceneNode->setNodeMask(MASK_3D);
    m_rootNode->addChild(m_sceneNode);
           
    //m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);  
    m_viewer->addEventHandler(new MouseHandler());  
    m_viewer->setSceneData(m_rootNode.get());
    
    const osg::BoundingSphere& bs = m_rootNode->getBound();  	
    osg::Camera *cam = m_viewer->getCamera();
	cam->setViewMatrixAsLookAt(osg::Vec3(0.0f, 2.0f, -7.0f)*bs.radius(), osg::Vec3(0.0,0.0,0.0),osg::Vec3(0.0f,1.0f,0.0f));	
	osg::DisplaySettings::instance()->setNumMultiSamples( 4 ); 
    m_viewer->realize();

    LevelManager2::instance().readSceneInfo();
    //LevelManager::instance().setCurrent(0,0); //set current episode 0 and current level 0
    // run this only to create levels.osgt for the first time
    //std::string osgtfile = LevelManager::instance().getOsgtFile();
    //build_scene_data(osgtfile.c_str());
    return true;
}

void OsgMain::reshape(int width, int height)
{
#if 0
    m_window->resized(m_window->getTraits()->x, m_window->getTraits()->y, width, height);
    m_window->getEventQueue()->windowResize(m_window->getTraits()->x, m_window->getTraits()->y, width, height);
    const osg::BoundingSphere& bs = m_rootNode->getBound();  	
    osg::Camera *cam = m_viewer->getCamera();
	cam->setViewMatrixAsLookAt(osg::Vec3(0.0f, 2.0f, -17.0f)*bs.radius(), osg::Vec3(0.0,0.0,0.0),osg::Vec3(0.0f,1.0f,0.0f));
#endif	
}

void OsgMain::draw()
{    
    if(m_viewer != NULL)
    {
        double timeOfLastMerge = m_viewer->getFrameStamp()->getReferenceTime();
        m_viewer->frame();
        double currentTime = m_viewer->getFrameStamp()->getReferenceTime();
        double delta_t = currentTime - timeOfLastMerge;
        Scene::instance().update(delta_t);
        EngineEventQueue::instance().processEvents();

    }
}

void OsgMain::run()
{
    if(m_viewer != NULL)
    {         
         while( !m_viewer->done() )
         {
            draw();                
         }
    }
}

void OsgMain::mouseButtonPress(float x, float y, unsigned int button)
{
    if(m_window.valid())
        m_window->getEventQueue()->mouseButtonPress( (int)x, (int)y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON );
}

void OsgMain::mouseButtonRelease(float x, float y, unsigned int button)
{
    if(m_window.valid())
        m_window->getEventQueue()->mouseButtonRelease( (int)x, (int)y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON );
}

void OsgMain::mouseMotion(float x, float y)
{
    if(m_window.valid())
        m_window->getEventQueue()->mouseMotion(x,y);     
}

void OsgMain::keyPress(int key)
{
    printf("keypressesd: %c\n",key);
    if(m_window.valid())
    {            
        m_window->getEventQueue()->keyPress( key );
        m_window->getEventQueue()->keyRelease( key );
    }
}

void OsgMain::addDemoScene()
{
#if 0
    osg::Geode* geode = new osg::Geode();
    osg::Drawable *dr;
    dr=new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),1.0f));
    dr->setName("Sphere");
    geode->addDrawable(dr);
    dr=new osg::ShapeDrawable(new osg::Cone(osg::Vec3(2.2f,0.0f,-0.4f),0.9f,1.8f));
    dr->setName("Cone");
    geode->addDrawable(dr);
    dr = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(4.4f,0.0f,0.0f),1.0f,1.4f));
    dr->setName("Cylinder");
    geode->addDrawable(dr);
    
    osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform();
    osg::Program *program = new osg::Program;
    program->setName( "shader" );
    osg::Shader *vert_s = new osg::Shader( osg::Shader::VERTEX, microshaderVertSource);
    osg::Shader *frag_s = new osg::Shader( osg::Shader::FRAGMENT, microshaderFragSource );
    vert_s->setName("vertex_shader");
    frag_s->setName("fragment_shader");
    program->addShader( vert_s );
    program->addShader( frag_s );
    osg::StateSet *ss = m_root->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON);
    xform->setNodeMask(MASK_3D);
    xform->addChild(geode);
    m_root->addChild(xform);
#endif
}

