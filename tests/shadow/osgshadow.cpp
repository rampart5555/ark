
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
#include <osgDB/WriteFile>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgShadow/ShadowTexture>
//#include "ArkShadowMap"
#include "ArkShadowTexture"


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
    osg::Shader *vert_shader = osg::Shader::readShaderFile(osg::Shader::VERTEX,  "default_color.vert");
    osg::Shader *frag_shader = osg::Shader::readShaderFile(osg::Shader::FRAGMENT,"default_color.frag");
    osg::Program *program=new osg::Program();
    program->addShader(vert_shader);
    program->addShader(frag_shader);
    osg::StateSet *ss = model->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON);
}

void setUpdateCallback(osg::MatrixTransform *model)
{
    osg::AnimationPath* animationPath = new osg::AnimationPath;
    animationPath->setLoopMode(osg::AnimationPath::SWING);
    animationPath->insert(0.0,osg::AnimationPath::ControlPoint(osg::Vec3(0.0,0.0,0.0)));
    animationPath->insert(12.0,osg::AnimationPath::ControlPoint(osg::Vec3(0.0,0.0,2.0)));
    osg::AnimationPathCallback *apc=new osg::AnimationPathCallback();
    apc->setAnimationPath(animationPath);
    model->setUpdateCallback(apc);
}

osgShadow::ShadowedScene* createShadowScene(osg::Group *scene_models, bool gles2_enabled)
{
    if(scene_models==NULL)
    {
        printf("Scene node is null\n");
        return NULL;
    }
    
    ArkShadowTexture *shadowMap = new ArkShadowTexture;
    
    osgShadow::ShadowedScene *shadowScene = new osgShadow::ShadowedScene;
    osgShadow::ShadowSettings* settings = shadowScene->getShadowSettings();
    settings->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
    settings->setCastsShadowTraversalMask(CastsShadowTraversalMask);    
    shadowScene->setShadowTechnique(shadowMap);  
    
    
    for(unsigned int i=0;i<scene_models->getNumChildren();i++)
    {
        osg::MatrixTransform *model = dynamic_cast<osg::MatrixTransform*> (scene_models->getChild(i));
        printf("Adding model: %s\n", model->getName().c_str());
        if(model->getName()=="entity_background")
        {
            model->setNodeMask(ReceivesShadowTraversalMask);            
            //setShader(model);
        }
        
        else if(model->getName()=="Sphere")
        {
            setShader(model);
            setUpdateCallback(model);            
            model->setNodeMask(CastsShadowTraversalMask);
        }
        else
        {
            setShader(model);
            model->setNodeMask(CastsShadowTraversalMask);
            
        }            
        shadowScene->addChild(model);               
    }
    return shadowScene;    
}


int main()
{
    //osg::setNotifyLevel(osg::INFO);
    osgViewer::Viewer *m_viewer = new osgViewer::Viewer;
    //m_window = m_viewer->setUpViewerAsEmbeddedInWindow(x, y, width, height);
    //m_window->getEventQueue()->windowResize(x, y, width, height);
    m_viewer->setUpViewInWindow(0,0,600,800);
    
    
    m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);  

    bool gles2_enabled=false;  
    osg::ref_ptr<osg::Node> scene_models;          
#ifdef GLES_2
    gles2_enabled=true;
#endif
    if(gles2_enabled==false)
        scene_models=osgDB::readRefNodeFile("scene_light.osgt");
    else
        scene_models=osgDB::readRefNodeFile("scene.osgt");
    
    osg::Group  *root = new osg::Group();
    
#if 1   
    //shadown
    osgShadow::ShadowedScene *shadowScene = createShadowScene(scene_models->asGroup(), gles2_enabled);    
    root->addChild(shadowScene);
#else    
    //no shadow
    osg::Group *scene = createScene(scene_models->asGroup());
    root->addChild(scene);
#endif            
    m_viewer->setSceneData(root);
    

    int i=0;
    m_viewer->realize();
    while (!m_viewer->done())
    {
        m_viewer->frame();
        if(i++ == 50)
        {
            printf("Writing debug scene\n");
            osgDB::writeNodeFile(*shadowScene, "scene_debug.osgt");
        }
    }
    
    return 0;
}    
