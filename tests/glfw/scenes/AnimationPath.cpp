
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

osg::PositionAttitudeTransform* createCube_1(Scene* scene)
{
     /* animated cube */
    osg::Geode *geode_cube = new osg::Geode;
    geode_cube->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),1.0)));
    osg::PositionAttitudeTransform *cube_transform = new osg::PositionAttitudeTransform();
    cube_transform->addChild(geode_cube);   
    cube_transform->setName("entity_cube");
    osg::Program *prog = scene->loadProgram("shaders/default_color.vert","shaders/default_color.frag");
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
                    {
                        //printf("animation complete:%s\n",node->getName().c_str());
                        osg::PositionAttitudeTransform *tr=dynamic_cast<osg::PositionAttitudeTransform*>(node);
                        //if(node!=NULL)
                        //    printf("%f %f\n",tr->getPosition().x(),tr->getPosition().y());
                            
                    }
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
    m_cubeEntity_1 = createCube_1(this);
    m_cubeEntity_1->setPosition(osg::Vec3(-5.0, 5.0,0.0));
    m_cubeEntity_2 = createCube_1(this);
    m_cubeEntity_2->setPosition(osg::Vec3( 5.0, 5.0,0.0));
    
    m_rootNode->addChild(m_cubeEntity_1);
    m_rootNode->addChild(m_cubeEntity_2);
}

void TestAnimationPath::handleSceneEvent()
{   
    printf("Play animation\n");
    m_cubeEntity_1->setPosition(osg::Vec3(-5.0, 5.0,0.0));
    m_cubeEntity_2->setPosition(osg::Vec3( 5.0, 5.0,0.0));
    
    osg::AnimationPathCallback *animaPathCb;
    osg::AnimationPath *animPath;
    //1
    animPath = createAnimationPath("door_left_open", 0.0, m_cubeEntity_1->getPosition(), osg::Vec3(5.0,-5.0,0.0));
    animaPathCb = new TestAnimationPathCallback();
    animaPathCb->setAnimationPath(animPath);
    m_cubeEntity_1->setUpdateCallback(animaPathCb);
    //2
    animPath = createAnimationPath("door_right_open", 7.0, m_cubeEntity_2->getPosition(), osg::Vec3(-5.0,-5.0,0.0));
    animaPathCb = new TestAnimationPathCallback();
    animaPathCb->setAnimationPath(animPath);
    m_cubeEntity_2->setUpdateCallback(animaPathCb);
    //createAnimationPath();
}

osg::AnimationPath* TestAnimationPath::createAnimationPath(
        std::string anim_name, float start_frame, osg::Vec3 start_pos, osg::Vec3 end_pos )
{
    osg::AnimationPath *animPath = NULL;
    if(anim_name=="door_left_open")
    {
        animPath = new osg::AnimationPath;
        animPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
        animPath->insert(0.0, osg::AnimationPath::ControlPoint(start_pos));
        if(start_frame!=0.0)
            animPath->insert(start_frame, osg::AnimationPath::ControlPoint(start_pos));
        animPath->insert(start_frame + 3.0,osg::AnimationPath::ControlPoint(end_pos));        
    }
    if(anim_name=="door_right_open")
    {
        animPath = new osg::AnimationPath;
        animPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
        animPath->insert(0.0, osg::AnimationPath::ControlPoint(start_pos));
        if(start_frame!=0.0)
            animPath->insert(start_frame, osg::AnimationPath::ControlPoint(start_pos));
        animPath->insert(start_frame + 3.0,osg::AnimationPath::ControlPoint(end_pos));        
    }
    return animPath;
}

void TestAnimationPath::createAnimationPath()
{
    osg::AnimationPath *m_animPath = new osg::AnimationPath;
    m_animPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
    m_animPath->insert(0.0,osg::AnimationPath::ControlPoint(osg::Vec3(-2.0,0.0,0.0)));
    m_animPath->insert(1.0,osg::AnimationPath::ControlPoint(osg::Vec3(0.0,0.0,1.0)));
    m_animPath->insert(2.0,osg::AnimationPath::ControlPoint(osg::Vec3(3.0,-5.0,1.0)));
    m_animPath->insert(3.0,osg::AnimationPath::ControlPoint(osg::Vec3(3.0,-5.0,0.0)));    
    osg::AnimationPathCallback *m_animaPathCb = new TestAnimationPathCallback();
    m_animaPathCb->setAnimationPath(m_animPath);
    m_cubeEntity_1->setUpdateCallback(m_animaPathCb);
    
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

