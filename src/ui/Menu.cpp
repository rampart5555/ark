#include <osgAnimation/EaseMotion>
#include "Menu.h"
#include "MenuManager.h"
#include "scene/SceneData.h"
#include "AssetsManager.h"
#include "Animation.h"


/* Menu */
Menu::Menu(MenuManager *mm)
{
    m_bounds.xMin = -3;
    m_bounds.xMax =  3;
    m_bounds.yMin = -2;
    m_bounds.yMax =  2;
    m_transform = new osg::MatrixTransform();    
    m_menuManager = mm;
}

osg::MatrixTransform* Menu::getMenuNode()
{
    return m_transform;
}

void Menu::createMenu()
{    

}

void Menu::setEnabled(bool en)
{
    if(en == true)
        m_transform->setNodeMask(MASK_FF);
    else
        m_transform->setNodeMask(MASK_00);
        
}

void Menu::show()
{
    m_currentAnimType = AnimationShow;
    setEnabled(true);
    AnimFade *af = new AnimFade(this, 0.0, 1.0, 1.0);
    m_uniform->setUpdateCallback(af);
    af->start(false);
    
}

void Menu::hide(EngineCallback cb, void *args)
{
    m_currentAnimType = AnimationHide;
    AnimFade *af = new AnimFade(this, 1.0, -1.0, 1.0);
    af->setCallback(cb, args);    
    m_uniform->setUpdateCallback(af);           
    af->start(false);
}

void Menu::animationComplete()
{
    if(m_currentAnimType == AnimationShow)
    {
        
    }
    else if(m_currentAnimType == AnimationHide)
    {
        setEnabled(false);
    }
}

/// Menu Start ///
MenuStart::MenuStart(MenuManager *mm):Menu(mm)
{
    m_uniform  = new osg::Uniform( "uAlpha", 1.0f );    
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->addUniform(m_uniform);
}

void MenuStart::createMenu()
{
    Widget *start = new Widget();    
    start->setNode(AssetsManager::instance().getWidgetModel("widget_button_1"));    
    start->setLabel("Start"); 
    start->setPosition(0.0, 0.0, -0.3);       
    start->setCallback(MenuStart_button_start_push);
    m_transform->addChild(start->getNode());        
}


/// Menu Episode ///
MenuEpisode::MenuEpisode(MenuManager *mm):Menu(mm)
{    
    m_uniform = NULL;
    m_episodeId = -1;
}

void MenuEpisode::setEpisodeId(int eid)
{
    m_episodeId = eid;
}

int  MenuEpisode::getEpisodeId()
{
    return m_episodeId;
}

void MenuEpisode::loadShaders()
{

#if 1    
    m_uniform  = new osg::Uniform( "uAlpha", 1.0f );    
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->addUniform(m_uniform);        
#endif
    
}

Widget* MenuEpisode::addWidget(const char* widget_model, const char* label, float x, float y)
{    
    Widget *w = new Widget();
    w->setNode(AssetsManager::instance().getWidgetModel(widget_model));
    w->setLabel(label);
    w->setPosition(x, y, -0.3);        
    m_transform->addChild(w->getNode());    
    return w;
}

void MenuEpisode::loadWidgets()
{
    EpisodeInfo ep_info;
    bool res;
    res = SceneLoader::instance().getEpisodeInfo(m_episodeId, ep_info);
    if(res==false)    
        return;
    Widget *w;
    w = addWidget("widget_title_1", ep_info.ep_name.c_str(), 0.0, 3.0);
    w = addWidget("widget_button_1", "Prev", -2.0, -4.0);
    w->setCallback(MenuLevelSelect_prev_episode);
    w = addWidget("widget_button_1", "Next",  2.0, -4.0);
    w->setCallback(MenuLevelSelect_next_episode); 
}
#if 0
void MenuEpisode::loadWidgets()
{        
    EpisodeData *ep = &Episode[m_episodeId];
    Widget *title = new Widget();
    title->setNode(AssetsManager::instance().getWidgetModel("level_select_title"));
    title->setLabel(ep->m_name);
    title->setPosition(0.0, 3.0, -0.3);
        
    m_transform->addChild(title->getNode());    
    
    /*level select buttons */
    osg::MatrixTransform *but_node = AssetsManager::instance().getWidgetModel("widget_button_1");
    if(but_node == NULL)
    {
        printf("ERROR: Invalid menu assets\n");
        return;
    }
    /* buttons */
    osg::Geode *geode = dynamic_cast <osg::Geode*>(but_node->getChild(0));
    osg::BoundingBox bb = geode->getBoundingBox();
    
    float but_width = bb.xMax() - bb.xMin();    
    float gap = (m_bounds.xMax - m_bounds.xMin -5*but_width)/(5+1);
    /* first button position*/
    float pos_first = m_bounds.xMin + gap + but_width/2;    
    float y = 0.5;
    int scene_index = 0;
    
    for(int j=0;j<3;j++)
    {
        float pos_y = y - j*(gap + but_width);        
        LevelSelectButton *but = new LevelSelectButton();
        but->setNode(but_node);            
        //but->setLabel("12");
        but->setSceneData(&ep->m_scene[scene_index]);
        m_transform->addChild(but->getNode());    
        but->setPosition(pos_first, pos_y, -0.3);
        but->setCallback(MenuLevelSelect_button_level_push);
        scene_index++;
        
        for(int i=1;i<5;i++)
        {
            but = new LevelSelectButton();
            but->setNode(but_node);
            float pos_x = pos_first + i*(gap + but_width);
            //printf("pos_x:%f\n",pos_x);            
            //but->setLabel("12");
            but->setSceneData(&ep->m_scene[scene_index]);
            m_transform->addChild(but->getNode());            
            but->setPosition(pos_x, pos_y, -0.3);
            but->setCallback(MenuLevelSelect_button_level_push);
            scene_index++;
        }        
    }
    Widget *prev = new Widget();    
    prev->setNode(but_node);    
    prev->setLabel("Prev"); 
    prev->setPosition(-2.0, -4.0, -0.3);   
    m_transform->addChild(prev->getNode());
    prev->setCallback(MenuLevelSelect_prev_episode);
       
    Widget *next = new Widget();
    next->setNode(but_node);
    next->setLabel("Next"); 
    next->setPosition(2.0, -4.0, -0.3);   
    next->setCallback(MenuLevelSelect_next_episode);
    m_transform->addChild(next->getNode());

}
#endif
void MenuEpisode::createMenu()    
{
    if( (m_episodeId >= 0) && (m_episodeId <= 2) )
    {
        loadShaders();
        loadWidgets();
    }
    else
    {
        printf("Error: Invalid episdode:%d\n", m_episodeId);
    } 
}

