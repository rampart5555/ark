#include "Simple.h"

class AnimateCallback: public osg::UniformCallback
{
    public:
    
        AnimateCallback(/*Operation op*/){}

        virtual void operator() ( osg::Uniform* uniform, osg::NodeVisitor* nv )
        {
           
                float angle = 2.0 * nv->getFrameStamp()->getSimulationTime();
                float sine = sinf( angle );        // -1 -> 1
                float v01 = 0.5f * sine + 0.5f;        //  0 -> 1
                float v10 = 1.0f - v01;                //  1 -> 0
                uniform->set( osg::Vec2(0.505f, 0.8f*v01) ); 
                //uniform->set( sine ); break;
                //uniform->set( osg::Vec3(v10, 0.0f, 0.0f) ); break;
                //uniform->set( osg::Vec3(v01, v01, v10) ); break;                
        }        
};

Simple::Simple():Scene()
{
    printf("Simple::m_rootNode:%p\n",m_rootNode);
}

Simple::~Simple()
{
    printf("Simple Destructor\n");
}

osg::ref_ptr<osg::Geode> Simple::createShape(int type)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Shape> shape;
    if(type == 0)
        shape=new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),1.0f);
    else if(type == 1)
        shape=new osg::Cone(osg::Vec3(2.2f,0.0f,-0.4f),0.9f,1.8f);
    else if(type == 2)
        shape=new osg::Cylinder(osg::Vec3(4.4f,0.0f,0.0f),1.0f,1.4f);
        
    if(shape.valid())
    {
        osg::ref_ptr<osg::Drawable> geom = new osg::ShapeDrawable(shape);
        geode->addDrawable(geom);       
    }
    return geode;
}

void Simple::createScene()
{        
    int shape_type[] = {0, 1, 2};
    
    osg::Texture2D *tex= loadTexture("images/noise_1.jpeg");
    
    for(int i=0; i<3; i++)
    {
        osg::ref_ptr <osg::Geode> geo = createShape(shape_type[i]);
        osg::StateSet* stateset = geo->getOrCreateStateSet();
        stateset->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
        osg::Uniform* texSampler = new osg::Uniform("sampler2D","Noise");
        osg::Uniform* OffsetUniform = new osg::Uniform( "Offset", osg::Vec2(0.0f, 0.0f) );
        OffsetUniform->setUpdateCallback(new AnimateCallback());
        stateset->addUniform(texSampler);
        stateset->addUniform(OffsetUniform);
        m_rootNode->addChild(geo);
    
    }
    osg::Program *prog = loadProgram("shaders/fire.vert","shaders/fire.frag");
    osg::StateSet *ss = m_rootNode->getOrCreateStateSet();
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
    osg::Uniform* texSampler = new osg::Uniform("sampler2D","uSampler");
    ss->addUniform(texSampler);
        
}










