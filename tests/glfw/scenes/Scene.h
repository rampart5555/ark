#ifndef __SCENE__
#define __SCENE__

#include <stdio.h>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include "Config.h"

class Scene
{
    public:
        Scene();
        virtual ~Scene();
        virtual void createScene();
        virtual void handleSceneEvent();
        virtual osg::ref_ptr <osg::Program> createProgram();
        
        osg::PositionAttitudeTransform* getSceneNode();
       
        osg::Program* loadProgram(const char*, const char*);
        osg::Texture2D* loadTexture(const char*);
        osg::Node* loadSceneModel(const char*);
        
    protected:
        osg::ref_ptr<osg::PositionAttitudeTransform> m_rootNode;        
};


#endif
