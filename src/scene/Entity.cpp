#include <stdio.h>
#include <osg/Geometry>
#include "AssetsManager.h"
#include "Logging.h"
#include "EntityManager.h"
#include "Entity.h"


Entity::Entity()
{
    m_type = ENTITY_NONE;
    m_subtype = 0;   
    m_transform = NULL; 
    m_phyBody = NULL;
    m_valid = true;
    m_phyActive = false;
    m_hits  = 0;
    m_value = 0;    
    m_powerup = POWERUP_NONE;
    m_color = 0xc0c0c0FF;
}

Entity::Entity(Entity& ent)
{
    LOG_DEBUG("Entity copy contructor: %s", ent.m_name.c_str());
    m_type = ent.m_type;
    m_subtype = ent.m_subtype;
    m_name = ent.m_name;
    m_hits = ent.m_hits;
    m_value = ent.m_value;
    m_transform = new osg::MatrixTransform(*(ent.m_transform), osg::CopyOp::DEEP_COPY_ALL);
    m_phyBody = NULL;
    m_valid = true;
    m_phyActive = false; 
    m_powerup = POWERUP_NONE;     
    m_color = 0xc0c0c0FF;  
}

Entity::~Entity()
{
    //debug_log(INFO,"Entity destructor:%s\n",m_name.c_str());
}

void Entity::setModel(osg::MatrixTransform& tr)
{
    m_transform = new osg::MatrixTransform(tr, osg::CopyOp::DEEP_COPY_ALL);
}

void Entity::setUniforms()
{
    float r,g,b;
    r = ((m_color>>16) & 0xff)/255.0;
    g = ((m_color>>8 ) & 0xff)/255.0;
    b = ((m_color>>0 ) & 0xff)/255.0;   
    //LOG_DEBUG("Entity::setUniforms()=> %s %d %f %f %f\n", m_name.c_str(), m_color,r,g,b);
    osg::Uniform* uEntityDiffuseColor = new osg::Uniform("uEntityDiffuseColor",osg::Vec4(r,g,b,1.0));
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->addUniform(uEntityDiffuseColor);     
#if 0
    osg::Texture *texture=NULL;
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    
    if(m_subtype==1)    
        texture = AssetsManager::instance().getTexture("texture_brick");    
    else if(m_subtype==2)
        texture = AssetsManager::instance().getTexture("texture_yellow");
    else if(m_subtype==3)
        texture = AssetsManager::instance().getTexture("texture_white");
    else if(m_subtype==4)
        texture = AssetsManager::instance().getTexture("texture_red");
    else if(m_subtype==5)
        texture = AssetsManager::instance().getTexture("texture_lime");
    
    if(texture != NULL)
    {    
        ss->setTextureAttribute(0, texture);
        osg::Uniform* baseTextureSampler = new osg::Uniform("wall_sampler",0);
        ss->addUniform(baseTextureSampler);     
    }
    else
    {
        printf("NULL TEXTURE entity subtype:%d\n",m_subtype);
    }
#endif    
}

osg::MatrixTransform* Entity::getEntityNode()
{
    return m_transform;
}

void Entity::setPosition(osg::Vec3 pos)
{
    m_position = pos;
    m_transform->setMatrix(osg::Matrix::translate(m_position));
}

osg::Vec3& Entity::getPosition()
{
    return m_position;
}

void Entity::setType(EntityType etype)
{
    m_type = etype;
}

EntityType Entity::getType()
{
    return m_type;
}

void Entity::updateInitialTransform()
{
    if(m_transform == NULL)
    {
        LOG_ERROR("%s\n","EntityPhysics::updateInitialTransform(): Entity model is NULL");
        return;
    }
    if(m_phyActive==false)
        return;    
    osg::Vec3f osg_pos,scale;
    osg::Quat osg_rot, so;
    osg::Matrix m = m_transform->getMatrix();
    m.decompose(osg_pos, osg_rot, scale, so);   
    m_phyBody->SetTransform(b2Vec2(osg_pos.x(), osg_pos.y()), 0.0);
    
}

void Entity::update()
{
    if(m_phyActive==false)
        return;
    if( m_phyBody->GetType() != b2_dynamicBody)
        return;
        
    const b2Vec2& pos = m_phyBody->GetPosition();
    osg::Matrix osg_pos = osg::Matrix::translate(pos.x, pos.y, 0.0);
    float bodyAngle = m_phyBody->GetAngle();
    osg::Matrix rot; 
    rot.makeRotate(bodyAngle, 0, 0, 1);
    m_transform->setMatrix(rot*osg_pos);        
}

void Entity::setCategoryBits( EntityType etype)
{
    /* fixture point to first fixture */
    b2Fixture *fixture = m_phyBody->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    
    if((m_type==ENTITY_WALL_TOP) || (m_type == ENTITY_WALL_BOTTOM) || 
       (m_type == ENTITY_WALL_LEFT) || (m_type == ENTITY_WALL_RIGHT))
    {
        filter.categoryBits = CAT_WALL;
        filter.maskBits = CAT_BALL | CAT_PADDLE | CAT_BULLET;
    } 
    else if(m_type==ENTITY_BRICK)
    {
        filter.categoryBits = CAT_BRICK;
        filter.maskBits = CAT_BALL | CAT_BULLET;
    }
    else if(m_type==ENTITY_BALL)
    {
        filter.categoryBits = CAT_BALL;
        filter.maskBits = CAT_WALL | CAT_BRICK | CAT_PADDLE;
    }
    else if(m_type==ENTITY_PADDLE)
    {
        filter.categoryBits = CAT_PADDLE;
        filter.maskBits = CAT_WALL | CAT_BALL | CAT_POWERUP;
    }   
    else if(m_type==ENTITY_POWERUP)
    {
        filter.categoryBits = CAT_POWERUP;
        filter.maskBits = CAT_PADDLE;
    }  
    else if(m_type==ENTITY_BULLET)
    {        
        filter.categoryBits = CAT_BULLET;
        filter.maskBits = CAT_WALL | CAT_BRICK;        
    }
     
    fixture->SetFilterData(filter);
}


