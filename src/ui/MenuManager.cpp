#include <osgDB/ReadFile>
#include "MenuManager.h"
#include "scene/Scene.h"


MenuManager::MenuManager()
{
    
    m_menuNode = new osg::MatrixTransform();
    createMenu();
    
}

void MenuManager::createMenu()
{
    m_start = new MenuStart(this);
    m_start ->createMenu();
    m_menuNode->addChild(m_start->getMenuNode());
    

    
    m_sceneHud = new MenuSceneHud(this);
    m_sceneHud->createMenu();
    m_menuNode->addChild(m_sceneHud->getMenuNode());    
    m_sceneHud->getMenuNode()->setNodeMask(0x0);
    
    m_levelFailed = new MenuLevelFailed(this);
    m_levelFailed->createMenu();
    m_menuNode->addChild(m_levelFailed->getMenuNode());        
    m_levelFailed->getMenuNode()->setNodeMask(0x0);

    osg::StateSet *ss = m_menuNode->getOrCreateStateSet();
    ss->addUniform(new osg::Uniform("uAlpha", 1.0f));
    
}

osg::MatrixTransform* MenuManager::getMenuNode()
{
    return m_menuNode;
}


