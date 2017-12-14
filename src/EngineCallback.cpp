#include <stdio.h>
#include "ui/Menu.h"
#include "ui/MenuManager.h"
#include "scene/Scene.h"
#include "scene/LevelManager.h"
#include "EngineCallback.h"
#include "Logging.h"

struct cb_item
{
    const char *cb_name;
    EngineCallback cb_func;
};

static struct cb_item cb_map[]={
    { "MenuLevelComplete_button_levels_push", MenuLevelComplete_button_levels_push },
    { "MenuLevelComplete_button_restart_push", MenuLevelComplete_button_restart_push },
    { "MenuLevelComplete_button_next_push", MenuLevelComplete_button_next_push },
    
    { "MenuLevelFailed_button_levels_push",  MenuLevelFailed_button_levels_push },
    { "MenuLevelFailed_button_restart_push", MenuLevelFailed_button_restart_push },
    
    { "MenuSceneHud_button_levels_push", MenuLevelComplete_button_levels_push },
    { "MenuSceneHud_button_lc_push", MenuSceneHud_button_lc_push },
    { "MenuSceneHud_button_lf_push", MenuSceneHud_button_lf_push },
    { "Scene_start_physics", Scene_start_physics},
    { "Scene_stop_physics", Scene_stop_physics},
    { NULL,NULL}
};