bool Entity::enablePhysics()
{
    if(m_phyActive == true)
    {
        LOG_WARN("%s","Entity::enablePhysics(): Physics already active\n");
        return true;
    }
    if(m_transform == NULL)
    {
        LOG_WARN("%s", "Entity::enablePhysics(): Entity model is NULL\n");
        return false;
    }
    if(m_transform->getNumChildren()==0)
    {
        LOG_WARN("%s", "Entity::enablePhysics(): Unable to compute BoundingBox\n");
        return false;
    }
    //osg::Node *nd = m_transform->getChild(0);
    osg::Geode *geo = dynamic_cast<osg::Geode*>(m_transform->getChild(0));
    if(geo==NULL)
    {
        LOG_WARN("%s", "Entity::enablePhysics(): dynamic cast to geode fail\n");
        return false;
    }
    LOG_INFO("Entity::enablePhysics(): Physics enable for entity:%s\n", m_name.c_str());     
    switch(m_type)
    {        
#if 0       
        case ENTITY_PADDLE:
        {
            setBoxShape(geo);
            m_phyActive = true;
            updateInitialTransform();            
            setCategoryBits(m_type);
            break;            
        }
#endif        
        case ENTITY_BRICK:
        case ENTITY_WALL_TOP:
        case ENTITY_WALL_BOTTOM:
        case ENTITY_WALL_LEFT:  
        case ENTITY_WALL_RIGHT:    
        case ENTITY_POWERUP:
        {               
            setBoxShape(geo);
            m_phyActive = true;
            updateInitialTransform();
            setCategoryBits(m_type);
            if(m_type==ENTITY_POWERUP)
                m_phyBody->SetLinearVelocity(b2Vec2(0.0,-0.3));
            break;
        }         
        default:
            break;       
    }
    return m_phyActive;
 
}

bool Entity::disablePhysics()
{
    if(m_phyActive==false)
        return false;        
    m_phyActive=false;
    if(m_phyBody != NULL)
    {
        EntityManager::m_world->DestroyBody( m_phyBody );
        m_phyBody = NULL;
    }
    return true;
}

void Entity::beginContact(Entity *ent, b2Contact *contact)
{
    if( (m_type==ENTITY_BRICK) && (ent->getType() == ENTITY_BALL) )
    {
        m_hits--;
        if(m_hits <= 0 )
            m_valid = false;        
    }
    else if ( (m_type==ENTITY_POWERUP) && (ent->getType() == ENTITY_PADDLE))
    {
        m_valid = false;
    }               
}

void Entity::endContact(Entity *ent, b2Contact *contact)
{    
    
}

void Entity::setJoint(b2Body *background)
{
#if 0
    if(m_type != ENTITY_PADDLE)
    {
        LOG_WARN("%s", "Joint is only for paddle\n");
        return;
    }
    
    b2PrismaticJointDef pjd;
    b2Vec2 axis(1.0f, 0.0f);
    pjd.Initialize(background, m_phyBody, b2Vec2(0.0f, 0.0f), axis);
    pjd.enableMotor = false;    
    m_phyJoint = (b2PrismaticJoint*)EntityManager::m_world->CreateJoint(&pjd);
#endif    
}

bool Entity::setBoxShape(osg::Geode *geode)
{
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    if( (m_type == ENTITY_POWERUP) || (m_type == ENTITY_PADDLE))
        bodyDef.type = b2_dynamicBody;
    else
        bodyDef.type = b2_staticBody;
    bodyDef.linearDamping = 0.0f;
    m_phyBody = EntityManager::m_world->CreateBody(&bodyDef);
    
    osg::BoundingBox bb = geode->getBoundingBox();
    float lx = bb.xMax() - bb.xMin();
    float ly = bb.yMax() - bb.yMin();    
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(lx/2.0, ly/2.0);  
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 20.0;
    fixtureDef.friction = 0.0;
    fixtureDef.restitution = 1.0;
    m_phyBody->CreateFixture(&fixtureDef);
    m_phyBody->SetUserData(this); 
    return true;
}

bool Entity::setCircleShape(osg::Geode *geode)
{
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    bodyDef.type = b2_dynamicBody;    
    bodyDef.linearDamping = 0.0f;
    
    m_phyBody = EntityManager::m_world->CreateBody(&bodyDef);
    
    osg::BoundingBox bb = geode->getBoundingBox();
    float lx = bb.xMax() - bb.xMin();    
    
    b2CircleShape circleShape;
    circleShape.m_p.Set( 0.0, 0.0 );
    circleShape.m_radius=lx/2.0;
    fixtureDef.shape=&circleShape;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 0.0;
    fixtureDef.restitution = 1.0;
    m_phyBody->CreateFixture( &fixtureDef);  	    
    m_phyBody->SetUserData(this); 
    return true;
        
}

bool Entity::setPolygonShape(osg::Geode *geode)
{
    return true;  
}
