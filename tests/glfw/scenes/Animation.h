#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Channel>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedRotateAxisElement>

#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osgDB/ReadFile>
#include "Scene.h" 

class AnimationScene: public Scene
{
    public:
        AnimationScene();
        virtual ~AnimationScene();
        virtual void createScene(); 
        virtual void handleSceneEvent();  
        
    private:
        osgAnimation::BasicAnimationManager* m_animMgr;
        osgAnimation::Animation* m_animTranslate;
        void createAnimationPath(const osg::Vec3& ,float, double );
        void animTranslate(const osg::Vec3& center, float radius, double looptime);

};

