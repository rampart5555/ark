#include "SceneAnimation.h"
#if 0
void SceneAnimation::animTranslate(const osg::Vec3& center, float radius, double looptime)
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
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(
        osgAnimation::Vec3Keyframe(0, osg::Vec3(0,0,0)));
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(
        osgAnimation::Vec3Keyframe(2, osg::Vec3(0,0,0.5)));
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(
        osgAnimation::Vec3Keyframe(4, osg::Vec3(5,-5,0.5)));
    channelAnimation1->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(
        osgAnimation::Vec3Keyframe(6, osg::Vec3(5,-5,0.0)));
    
    m_animTranslate = new osgAnimation::Animation;
    m_animTranslate->addChannel(channelAnimation1);
    m_animTranslate->setPlayMode(osgAnimation::Animation::ONCE);   
    
    m_animMgr = new osgAnimation::BasicAnimationManager();    
    m_animMgr->registerAnimation(m_animTranslate);    
    m_rootNode->setUpdateCallback(m_animMgr);
    //m_animMgr->playAnimation(m_animTranslate);
}
#endif

SceneAnimation::SceneAnimation()
{
    m_animMgr = new osgAnimation::BasicAnimationManager();
    m_animNode = new osg::Group();  
    m_animNode->setUpdateCallback(m_animMgr);
}

void SceneAnimation::createPaddleAnimation()
{    
    osgAnimation::Vec3LinearChannel* channel = new osgAnimation::Vec3LinearChannel;    
    osgAnimation::Vec3KeyframeContainer* keyframe = channel->getOrCreateSampler()->getOrCreateKeyframeContainer();
    keyframe->push_back(osgAnimation::Vec3Keyframe(0, osg::Vec3(0,0,0)));
    keyframe->push_back(osgAnimation::Vec3Keyframe(1, osg::Vec3(0,0,0.5)));
    keyframe->push_back(osgAnimation::Vec3Keyframe(2, osg::Vec3(0.0,-1.0,0.5)));
    keyframe->push_back(osgAnimation::Vec3Keyframe(3, osg::Vec3(0.0,-1.0,0.0)));
    channel->setTargetName("AnimatedCallback");
    channel->setName("position");
    m_animPaddle = new osgAnimation::Animation;
    m_animPaddle->addChannel(channel);
    m_animPaddle->setPlayMode(osgAnimation::Animation::PPONG); 
    m_animMgr->registerAnimation(m_animPaddle);  
    
#if 0    
    osg::MatrixTransform *trans = new osg::MatrixTransform();
    trans->setName("AnimatedNode");    
    trans->setDataVariance(osg::Object::DYNAMIC);
    trans->setMatrix(osg::Matrix::identity());
    trans->addChild(node);
    trans->setUpdateCallback(updatecb);
    m_animNode->addChild(trans);    
#endif    
}

void SceneAnimation::playPaddleAnimation(osg::MatrixTransform *node)
{
    osgAnimation::UpdateMatrixTransform* updatecb = new osgAnimation::UpdateMatrixTransform("AnimatedCallback");
    updatecb->getStackedTransforms().push_back(new osgAnimation::StackedTranslateElement("position"));
    osg::MatrixTransform *trans = new osg::MatrixTransform();
    trans->setName("AnimatedNode");    
    trans->setDataVariance(osg::Object::DYNAMIC);
    trans->setMatrix(osg::Matrix::identity());
    trans->addChild(node);
    trans->setUpdateCallback(updatecb);        
    m_animNode->addChild(trans);
    m_animMgr->playAnimation(m_animPaddle);
}