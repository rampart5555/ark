#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>
#include <osg/BlendFunc>
#include <osgDB/WriteFile>
#include "Particle.h"
#include "ps_template.h"
#include "PSystem.h"

class ParticleSystem_GLES2 :public osgParticle::ParticleSystem
{
    public:
        ParticleSystem_GLES2();
        virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
        void setAttr(bool, int);
};

ParticleSystem_GLES2::ParticleSystem_GLES2():osgParticle::ParticleSystem()
{
}

void ParticleSystem_GLES2::setAttr(bool emissive_particles, int texture_unit)
{

    osg::StateSet *stateset = new osg::StateSet;

    osg::Texture2D *texture = new osg::Texture2D;
    texture->setImage(osgDB::readRefImageFile("images/particle.png"));
    texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::MIRROR);
    texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::MIRROR);
    stateset->setTextureAttributeAndModes(texture_unit, texture, osg::StateAttribute::ON);
    

    osg::BlendFunc *blend = new osg::BlendFunc;
    if (emissive_particles)
    {
        blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE);
    }
    else
    {
        blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    }
    stateset->setAttributeAndModes(blend, osg::StateAttribute::ON);

    osg::Program *program = new osg::Program;

    program->addShader(osg::Shader::readShaderFile(osg::Shader::VERTEX,"shaders/particle.vert"));
    program->addShader(osg::Shader::readShaderFile(osg::Shader::FRAGMENT,"shaders/particle.frag"));

    stateset->setAttributeAndModes(program, osg::StateAttribute::ON);
        
    setStateSet(stateset);

    setUseVertexArray(false);
    setUseShaders(true);
}

void ParticleSystem_GLES2::drawImplementation(osg::RenderInfo& renderInfo) const
{
        osg::State& state = *renderInfo.getState();

        ScopedReadLock lock(_readWriteMutex);

        // update the frame count, so other objects can detect when
        // this particle system is culled
        _last_frame = state.getFrameStamp()->getFrameNumber();

        // update the dirty flag of delta time, so next time a new request for delta time
        // will automatically cause recomputing
        _dirty_dt = true;

        // get the current modelview matrix
        osg::Matrix modelview = state.getModelViewMatrix();

        // set up depth mask for first rendering pass
    #if !defined(OSG_GLES1_AVAILABLE) && !defined(OSG_GLES2_AVAILABLE) && !defined(OSG_GL3_AVAILABLE)
        glPushAttrib(GL_DEPTH_BUFFER_BIT);
    #endif

        glDepthMask(GL_FALSE);

        // render, first pass
        if (_useVertexArray)
            render_vertex_array(renderInfo);
        else
            single_pass_render(renderInfo, modelview);

    #if !defined(OSG_GLES1_AVAILABLE) && !defined(OSG_GLES2_AVAILABLE) && !defined(OSG_GL3_AVAILABLE)
        // restore depth mask settings
        glPopAttrib();
    #endif

        // render, second pass
        if (_doublepass) {
            // set up color mask for second rendering pass
    #if !defined(OSG_GLES1_AVAILABLE) && !defined(OSG_GLES2_AVAILABLE) && !defined(OSG_GL3_AVAILABLE)
            glPushAttrib(GL_COLOR_BUFFER_BIT);
    #endif
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            // render the particles onto the depth buffer
            if (_useVertexArray)
                render_vertex_array(renderInfo);
            else
                single_pass_render(renderInfo, modelview);

    #if !defined(OSG_GLES1_AVAILABLE) && !defined(OSG_GLES2_AVAILABLE) && !defined(OSG_GL3_AVAILABLE)
            // restore color mask settings
            glPopAttrib();
    #endif
        }

}

