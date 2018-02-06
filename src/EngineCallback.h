#ifndef __ENGINE_CALLBACK__
#define __ENGINE_CALLBACK__
#include <string>
#include <list>
#include "Logging.h"

typedef void (*EngineCallback)(void *args);
EngineCallback getCallback(std::string);

void MenuStart_show(void* args);
void MenuStart_button_start_push(void *args);

void MenuLevelSelect_show(void* args);
void MenuLevelSelect_button_level_push(void *args);
void MenuLevelSelect_next_episode(void *args);
void MenuLevelSelect_prev_episode(void *args);

void MenuLevelComplete_show(void *args);
void MenuLevelComplete_button_levels_push(void *args);
void MenuLevelComplete_button_restart_push(void *args);
void MenuLevelComplete_button_next_push(void *args);

void MenuLevelFailed_show(void *args);
void MenuLevelFailed_button_levels_push(void *args);
void MenuLevelFailed_button_continue_push(void *args);

void MenuSceneHud_show(void *args);
void MenuSceneHud_update_score(void *args);
void MenuSceneHud_button_levels_push(void *args);
void MenuSceneHud_button_lc_push(void *args); //level complete test funtion
void MenuSceneHud_button_lf_push(void *args); //level failed test funtion

void Scene_load_level(void *args);
void Scene_resume(void *args);
void Scene_pause(void *args);
void Scene_paddle_select(void *args);
void Scene_paddle_move(void *args);
void Scene_paddle_unselect(void *args);
void Scene_level_complete(void *args);
void Scene_level_complete(void *args);
void Scene_unlock_next_level(void *args);
void Scene_level_failed(void *args);
void Scene_level_continue(void *args);

/*levels*/
void Level_update_score(void *args);
void Level_cleared(void* args);
void Level_completed(void* args);

/*
NEW STYLE OF CALLBACK FOR SCENES
*/

typedef enum
{
    LEVEL_LOAD = 0xab11,
    LEVEL_CLEARED = 0xab12,
    LEVEL_COMPLETED = 0xab13,
    LEVEL_FAILED = 0xab14,
    LEVEL_ANIMATION_COMPLETE=0xab15

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
