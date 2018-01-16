
#include "Scene.h" 

class TestRefptr: public Scene
{
    public:
        TestRefptr();
        virtual ~TestRefptr();
        virtual void createScene(); 
        virtual void handleSceneEvent();  
    private:
        osg::ref_ptr<osg::Node> m_node;             

};

