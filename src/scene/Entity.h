#ifndef __ENTITY__
#define __ENTITY__

#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <Box2D/Box2D.h>
#include "Config.h"
const unsigned int CAT_WALL           = 0x0001;
const unsigned int CAT_BALL           = 0x0002;
const unsigned int CAT_BRICK          = 0x0004;
const unsigned int CAT_PADDLE         = 0x0008;
const unsigned int CAT_POWERUP        = 0x0010;
const unsigned int CAT_BULLET         = 0x0020;
const unsigned int CAT_ALL    = 0xFFFF;

const unsigned int CAT_WALL_MASK           = 0x0001;
const unsigned int CAT_BALL_MASK           = 0x0002;
const unsigned int CAT_BRICK_MASK          = 0x0004;
const unsigned int CAT_PADDLE_MASK         = 0x0008;
const unsigned int CAT_POWERUP_MASK        = 0x0010;
const unsigned int CAT_BULLET_MASK         = 0x0020;
const unsigned int CAT_ALL_MASK    = 0xFFFF;
# if 0
typedef enum 
{
    ENTITY_NONE         = 0,
    ENTITY_WALL         = 1,
    ENTITY_BALL         = 2,
    ENTITY_BRICK        = 3,
    ENTITY_PADDLE       = 4,
    ENTITY_POWERUP      = 5,
    ENTITY_WALL_TOP     = 6,
    ENTITY_WALL_BOTTOM  = 7,
    ENTITY_WALL_LEFT    = 8,
    ENTITY_WALL_RIGHT   = 9,
    ENTITY_BACKGROUND   = 10,
    
}EntityType;
#endif
class EntityBall;
class EntityPaddle;
class EntityBullet;
class Entity : public osg::Referenced
{
    public:
        Entity();   
        Entity(Entity& ent);
        void setModel(osg::MatrixTransform& );
        osg::MatrixTransform* getEntityNode();
        void setName(const char* name) { m_name=name; }
        std::string& getName() { return m_name; }
        void setType(EntityType);        
        EntityType getType();
        void setSubType(int stype) { m_subtype = stype; }
        int  getSubType() { return m_subtype; }
        void updateInitialTransform();

        virtual void update();
        virtual void setPosition(osg::Vec3 pos);
        virtual osg::Vec3& getPosition();
        /* physics */
        virtual bool enablePhysics();
        virtual bool disablePhysics();
        void setCategoryBits(EntityType);        
        void setJoint(b2Body*);
        virtual void beginContact(Entity*, b2Contact*);    
        virtual void endContact(Entity*, b2Contact* );
        virtual EntityBall* asEntityBall() { return 0; }
        virtual EntityPaddle* asEntityPaddle() { return 0; }
        virtual EntityBullet* asEntityBullet() { return 0; }
        b2Body* getPhyBody() { return m_phyBody; }
        bool phyActive()     { return m_phyActive; }
        bool isValid()       { return m_valid; }
        void setUniforms();
        
        /*box 2d */
        bool setBoxShape(osg::Geode*);
        bool setCircleShape(osg::Geode*);
        bool setPolygonShape(osg::Geode*);
        
        /* score */
        void setHits(int hits)    { m_hits = hits; }
        int  getHits()            { return m_hits; }
        void setValue(int value)  { m_value = value; }
        int  getValue()           { return m_value; }  
        void setPowerup( PowerupType pup) { m_powerup = pup; }
        PowerupType  getPowerup( )        { return m_powerup; }
        void setColor(int color)  { m_color = color; }
        
    protected:
        ~Entity();
        osg::ref_ptr<osg::MatrixTransform> m_transform;
        EntityType m_type;
        int m_subtype;
        /* ode physics */
        bool m_phyActive;
        b2Body *m_phyBody;
        b2PrismaticJoint *m_phyJoint;
        std::string m_name;
        bool m_valid;
        int m_hits;
        int m_value;
        PowerupType m_powerup;
        unsigned int m_color;
        osg::Vec3 m_position;
};

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) 
    {
        Entity *eA, *eB;
                               
        eA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
        eB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());
        if( (eA!=NULL) && (eB!=NULL) )
        {   
            //printf("BeginContact:%s<->%s\n",eA->getName().c_str(),eB->getName().c_str());            
            eA->beginContact(eB,contact);
            eB->beginContact(eA,contact);            
        }
    }

    void EndContact(b2Contact* contact) 
    {
        Entity *eA, *eB;
                        
        eA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
        eB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());
        if( (eA!=NULL) && (eB!=NULL) )
        { 
            //printf("EndContact:%s<->%s\n",eA->getName().c_str(),eB->getName().c_str());                    
            eA->endContact(eB, contact);
            eB->endContact(eA, contact);
            
        }
    }
};

#endif