void create_particle_system(osg::Group *ps_root)
{
    osgParticle::Particle ptemplate;
    ptemplate.setShape(osgParticle::Particle::HEXAGON);
    //ptemplate.setShape(osgParticle::Particle::POINT);
    ptemplate.setLifeTime(1.0);    
    ptemplate.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    ptemplate.setAlphaRange(osgParticle::rangef(1.0f, 0.0f));
    float sr,sg,sb,sa;
    float er,eg,eb,ea;
    sr = 104.0/256; sg = 184.0/256; sb = 51.0/256; sa = 50.0/256;
    er = 144.0/256; eg = 196.0/256; eb = 12.0/256; ea = 100.0/256;
    ptemplate.setColorRange(osgParticle::rangev4(osg::Vec4(sr, sg, sb, sa), osg::Vec4(er, eg, eb, ea)));
    ptemplate.setRadius(0.5f);    
    ptemplate.setMass(0.05f);
    
    ParticleSystem_GLES2 *ps = new ParticleSystem_GLES2();
    ps->setAttr(true,0);
    ps->setDefaultParticleTemplate(ptemplate);
    
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;    
    emitter->setParticleSystem(ps);

    // setup the counter
    osgParticle::RandomRateCounter *counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(50, 100);
    emitter->setCounter(counter);

    // setup the placer; it will be a circle of radius 5 (the particles will
    // be placed inside this circle).
    osgParticle::SectorPlacer *placer = new osgParticle::SectorPlacer;
    placer->setCenter(0, 0, 0);
    placer->setRadiusRange(2.0, 3.0);
    placer->setPhiRange(0, 2 * osg::PI);    // 360\B0 angle to make a circle
    emitter->setPlacer(placer);    
    
    ps_root->addChild(emitter);
    
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    shooter->setInitialSpeedRange(0, 0);
    shooter->setPhiRange(0.0f, osg::PI_2);
    shooter->setInitialSpeedRange(1.0f,10.0f);
    emitter->setShooter(shooter);
#if 0    
    osgParticle::ModularProgram *program = new osgParticle::ModularProgram;
    program->setParticleSystem(ps);    
    osgParticle::AccelOperator *op1 = new osgParticle::AccelOperator;
    op1->setToGravity();
    program->addOperator(op1);
    ps_root->addChild(program);
#endif    
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
    ps_root->addChild(psu);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
    ps_root->addChild(geode);  
 
}

Particle::Particle():Scene()
{
    printf("Particle::m_rootNode:%p\n",m_rootNode);
}

Particle::~Particle()
{
    printf("Particle Destructor\n");
}

void Particle::createScene()
{   
#if 0
    osg::Group* ps_group = new osg::Group();
    create_particle_system(ps_group);
    m_rootNode->addChild(ps_group);  
#else    
    bool use_file = false;
    
    PSystem *ps = new PSystem();
    ps->init();
    ps->setup();
    ps->loadShader();
    osg::Group *ps_root = ps->getRootNode();

    m_rootNode->addChild(ps_root);


#endif    

}

#if 0
void Particle::createScene()
{        
   osg::Geode* geode = new osg::Geode();

    osg::Geometry* pointsGeom = new osg::Geometry();

    osg::Vec3Array* vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(1.0, 1.0, 0.0));
    vertices->push_back(osg::Vec3(-1.0, 1.0, 0.0));
    vertices->push_back(osg::Vec3(-1.0, -1.0, 0.0));
    vertices->push_back(osg::Vec3(1.0, -1.0, 0.0));
    vertices->push_back(osg::Vec3(0.0, 0.0, 0.0));


    pointsGeom->setVertexArray(vertices);

    osg::Vec4Array* colors = new osg::Vec4Array;        
    colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));


    //pointsGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


    //osg::Vec3Array* normals = new osg::Vec3Array;
    //normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    //pointsGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);



    pointsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,vertices->size()));
    
    osg::StateSet* sset = pointsGeom->getOrCreateStateSet();
    //sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    sset->setMode(GL_VERTEX_PROGRAM_POINT_SIZE, osg::StateAttribute::ON);
    //osg::Point* p = new osg::Point;
    //p->setSize(6);
    //sset->setAttribute( p );
    geode->setInitialBound(osg::BoundingSphere(osg::Vec3(), 2.0));
    geode->addDrawable(pointsGeom);
    geode->setCullingActive(false);
    
    m_rootNode->addChild(geode);

}
#endif







