
#include "OsgMain.h"
#include "TimelineAnimation.h"
#include <osgDB/WriteFile>
#include <osgAnimation/Channel>

class FindAnimation : public osg::NodeVisitor
{
    public:
        osg::ref_ptr<osgAnimation::BasicAnimationManager> _am;    
        FindAnimation(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
                        
        }     
        void apply(osg::Node& node) 
        {
            if (_am.valid())
            {                
                return;
            }
            if (node.getUpdateCallback()) 
            {
                osgAnimation::BasicAnimationManager* b = dynamic_cast<osgAnimation::BasicAnimationManager*>(node.getUpdateCallback());
                if (b) 
                {
                    _am = new osgAnimation::BasicAnimationManager(*b);
                    printf("****************************found animation\n");

                    for (osgAnimation::AnimationList::const_iterator animIter = _am->getAnimationList().begin();
                    animIter != _am->getAnimationList().end(); ++animIter)
                    {
                        const osgAnimation::ChannelList& cl = (*animIter)->getChannels();
                        for (osgAnimation::ChannelList::const_iterator it = cl.begin(); it != cl.end(); ++it)
                        {
                            printf("chanel:%s\n", (*it)->getName().c_str());
                            osgAnimation::Sampler *sampler= (*it)->getSampler();
                            osgAnimation::KeyframeContainer *kfc = sampler->getKeyframeContainer();
                            printf("kfc size:%d\n",kfc->size());
                        }

                    }                    
                    return;

                }
            }
            traverse(node);
        }                       
};   

class FindModel_2 : public osg::NodeVisitor
{
    public:            
        FindModel_2(TimelineAnimation* tl): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
             m_timeline=tl;           
        } 
        void apply(osg::MatrixTransform& tr)
        {              
            //AssetsManager::instance().addModelAsset(tr);            
            //traverse(tr);
           m_timeline->addEntity(tr);
        }
        TimelineAnimation *m_timeline;
};

static osgAnimation::Animation* createAnimation(osg::MatrixTransform *ent, osg::Vec3 start_pos, osg::Vec3 end_pos)
{
    //update callback
    osgAnimation::UpdateMatrixTransform* updatecb = new osgAnimation::UpdateMatrixTransform(ent->getName());
    osgAnimation::StackedTranslateElement *trans = new osgAnimation::StackedTranslateElement("position");    
    trans->setTranslate(start_pos);  
    updatecb->getStackedTransforms().push_back(trans);    
    ent->setUpdateCallback(updatecb);
    
    //channel
    osgAnimation::Vec3LinearChannel* channel = new osgAnimation::Vec3LinearChannel;
    channel->setTargetName(ent->getName());
    channel->setName("position");
    if(ent->getName()=="entity_cube")
    {
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(0.0, start_pos ));
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(5.0, end_pos ));
    }
    else if(ent->getName()=="entity_sphere")
    {
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(0.0, start_pos ));
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(5.0, start_pos ));
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(8.0, end_pos ));
    }
    else if(ent->getName()=="entity_wall")
    {
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(0.0, start_pos ));
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(8.0, start_pos ));
        channel->getOrCreateSampler()->getOrCreateKeyframeContainer()->push_back(osgAnimation::Vec3Keyframe(10.0, end_pos ));
    }
    
    //animation
    osgAnimation::Animation *anim = new osgAnimation::Animation;
    anim->addChannel(channel);
    anim->setPlayMode(osgAnimation::Animation::ONCE);   

    return anim;

}

struct Animation
{
    const char* m_target;
    float m_frames[1][4];
};

struct Animation level_complete={"entity_sphere", {{0.0,0.0,0.0}}};


TimelineAnimation::TimelineAnimation():Scene()
{
    printf("Simple::m_rootNode:%p\n", m_rootNode);
}

TimelineAnimation::~TimelineAnimation()
{
    printf("Simple Destructor\n");
}

void TimelineAnimation::addEntity(osg::MatrixTransform &tr)
{
    osg::ref_ptr <osg::MatrixTransform> ent = new osg::MatrixTransform(tr);    
    m_entityMap.insert(std::make_pair(ent->getName(), ent));    
    printf("Entity added:%s\n", ent->getName().c_str());
       
}

void TimelineAnimation::createScene()
{   
    osg::ref_ptr<osg::Node> model = osgDB::readRefNodeFile("./models/timeline_models.osgt");
    FindModel_2 fm(this);
    //FindAnimation fa;
    if(model.valid())
    {
        model->accept(fm);
        //model->accept(fa);
    }
    else 
        printf("Invalid model\n");
    printf("BasicAnimationManager creaste scene\n");
    
    osg::Program *prog = loadProgram("shaders/default_color.vert","shaders/default_color.frag");
    osg::StateSet *ss = m_rootNode->getOrCreateStateSet();
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);    
        
    //add entities
    std::map<std::string,osg::ref_ptr<osg::MatrixTransform> >::iterator it;
    for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
        m_rootNode->addChild(it->second);
        
    //add animation manager    
    osgAnimation::BasicAnimationManager *animMgr = new osgAnimation::BasicAnimationManager();    

    m_rootNode->setUpdateCallback(animMgr);
    
    //add animations
    osgAnimation::Animation *anim_1 = createAnimation(m_entityMap["entity_sphere"].get(), osg::Vec3(-2.5, 2.5, 0.0), osg::Vec3(2.5,  -2.5, 0.0) );    
    osgAnimation::Animation *anim_2 = createAnimation(m_entityMap["entity_cube"].get(),   osg::Vec3( 2.5, 2.5, 0.0), osg::Vec3(-2.5, -2.5, 0.0) );
    osgAnimation::Animation *anim_3 = createAnimation(m_entityMap["entity_wall"].get(),   osg::Vec3( 0.0, -2.5, 0.0), osg::Vec3(0.0, 2.5, 0.0) );
    animMgr->registerAnimation(anim_1);
    animMgr->registerAnimation(anim_2);
    animMgr->registerAnimation(anim_3);

}

void TimelineAnimation::handleSceneEvent()
{   
    printf("TimelineAnimation::handleSceneEvent()\n");    
    osgAnimation::BasicAnimationManager* animMgr = dynamic_cast<osgAnimation::BasicAnimationManager*>(m_rootNode->getUpdateCallback());
    if(animMgr==NULL)
    {
        printf("Invalid update callback\n");
    }
    osgAnimation::AnimationList::const_iterator animIter;
    for (animIter = animMgr->getAnimationList().begin(); animIter != animMgr->getAnimationList().end(); ++animIter)
    {
       animMgr->playAnimation(*animIter);
    }               
}
