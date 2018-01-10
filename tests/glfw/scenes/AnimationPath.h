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
        osg::MatrixTransform *m_cubeEntity;

};

