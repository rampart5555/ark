#include "Config.h"
#include "OsgMain.h"
#include "Widget.h"
#include "MouseHandler.h"
#include "scenes/Simple.h"
#include "scenes/Particle.h"
#include "scenes/Alpha.h"
#include "scenes/Userdata.h"
#include "scenes/Animation.h"
#include "scenes/AnimationPath.h"
//USE_OSGPLUGIN(osg2)
//USE_SERIALIZER_WRAPPER_LIBRARY(osg)
//USE_GRAPHICSWINDOW()


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
    m_currentScene = NULL;
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
    w->setCallback(show_statistics);
    camera->addChild(w->getNode());
    
    w = new Widget();
    w->setPosition(-1.5, -4.0, 0.0);
    w->setLabel("Anim_1");
    w->setCallback(play_animation);
    camera->addChild(w->getNode());
            
    return camera;
}

bool OsgMain::init(int x, int y, int width, int height)
{
    //in StatsHandler.cpp change gupu stats from true to false collectStats("gpu",false);
    osg::setNotifyLevel(osg::INFO);
    m_viewer = new osgViewer::Viewer;
    m_window = m_viewer->setUpViewerAsEmbeddedInWindow(x, y, width, height);
    m_window->getEventQueue()->windowResize(x, y, width, height);
    //m_viewer->setUpViewInWindow(x,y,width,height);
    
    m_root = new osg::Group();
    m_root->setNodeMask(MASK_2D);  
    m_root->addChild(createOrthoCamera(width,height));  
    
    initScene();
    //loadScene("Simple");
    //loadScene("Particle");
    //loadScene("Alpha");
    loadScene("TestAnimationPath");
    m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);  
    m_viewer->addEventHandler(new MouseHandler());  
    m_viewer->setSceneData(m_root.get());

    m_viewer->realize();
    
    return true;
}

void OsgMain::reshape(int width, int height)
{
    m_window->resized(m_window->getTraits()->x, m_window->getTraits()->y, width, height);
    m_window->getEventQueue()->windowResize(m_window->getTraits()->x, m_window->getTraits()->y, width, height);
}

void OsgMain::draw()
{
    if(m_viewer!=NULL)
        m_viewer->frame();
}

void OsgMain::run()
{
    if(m_viewer!=NULL)
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

void OsgMain::initScene()
{
    m_sceneNode = new osg::Group();    
    osg::Program *program = new osg::Program;
    program->setName( "shader" );
    osg::Shader *vert_s = new osg::Shader( osg::Shader::VERTEX, microshaderVertSource);
    osg::Shader *frag_s = new osg::Shader( osg::Shader::FRAGMENT, microshaderFragSource );
    vert_s->setName("vertex_shader");
    frag_s->setName("fragment_shader");
    program->addShader( vert_s );
    program->addShader( frag_s );
    osg::StateSet *ss = m_sceneNode->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON);
    m_sceneNode->setNodeMask(MASK_3D);
    m_root->addChild(m_sceneNode);

}

void OsgMain::loadScene(const char* scene_name)
{
    Scene *scene=NULL;
    
    if(strcmp(scene_name,"Simple")==0)
    {
        scene = new Simple();
        scene->createScene();
    }
    else if(strcmp(scene_name,"Particle")==0)
    {
        scene = new Particle();
        scene->createScene();

    }
    else if(strcmp(scene_name,"Alpha")==0)
    {
        scene = new Alpha();
        scene->createScene();

    }
    else if(strcmp(scene_name,"Userdata")==0)
    {
        scene = new Userdata();
        scene->createScene();
        //m_sceneNode->addChild(s->getSceneNode());
    }
    else if(strcmp(scene_name,"AnimationScene")==0)
    {
        scene = new AnimationScene();
        scene->createScene();        
    }
    else if(strcmp(scene_name,"TestAnimationPath")==0)
    {
        scene = new TestAnimationPath();
        scene->createScene();        
    }
    m_currentScene = scene;
    if(scene != NULL)
        m_sceneNode->addChild(scene->getSceneNode());
}