EngineCallback getCallback(std::string cb_name)
{    
    for (int i=0;;i++)
    {   
        if(cb_map[i].cb_name==NULL)
        {
            LOG_WARN("Callback function not found:%s", cb_name.c_str());
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
    ms->hide(MenuLevelSelect_show, NULL);    
}


void MenuLevelSelect_show(void *args)
{
    LOG_INFO("%s","MenuLevelSelect_show\n");
    MenuLevelSelect *mls = MenuManager::instance().getMenuLevelSelect();
    mls->show();    
}

void MenuLevelSelect_button_level_push(void *args)
{
    MenuLevelSelect *mls = MenuManager::instance().getMenuLevelSelect();
    SceneLevel *sdata = static_cast<SceneLevel*>(args);    
    if(sdata->m_unlocked==true)
    {
        Scene_load_level(args);
        mls->hide(MenuSceneHud_show, args);
    }
}


void MenuLevelSelect_next_episode(void *args)
{
    LOG_INFO("%s","MenuLevelSelect_next_episode\n");
    MenuLevelSelect *mls = MenuManager::instance().getMenuLevelSelect();
    mls->nextEpisode();
}

void MenuLevelSelect_prev_episode(void *args)
{
    LOG_INFO("%s","MenuLevelSelect_prev_episode\n");
    MenuLevelSelect *mls = MenuManager::instance().getMenuLevelSelect();
    mls->prevEpisode();
}

void MenuSceneHud_show(void *args)
{
    LOG_INFO("%s","MenuSceneHud_show\n");
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();    
    msh->show();
}
#if 0
void MenuSceneHud_update_score(void *args)
{

    int *score = static_cast<int*>(args);            
    Scene::instance().updateLevelScore(*score);
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();
    msh->updateScore( Scene::instance().getLevelScore() );

}
#endif
void MenuSceneHud_button_levels_push(void *args)
{
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();    
    msh->hide(MenuLevelSelect_show, args);
}

/*test level complete and level failed functions */
void MenuSceneHud_button_lc_push(void *args)
{
    LOG_INFO("%s","MenuLevelComplete_button_lc_push\n");
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud(); 
    msh->hide(MenuLevelComplete_show, args);   
}

void MenuSceneHud_button_lf_push(void *args)
{
    LOG_INFO("%s","MenuSceneHud_button_lf_push\n");
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud(); 
    msh->hide(MenuLevelFailed_show, args);   
}

/*MenuLevelComplete */
void MenuLevelComplete_show(void* args)
{
    LOG_INFO("%s","MenuLevelComplete_show\n");
    MenuLevelComplete *mlc = MenuManager::instance().getMenuLevelComplete();    
    mlc->show();
    
}

void MenuLevelComplete_button_levels_push(void* args)
{
    LOG_INFO("%s","MenuLevelComplete_button_levels_push\n");
    MenuLevelComplete *mlc = MenuManager::instance().getMenuLevelComplete();    
    mlc->hide(MenuLevelSelect_show, args);    
}

void MenuLevelComplete_button_next_push(void* args)
{
    LOG_INFO("%s","MenuLevelComplete_button_next_push\n");
}

void MenuLevelComplete_button_restart_push(void* args)
{
    LOG_INFO("%s","MenuLevelComplete_button_restart_push\n");
}

/*** MenuLevel Failed ***/
void MenuLevelFailed_show(void *args)
{
    MenuLevelFailed *mlf = MenuManager::instance().getMenuLevelFailed();
    mlf->show();
}

void MenuLevelFailed_button_levels_push(void* args)
{
    MenuLevelFailed *mlf = MenuManager::instance().getMenuLevelFailed();
    mlf->hide(MenuLevelSelect_show, args);
}

void MenuLevelFailed_button_restart_push(void* args)
{
    MenuLevelFailed *mlf = MenuManager::instance().getMenuLevelFailed();
    mlf->hide(MenuSceneHud_show, args);
}

/*** Scene ***/
void Scene_load_level(void *args)
{
    SceneEpisode* ep_data;
    SceneLevel *sdata = static_cast<SceneLevel*>(args);    
    ep_data = LevelManager::instance().getEpisode(sdata->m_epId);
    
    /* score */
    LevelManager::instance().resetScore();
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();
    msh->updateScore( 0 );

    if(ep_data != NULL)
    {
        LOG_INFO("Loading level: %s from: %s\n", sdata->m_name.c_str(), ep_data->m_file.c_str());
        LevelManager::instance().setCurrent(sdata->m_epId, sdata->m_id);
        Scene::instance().loadScene(ep_data->m_file.c_str(),sdata->m_name.c_str());
    }    
    

}

void Scene_start_physics(void *args)
{
    Scene::instance().getEntityManager().startPhysics();
}

void Scene_stop_physics(void *args)
{
    Scene::instance().getEntityManager().stopPhysics();
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

void Scene_level_complete(void *args)
{
    
    LOG_INFO("%s","Scene_level_complete\n");
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();
    msh->hide(MenuLevelComplete_show, args);    
    Scene_unlock_next_level(args);
    
}
void Scene_level_failed(void *args)
{
    LOG_INFO("SceneLevelFailed:%s\n","");
}
void Scene_unlock_next_level(void *args)
{
    unsigned int ep_id, lvl_id;
    SceneLevel* sl;

    LevelManager::instance().unlockNextLevel(ep_id, lvl_id);    
    sl = LevelManager::instance().getLevel(ep_id, lvl_id);

    if(sl == NULL)
    {
        LOG_ERROR("Level not for for ep:%d lvl: %d\n",ep_id, lvl_id);
        return;
    }

    MenuLevelSelect *mls = MenuManager::instance().getMenuLevelSelect();
    MenuEpisode *me = mls->getMenuEpisode(ep_id);

    if(me==NULL)
    {
        LOG_ERROR("Menu Episode not found %d:\n",ep_id);
        return;
    }
    LevelSelectButton *but = dynamic_cast<LevelSelectButton*>(me->getWidget(sl->m_widgetId));
    if(but==NULL)
    {
        LOG_ERROR("Widget not found %d:\n",lvl_id);
        return;
    }
    char buf[10];
    sprintf(buf, "%d", sl->m_id + 1);
    but->setLabel(buf);        
}

/*Levels Callback*/
void Level_update_score(void *args)
{
    int *score = static_cast<int*>(args);            
    LevelManager::instance().updateScore(*score);
    MenuSceneHud *msh = MenuManager::instance().getMenuSceneHud();
    msh->updateScore( LevelManager::instance().getScore() );
    
}



