#ifndef __ENGINE_CALLBACK__
#define __ENGINE_CALLBACK__
#include <string>
#include <list>
#include "Logging.h"

typedef void (*EngineCallback)(void *args);
EngineCallback getCallback(std::string);

void MenuStart_show(void* args);
void MenuStart_button_start_push(void *args);

void MenuLevelFailed_show(void *args);
void MenuLevelFailed_button_continue_push(void *args);

void MenuSceneHud_show(void *args);
void MenuSceneHud_update_score(void *args);

void Scene_resume(void *args);
void Scene_pause(void *args);
void Scene_paddle_select(void *args);
void Scene_paddle_move(void *args);
void Scene_paddle_unselect(void *args);


/*levels*/
void Level_update_score(void *args);

/*
NEW STYLE OF CALLBACK FOR SCENES
*/

typedef enum
{
    LEVEL_LOAD = 0xab11,
    LEVEL_CLEARED = 0xab12,
    LEVEL_COMPLETED = 0xab13,
    LEVEL_FAILED = 0xab14,
    LEVEL_LOAD_NEXT = 0xab15,
    LEVEL_ANIMATION_COMPLETE=0xab16,
    LEVEL_CONTINUE=0xab17,
    SCENE_ADD_SPARE_PADDLE = 0xac11

}EventId;

class EngineEvent: public osg::Object
{
    public:
        EngineEvent(){}
        virtual const char* libraryName() const {return "";}
        virtual const char* className() const {return "";}
        virtual osg::Object* cloneType() const {return NULL;}
        virtual osg::Object* clone(const osg::CopyOp&) const {return NULL;}    
        EventId m_eventId;
        std::string m_eventName;
    private:
        ~EngineEvent()
        {
           LOG_INFO("Delete Event:%s\n","");
        }        
};

void  Scene_level_callback(osg::ref_ptr<EngineEvent> event);


class EngineEventQueue
{
    public:
        EngineEventQueue(){}
        static EngineEventQueue& instance()
        {
            static EngineEventQueue instance;
            return instance;
        }
        void setEvent(osg::ref_ptr<EngineEvent> event);        
        void processEvents();
        
    private:
        std::list < osg::ref_ptr<EngineEvent> > m_eventQueue;
};
#endif
