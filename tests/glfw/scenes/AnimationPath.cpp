
#include "OsgMain.h"
#include "AnimationPath.h"
#include <osgDB/WriteFile>

osg::MatrixTransform* createPlane_1(Scene* scene)
{
    osg::Geode *geode_plane = new osg::Geode;
    osg::ShapeDrawable *dr = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f,0.0f, -0.01f), 10.0, 10.0, 0.02));
    geode_plane->addDrawable(dr);    
    osg::MatrixTransform *plane_transform = new osg::MatrixTransform();
    plane_transform->addChild(geode_plane);   
    
    osg::Program *prog = scene->loadProgram("shaders/default_color.vert","shaders/default_color.frag");
    osg::StateSet *ss = plane_transform->getOrCreateStateSet();
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
    
    return plane_transform;
}

osg::MatrixTransform* createCube_1(Scene* scene)
{
     /* animated cube */
    osg::Geode *geode_cube = new osg::Geode;
    geode_cube->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(-4.5f,4.5f,0.5f),1.0)));
    osg::MatrixTransform *cube_transform = new osg::MatrixTransform();
    cube_transform->addChild(geode_cube);   
    osg::Program *prog = scene->loadProgram("shaders/screen.vert","shaders/screen.frag");
    osg::StateSet *ss = cube_transform->getOrCreateStateSet();
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
    return cube_transform;
}

class TestAnimationPathCallback: public osg::AnimationPathCallback
{
    public:
        TestAnimationPathCallback():AnimationPathCallback()
        {
        }
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
            if (_animationPath.valid() &&
                nv->getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR &&
                nv->getFrameStamp())
            {
                double time = nv->getFrameStamp()->getSimulationTime();
                _latestTime = time;

                if (!_pause)
                {
                    // Only update _firstTime the first time, when its value is still DBL_MAX
                    if (_firstTime==DBL_MAX) _firstTime = time;
                    update(*node);
                    if( getAnimationTime() > _animationPath->getLastTime() )
                       printf("animation complete\n");
                }
            }

            // must call any nested node callbacks and continue subgraph traversal.
            osg::NodeCallback::traverse(node,nv);
        }       
};

TestAnimationPath::TestAnimationPath():Scene()
{
    printf("Simple::m_rootNode:%p\n", m_rootNode);
}

TestAnimationPath::~TestAnimationPath()
{
    printf("Simple Destructor\n");
}


void TestAnimationPath::createScene()
{   
    m_rootNode->addChild( createPlane_1(this));
    m_cubeEntity = createCube_1(this);
    m_rootNode->addChild( m_cubeEntity);
}

void TestAnimationPath::handleSceneEvent()
{   
    printf("play Animation\n");
    createAnimationPath();
}

void TestAnimationPath::createAnimationPath()
{
    osg::AnimationPath *m_animPath = new osg::AnimationPath;
    m_animPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
    m_animPath->insert(0.0,osg::AnimationPath::ControlPoint(osg::Vec3(0.0,0.0,0.0)));
    m_animPath->insert(1.0,osg::AnimationPath::ControlPoint(osg::Vec3(0.0,0.0,1.0)));
    m_animPath->insert(2.0,osg::AnimationPath::ControlPoint(osg::Vec3(3.0,-5.0,1.0)));
    m_animPath->insert(3.0,osg::AnimationPath::ControlPoint(osg::Vec3(3.0,-5.0,0.0)));    
    osg::AnimationPathCallback *m_animaPathCb = new TestAnimationPathCallback();
    m_animaPathCb->setAnimationPath(m_animPath);
    m_cubeEntity->setUpdateCallback(m_animaPathCb);
    
}
#if 0
void AnimationScene::animTranslate(const osg::Vec3& center, float radius, double looptime)
{
    m_rootNode->addChild( createPlane(this) );
    /* animated cube */
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(-4.5f,4.5f,0.5f),1.0)));
    
}
#endif

