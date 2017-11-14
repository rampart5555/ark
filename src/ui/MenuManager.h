#ifndef __MENUMANAGER__
#define __MENUMANAGER__
#include <map>
#include <stdio.h>
#include <osg/MatrixTransform>
#include "Menu.h"
#include "Config.h"

class MenuManager
{
    public:
        static MenuManager& instance()
        {
            static MenuManager instance;
            return instance;
        }        
        void createMenu();
        osg::MatrixTransform* getMenuNode();
        MenuStart* getMenuStart() { return m_start; }
        MenuLevelSelect* getMenuLevelSelect() { return m_levelSelect; }
        MenuSceneHud* getMenuSceneHud() { return m_sceneHud; }
        MenuLevelComplete* getMenuLevelComplete() { return m_levelComplete; }
        MenuLevelFailed* getMenuLevelFailed() { return m_levelFailed; }
        
    protected:        
        osg::MatrixTransform *m_menuNode;        
        MenuLevelSelect *m_levelSelect;
        MenuLevelComplete *m_levelComplete;
        MenuLevelFailed *m_levelFailed;
        MenuStart *m_start;
        MenuSceneHud *m_sceneHud;
        
    private:
        MenuManager();
};

#endif
