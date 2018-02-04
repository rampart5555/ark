#include <osg/AnimationPath>

#include "Scene.h" 

class TestAnimationPath: public Scene
{
    public:
        TestAnimationPath();
        virtual ~TestAnimationPath();
        virtual void createScene(); 
        virtual void handleSceneEvent();          
        
    private:
        void createAnimationPath();
        osg::AnimationPath* createAnimationPath(std::string, float, osg::Vec3, osg::Vec3);
        osg::PositionAttitudeTransform *m_cubeEntity_1;
        osg::PositionAttitudeTransform *m_cubeEntity_2;

};

