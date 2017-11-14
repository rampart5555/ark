#include "PSystem.h"
PSystem::PSystem()
{
    
}

void PSystem::init()
{    
    m_root = new osg::Group();

    m_emitter = new osgParticle::ModularEmitter;
    m_psystem = new osgParticle::ParticleSystem;    
    m_emitter->setParticleSystem(m_psystem.get());
    m_counter = new osgParticle::RandomRateCounter;     
    m_emitter->setCounter(m_counter.get());        
    m_placer = new osgParticle::SectorPlacer;
    m_emitter->setPlacer(m_placer.get());    
    m_shooter = new osgParticle::RadialShooter;
    m_emitter->setShooter(m_shooter.get());
    
}

void PSystem::setup()
{
    //ptemplate
    m_ptemplate.setShape(osgParticle::Particle::HEXAGON);
    m_ptemplate.setLifeTime(0.5);
    m_ptemplate.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    m_ptemplate.setAlphaRange(osgParticle::rangef(0.1f, 1.0f));
    m_ptemplate.setColorRange(osgParticle::rangev4(
                                            osg::Vec4(1.0f, 0.8f, 0.2f, 1.0f),
                                            osg::Vec4(1.0f, 0.4f, 0.1f, 0.0f)));
    
    //emitter
    m_emitter->setStartTime(0.0);
    //m_emitter->setLifeTime(1.0);
    //m_emitter->setEndless(false);
    
    //particle system
    m_psystem->setDefaultParticleTemplate(m_ptemplate);
    //m_psystem->setParticleScaleReferenceFrame(osgParticle::ParticleSystem::LOCAL_COORDINATES);
    
    //counter
    m_counter->setRateRange(50, 100);
    
    //placer
    m_placer->setCenter(0, 0, 0);
    m_placer->setRadiusRange(2.0, 3.0);
    
    //shooter
    m_shooter->setThetaRange(0.0f, osg::PI_2);
    m_shooter->setInitialSpeedRange(1.0f, 10.0f);
    
    // psu
    m_root->addChild(m_emitter);    
    osg::ref_ptr<osgParticle::ParticleSystemUpdater> psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(m_psystem.get());
    m_root->addChild(psu.get());    
    
    // drawable
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(m_psystem.get());
    m_root->addChild(geode);
}

void PSystem::loadShader()
{
    osg::StateSet *stateset = new osg::StateSet;

    osg::Program *program = new osg::Program;

    program->addShader(osg::Shader::readShaderFile(osg::Shader::VERTEX,"shaders/particle.vert"));
    program->addShader(osg::Shader::readShaderFile(osg::Shader::FRAGMENT,"shaders/particle.frag"));

    stateset->setAttributeAndModes(program, osg::StateAttribute::ON);
        
    m_psystem->setStateSet(stateset);

    m_psystem->setUseVertexArray(false);
    m_psystem->setUseShaders(true);
    osg::BlendFunc *blend = new osg::BlendFunc;
    //blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE);
    blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE);
    stateset->setAttributeAndModes(blend, osg::StateAttribute::ON);
    
    osg::Texture2D *texture = new osg::Texture2D;
    texture->setImage(osgDB::readRefImageFile("images/fx_blurry_dot3.png"));
    texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::MIRROR);
    texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::MIRROR);
    stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
    
}

