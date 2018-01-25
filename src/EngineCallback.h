#ifndef __ENGINE_CALLBACK__
#define __ENGINE_CALLBACK__
#include <string>
#include <list>

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

typedef enum 
{
    LEVEL_COMPLETE
}EventType;

typedef struct
{
    EventType m_type;    

}EngineEvent;

class EngineEventQueue
{
    public:
        EngineEventQueue(){}
        static EngineEventQueue& instance()
        {
            static EngineEventQueue instance;
            return instance;
        }
        void setEvent(EngineEvent event)
        {
            m_eventQueue.push_back(event);
        }
        void processEvents()
        {
            if (m_eventQueue.empty())
                return;
            std::list<EngineEvent>::iterator it;
            while( !m_eventQueue.empty() )
            {
                it=m_eventQueue.begin();
                if( (*it).m_type==LEVEL_COMPLETE )
                    Level_completed(NULL);
                m_eventQueue.pop_front();
            }
        }
    private:
        std::list <EngineEvent> m_eventQueue;
};
#endif
