
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include <osg/Light>
#include <osg/LightSource>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <osgShadow/ShadowedScene>
#include "ArkShadowMap"




const int ReceivesShadowTraversalMask = 0x1;
const int CastsShadowTraversalMask = 0x2;


typedef enum
{
    BACKGROUND,
    CUBE,
    CYLINDER
    
}ModelType;

void setShader(osg::MatrixTransform *model)
{
    osg::Shader *vert_shader = osg::Shader::readShaderFile(osg::Shader::VERTEX,  "../assets/shaders/default_color.vert");
    osg::Shader *frag_shader = osg::Shader::readShaderFile(osg::Shader::FRAGMENT,"../assets/shaders/default_color.frag");
    osg::Program *program=new osg::Program();
    program->addShader(vert_shader);
    program->addShader(frag_shader);
    osg::StateSet *ss = model->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON);
}

osgShadow::ShadowedScene* createShadowScene(osg::Group *scene_models)
{
    if(scene_models==NULL)
    {
        printf("Scene node ios null\n");
        return NULL;
    }
    
    int mapres = 1024;
    ArkShadowMap *shadowMap = new ArkShadowMap;
    //shadowMap->setTextureSize(osg::Vec2s(mapres, mapres));
    
    osg::ref_ptr<osg::Light> light = new osg::Light();
    light->setPosition( osg::Vec4( -3.0f, -3.0f, 3.0f, 0.0f ) );
    light->setAmbient( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    osg::LightSource *ls=new osg::LightSource();
    ls->setLight(light);
    
    //shadowMap->setLight(ls);

    //osg::Shader *shadow_frag_1 = new osg::Shader(osg::Shader::FRAGMENT,fragmentShaderSource_noBaseTexture);
    //shadowMap->addShader(shadow_frag_1);
    //osg::Shader *shadow_frag =osg::Shader::readShaderFile(osg::Shader::FRAGMENT, "../assets/shaders/shadow.frag");
    //shadowMap->addShader(shadow_frag);
    
    osgShadow::ShadowedScene *shadowScene = new osgShadow::ShadowedScene;
    osgShadow::ShadowSettings* settings = shadowScene->getShadowSettings();
    settings->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
    settings->setCastsShadowTraversalMask(CastsShadowTraversalMask);    
    shadowScene->setShadowTechnique(shadowMap);  
    shadowScene->addChild(ls);    

    
    for(unsigned int i=0;i<scene_models->getNumChildren();i++)
    {
        osg::MatrixTransform *model = dynamic_cast<osg::MatrixTransform*> (scene_models->getChild(i));
        printf("model: %s\n", model->getName().c_str());
        if(model->getName()=="entity_light")
            continue;
        setShader(model);
        shadowScene->addChild(model);
    }
        
    return shadowScene;    
}


int main()
{
    osg::setNotifyLevel(osg::INFO);
    osgViewer::Viewer *m_viewer = new osgViewer::Viewer;
    //m_window = m_viewer->setUpViewerAsEmbeddedInWindow(x, y, width, height);
    //m_window->getEventQueue()->windowResize(x, y, width, height);
    m_viewer->setUpViewInWindow(0,0,600,800);
    
        
    m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);  
    
    osg::ref_ptr<osg::Node> scene_models=osgDB::readRefNodeFile("../assets/models/scene.osgt");
    
    osg::Group  *root = new osg::Group();
#if 1   
    //shadown
    osgShadow::ShadowedScene *shadowScene = createShadowScene(scene_models->asGroup());    
    root->addChild(shadowScene);
#else    
    //no shadow
    osg::Group *scene = createScene(scene_models->asGroup());
    root->addChild(scene);
#endif            
    m_viewer->setSceneData(root);
    
#if 0    
    osgViewer::Viewer::Windows windows;    
    m_viewer->getWindows(windows);        

    osgShadow::ShadowMap* sm = dynamic_cast<osgShadow::ShadowMap*>(shadowScene->getShadowTechnique());
    if( sm ) 
    {
        osg::ref_ptr<osg::Camera> hudCamera = sm->makeDebugHUD();

        // set up cameras to rendering on the first window available.
        hudCamera->setGraphicsContext(windows[0]);
        hudCamera->setViewport(0,0,windows[0]->getTraits()->width, windows[0]->getTraits()->height);

        m_viewer->addSlave(hudCamera.get(), false);
    }
#endif     

    m_viewer->realize();
    while (!m_viewer->done())
    {
        m_viewer->frame();
    }
    
    return 0;
}    
