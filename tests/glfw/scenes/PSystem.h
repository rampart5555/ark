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
#include <osg/Texture2D>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>

class PSystem
{
    public:
        PSystem();        
        void init();
        void setup();
        void loadShader();
        osg::Group* getRootNode() { return m_root.get(); }
        osgParticle::ParticleSystem* getParticleSystem() { return m_psystem.get(); }
        
    private:
        osg::ref_ptr<osgParticle::ParticleSystem> m_psystem;
        osg::ref_ptr<osgParticle::ModularEmitter> m_emitter;
        osg::ref_ptr<osgParticle::RandomRateCounter> m_counter;
        osg::ref_ptr<osgParticle::SectorPlacer>    m_placer;
        osg::ref_ptr<osgParticle::RadialShooter>   m_shooter;
        osgParticle::Particle                     m_ptemplate;
        osg::ref_ptr<osg::Group> m_root;
};
