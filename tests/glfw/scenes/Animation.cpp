
#include "OsgMain.h"
#include "Animation.h"
#include <osgDB/WriteFile>

osg::MatrixTransform* createPlane(Scene *s)
{
    osg::Geode *geode_plane = new osg::Geode;
    osg::ShapeDrawable *dr = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f,0.0f, -0.01f), 10.0, 10.0, 0.02));
    geode_plane->addDrawable(dr);    
    osg::MatrixTransform *plane_transform = new osg::MatrixTransform();
    plane_transform->addChild(geode_plane);   
    
    osg::Program *prog = s->loadProgram("shaders/default_color.vert","shaders/default_color.frag");
    osg::StateSet *ss = plane_transform->getOrCreateStateSet();
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
    
    return plane_transform;
}

AnimationScene::AnimationScene():Scene()
{
    printf("Simple::m_rootNode:%p\n", m_rootNode);
}

AnimationScene::~AnimationScene()
{
    printf("Simple Destructor\n");
}


void AnimationScene::createScene()
{   
    printf("AnimationScene creaste scene\n");
    
    osg::Program *prog = loadProgram("shaders/screen.vert","shaders/screen.frag");
    osg::StateSet *ss = m_rootNode->getOrCreateStateSet();
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
    
    osg::Vec3 center(-0.5,-0.5,0.0);
    float radius=1.0;
    double looptime = 2.0;
    animTranslate(center,radius,looptime);
}

void AnimationScene::handleSceneEvent()
{   
#if 0    
    osg::Group *main_node = OsgMain::instance().getSceneNode();
    osgDB::writeNodeFile(*main_node, "animation.osgt");
#endif    
    m_animMgr->playAnimation(m_animTranslate);

}

void AnimationScene::animTranslate(const osg::Vec3& center, float radius, double looptime)
{
    m_rootNode->addChild( createPlane(this) );
    /* animated cube */
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(-4.5f,4.5f,0.5f),1.0)));
    
    osgAnimation::UpdateMatrixTransform* updatecb = new osgAnimation::UpdateMatrixTransform("AnimatedCallback");
    updatecb->getStackedTransforms().push_back(new osgAnimation::StackedTranslateElement("position"));
    
    osg::MatrixTransform *trans = new osg::MatrixTransform();
    trans->setName("AnimatedNode");    
    trans->setDataVariance(osg::Object::DYNAMIC);
    trans->setMatrix(osg::Matrix::identity());
    trans->addChild(geode);
    trans->setUpdateCallback(updatecb);
    m_rootNode->addChild(trans);
    
    osgAnimation::Vec3LinearChannel* channelAnimation1 = new osgAnimation::Vec3LinearChannel;
    channelAnimation1->setTargetName("AnimatedCallback");
    channelAnimation1->setName("position");
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(0, osg::Vec3(0,0,0)));
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(2, osg::Vec3(0,0,0.5)));
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(4, osg::Vec3(5,-5,0.5)));
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(6, osg::Vec3(5,-5,0.0)));
    
    m_animTranslate = new osgAnimation::Animation;
    m_animTranslate->addChannel(channelAnimation1);
    m_animTranslate->setPlayMode(osgAnimation::Animation::ONCE);   
    
    m_animMgr = new osgAnimation::BasicAnimationManager();    
    m_animMgr->registerAnimation(m_animTranslate);    
    m_rootNode->setUpdateCallback(m_animMgr);
    //m_animMgr->playAnimation(m_animTranslate);
}


