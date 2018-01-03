#include <osgDB/ReadFile>
#include "MenuManager.h"
#include "scene/Scene.h"


MenuManager::MenuManager()
{
    m_levelSelect = NULL;
    m_levelComplete = NULL;
    m_menuNode = new osg::MatrixTransform();
    createMenu();
    
}

void MenuManager::createMenu()
{
    m_start = new MenuStart(this);
    m_start ->createMenu();
    m_menuNode->addChild(m_start->getMenuNode());
    
    m_levelSelect = new MenuLevelSelect(this);
    m_levelSelect->createMenu();
    m_menuNode->addChild(m_levelSelect->getMenuNode());
    m_levelSelect->getMenuNode()->setNodeMask(0x0);

    m_levelComplete = new MenuLevelComplete(this);
    m_levelComplete->createMenu();
    m_menuNode->addChild(m_levelComplete->getMenuNode());        
    m_levelComplete->getMenuNode()->setNodeMask(0x0);
    
    m_sceneHud = new MenuSceneHud(this);
    m_sceneHud->createMenu();
    m_menuNode->addChild(m_sceneHud->getMenuNode());    
    m_sceneHud->getMenuNode()->setNodeMask(0x0);
    
    m_levelFailed = new MenuLevelFailed(this);
    m_levelFailed->createMenu();
    m_menuNode->addChild(m_levelFailed->getMenuNode());        
    m_levelFailed->getMenuNode()->setNodeMask(0x0);
#if 0    
    /* custom menu for testing */
    Widget *showLevelComplete = new Widget();
    showLevelComplete->setNode(AssetsManager::instance().getWidgetModel("widget_button_1"));
    showLevelComplete->setPosition(0.0,4.9,-0.3);
    showLevelComplete->setLabel("Show_LC");
    showLevelComplete->setCallback(MenuLevelComplete_show);
    // set widget visible se ualpah to 1.0f to avoid crash
    showLevelComplete->setUniform(new osg::Uniform("uAlpha",1.0f));
    m_menuNode->addChild(showLevelComplete->getNode());
#endif    
    osg::StateSet *ss = m_menuNode->getOrCreateStateSet();
    ss->addUniform(new osg::Uniform("uAlpha", 1.0f));
    
}

osg::MatrixTransform* MenuManager::getMenuNode()
{
    return m_menuNode;
}