void MenuEpisode::setPosition(float x, float y, float z)
{
    m_position.set(x,y,z);
    osg::Matrix m;
    m.makeTranslate(m_position);
    m_transform->setMatrix(m);
}

MenuLevelSelect::MenuLevelSelect(MenuManager *mm) : Menu(mm)
{
    
}


void MenuLevelSelect::createMenu()
{
    for(int i=0;i<3;i++)
    {
        MenuEpisode *menu = new MenuEpisode(m_menuManager);
        menu->setEpisodeId(i);
        menu->createMenu();
        menu->setPosition(i*ORTHO_WIDTH,0.0, 0.0);
        m_transform->addChild(menu->getMenuNode()); 
        m_menuList.push_back(menu);
    }
    m_menuIndex = 0;
    
}

void MenuLevelSelect::show()
{
    setEnabled(true);    
    m_menuList[0]->show();
    /*
    for(unsigned int i=0; i<m_menuList.size(); i++)
    {
        m_menuList[i]->show();
    } 
    */   
    
}

void MenuLevelSelect::hide(EngineCallback cb, void* args)
{    
    m_menuList[0]->hide(cb, args);
    /* prevent calling callback 3 times */
    /*for(unsigned int i=1; i<m_menuList.size(); i++)
    {
        m_menuList[i]->hide(NULL, NULL);
    }
    */
}

void MenuLevelSelect::nextEpisode()
{
    if(m_menuIndex < 2)
    {
        AnimTranslate *at = new AnimTranslate(-m_menuIndex*ORTHO_WIDTH, -ORTHO_WIDTH, 2.0);        
        m_transform->setUpdateCallback(at);
        at->start(false);
        m_menuIndex++;
    }
}

void MenuLevelSelect::prevEpisode()
{
    if((m_menuIndex > 0))
    {        
        AnimTranslate *at = new AnimTranslate(-m_menuIndex*ORTHO_WIDTH, ORTHO_WIDTH, 2.0);
        m_transform->setUpdateCallback(at);
        at->start(false);                
        m_menuIndex--;        
    }
}

/// MenuSceneHud ///
MenuSceneHud::MenuSceneHud(MenuManager *mm) : Menu(mm)
{
    m_uniform  = new osg::Uniform( "uAlpha", 1.0f );    
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->addUniform(m_uniform);        
}

void MenuSceneHud::createMenu()
{
    std::vector<MenuItem> *items;
    MenuItem itm;
    items = AssetsManager::instance().getMenuItems("MenuSceneHud");
    if(items==NULL)
    {            
        return;
    }
    for (unsigned int i=0; i<items->size(); i++)
    {
        itm = items->at(i);
        Widget *w = new Widget(itm);
        m_transform->addChild(w->getNode());
        printf("MenuSceneHud::createMenu add item:%s\n",(items->at(i)).m_label.c_str());
        if(itm.m_name=="HudScore")
        {
            m_score=w;
        }        
    }
           
}

void MenuSceneHud::updateScore(int score)
{
    char buf[32];
    sprintf(buf,"%d",score);
    
    printf("score: %s\n", buf);
    m_score->setLabel(buf);
}

/////////////////////////////////////////

MenuLevelComplete::MenuLevelComplete(MenuManager *mm):Menu(mm)
{
    m_uniform  = new osg::Uniform( "uAlpha", 1.0f );    
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->addUniform(m_uniform);

}
 
void MenuLevelComplete::createMenu()
{
    std::vector<MenuItem> *items;
    items = AssetsManager::instance().getMenuItems("MenuLevelComplete");
    if(items==NULL)
    {        
        return;
    }
    for (unsigned int i; i<items->size(); i++)
    {
        Widget *w = new Widget(items->at(i));
        m_transform->addChild(w->getNode());
        printf("Add item:%s\n",(items->at(i)).m_label.c_str());
    }
}

/////////////////////////////////////////

MenuLevelFailed::MenuLevelFailed(MenuManager *mm):Menu(mm)
{
    m_uniform  = new osg::Uniform( "uAlpha", 1.0f );    
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->addUniform(m_uniform);

}
 
void MenuLevelFailed::createMenu()
{
    std::vector<MenuItem> *items;
    items = AssetsManager::instance().getMenuItems("MenuLevelFailed");
    if(items==NULL)
    {        
        return;
    }
    for (unsigned int i; i<items->size(); i++)
    {
        Widget *w = new Widget(items->at(i));
        m_transform->addChild(w->getNode());
        printf("Add item:%s\n",(items->at(i)).m_label.c_str());
    }
} 
 

