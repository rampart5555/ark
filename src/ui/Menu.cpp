#include <osgAnimation/EaseMotion>
#include "Menu.h"
#include "MenuManager.h"
#include "scene/LevelManager.h"
#include "AssetsManager.h"
#include "Animation.h"
#include "Logging.h"

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

Widget* Menu::addWidget(WidgetType wtype, const char* widget_model, const char* label, float x, float y)
{    
    Widget *w;
    if(wtype==WidgetButtonLevelSelect)
        w = new LevelSelectButton();
    else
        w = new Widget();
    w->setNode(AssetsManager::instance().getWidgetModel(widget_model));
    w->setLabel(label);
    w->setPosition(x, y, -0.3);        
    m_transform->addChild(w->getNode());
    m_widgetList.push_back(w);    
    return w;
}

Widget* Menu::getWidget(unsigned int wid)
{
    for(unsigned int i=0;i<m_widgetList.size();i++)
    {
        if(m_widgetList[i]->getId() == wid)
            return m_widgetList[i];
    }
    return NULL;
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
        LOG_DEBUG("MenuSceneHud::createMenu add item:%s\n",(items->at(i)).m_label.c_str());
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
    
    LOG_DEBUG("updateScore: %s\n", buf);
    m_score->setLabel(buf);
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
    for (unsigned int i=0; i<items->size(); i++)
    {
        Widget *w = new Widget(items->at(i));
        m_transform->addChild(w->getNode());
        LOG_DEBUG("Add item:%s\n",(items->at(i)).m_label.c_str());
    }
} 
 

