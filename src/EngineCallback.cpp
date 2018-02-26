#include <stdio.h>
#include "ui/Menu.h"
#include "ui/MenuManager.h"
#include "scene/Scene.h"
#include "scene/LevelManager.h"
#include "scene/LevelManager2.h"
#include "EngineCallback.h"



void EngineEventQueue::setEvent(osg::ref_ptr<EngineEvent> event)
{
    m_eventQueue.push_back(event);
}

void EngineEventQueue::processEvents()
{
    if (m_eventQueue.empty())
        return;
    std::list< osg::ref_ptr<EngineEvent> >::iterator it;
    while( !m_eventQueue.empty() )
    {        
        it=m_eventQueue.begin();
        LOG_INFO("EngineEventQueue::processEvents(): %x %s\n",(*it)->m_eventId,(*it)->m_eventName.c_str());
        Scene_level_callback(*it);        
        m_eventQueue.pop_front();
    }
}

struct cb_item
{
    const char *cb_name;
    EngineCallback cb_func;
};

static struct cb_item cb_map[]={
            
    { "MenuLevelFailed_button_continue_push", MenuLevelFailed_button_continue_push },        
    { "Scene_resume", Scene_resume},
    { "Scene_pause", Scene_pause},    
    { NULL,NULL}
};

EngineCallback getCallback(std::string cb_name)
{    
    for (int i=0;;i++)
    {   
        if(cb_map[i].cb_name==NULL)
        {
            LOG_WARN("Callback function not found:%s\n", cb_name.c_str());
            break;
        }
        else if(cb_name == cb_map[i].cb_name)
            return cb_map[i].cb_func;
    }

    return NULL;
}

void MenuStart_show(void *args)
{
    MenuStart *ms = MenuManager::instance().getMenuStart();
    ms->show();
}

void MenuStart_button_start_push(void *args)
{
    LOG_INFO("%s","MenuStart_button_start_push\n");
    MenuStart *ms = MenuManager::instance().getMenuStart();
    EngineEvent *event = new EngineEvent;  
    event->m_eventId = LEVEL_LOAD;      
    ms->hide(MenuSceneHud_show, NULL);    
    EngineEventQueue::instance().setEvent(event);
}

void MenuSceneHud_show(void *args)
{
    LOG_INFO("%s","MenuSceneHud_show\n");
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();    
    msh->show();
}

/*** MenuLevel Failed ***/
void MenuLevelFailed_show(void *args)
{
    MenuLevelFailed *mlf = MenuManager::instance().getMenuLevelFailed();
    mlf->show();
}

void MenuLevelFailed_button_continue_push(void* args)
{
    MenuLevelFailed *mlf = MenuManager::instance().getMenuLevelFailed();
    mlf->hide(MenuSceneHud_show, args);    
    EngineEvent *event = new EngineEvent;  
    event->m_eventId = LEVEL_CONTINUE;          
    EngineEventQueue::instance().setEvent(event);
}


void Scene_resume(void *args)
{
    Scene::instance().getEntityManager().resumePhysics();
}

void Scene_pause(void *args)
{
    Scene::instance().getEntityManager().pausePhysics();
}

void Scene_paddle_select(void *args)
{
    Scene::instance().getEntityManager().paddleSelect(args);
}

void Scene_paddle_move(void *args)
{
    Scene::instance().getEntityManager().paddleMove(args);
}

void Scene_paddle_unselect(void *args)
{
    Scene::instance().getEntityManager().paddleUnselect(args);
}

/*Levels Callback*/
void Level_update_score(void *args)
{
    int *score = static_cast<int*>(args);            
    LevelManager::instance().updateScore(*score);
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();
    msh->updateScore( LevelManager::instance().getScore() );
    
}

void Scene_level_callback( osg::ref_ptr<EngineEvent> args)
{
    EventId eid;
    if(args.valid())
    {
        eid=args->m_eventId;        
    }
    else    
    {
        LOG_WARN("Scene_level_callback :%s\n", "NULL status");
        return;
    }
    LOG_INFO("Scene_level_callback: %x\n", eid);
    switch(eid)
    {
        case SCENE_ADD_SPARE_PADDLE:
            {
                Scene::instance().addSparePaddle(osg::Vec3(0,0,1));
            }
            break;            
        case LEVEL_LOAD:  
            {
                Scene::instance().getEntityManager().stopPhysics();
                Scene::instance().resetEntities();

                LevelInfo *level_info = LevelManager2::instance().getCurrentLevel();                
                Scene::instance().loadScene("levels/episode_1.lua", level_info->m_name.c_str());
                Scene::instance().animationStart("animation_level_new");
            }          
            break;
        case LEVEL_CLEARED:
            {
                Scene::instance().animationStart("animation_level_cleared");
            }
            break;

        case LEVEL_ANIMATION_COMPLETE:
            {                
                Scene::instance().animationEnd(args->m_eventName);             
            }
            break;
        case LEVEL_COMPLETED: 
            /* play animation here, after animation stop call LEVEL_LOAD_NEXT
               for the moment just unlock and load next level
            */
            {
                Scene::instance().getEntityManager().stopPhysics();
                Scene::instance().resetEntities();

                LevelInfo *level_info = LevelManager2::instance().getNextLevel();                
                Scene::instance().loadScene("levels/episode_1.lua", level_info->m_name.c_str());
                Scene::instance().animationStart("animation_level_new");                
            }
            break;
        case LEVEL_LOAD_NEXT:         
            break;
        case LEVEL_CONTINUE:
            {
                Scene::instance().getEntityManager().stopPhysics();
                Scene::instance().resetEntities();
                if(Scene::instance().getSparePaddleNumber()==0)
                {
                    LOG_STATE("*** Game End ***:%s\n","");
                    return;
                }
                Scene::instance().animationStart("animation_level_continue");                
            }
            break;
        case LEVEL_FAILED:
            {
                Scene::instance().getEntityManager().stopPhysics();
                Scene::instance().resetEntities();
                MenuLevelFailed_show(NULL);        
            }
            break;
        default:
            break;
    }   
}

