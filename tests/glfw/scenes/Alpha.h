
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osgDB/ReadFile>
#include "Scene.h" 

class Alpha: public Scene
{
    public:
        Alpha();
        virtual ~Alpha();
        virtual void createScene();   
    private:
        void addModel(const char* model_name, const char *tex_name, const char *vs_name, const char *fs_name);             

};

