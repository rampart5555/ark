#ifndef __MENU__
#define __MENU__
#include <map>
#include <vector>
#include <stdio.h>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include "AssetsManager.h"

#include "Widget.h"

typedef struct
{
    float xMin;
    float xMax;
    float yMin;
    float yMax;
}MenuBounds;

typedef enum 
{
    AnimationNone,
    AnimationShow,
    AnimationHide
}AnimationType;

typedef enum
{
    WidgetTitle=0,
    WidgetButton,
    WidgetButtonLevelSelect
}WidgetType;

class MenuManager;

class Menu
{
    public:
        Menu(MenuManager*);
        virtual void createMenu();
        osg::MatrixTransform* getMenuNode();          
        virtual void show();
        virtual void hide(EngineCallback, void*);
        virtual void animationComplete();
        virtual void setEnabled(bool);
        Widget* addWidget(WidgetType, const char*, const char*, float, float);
        Widget* getWidget(unsigned int);

    protected:
        osg::MatrixTransform *m_transform;
        MenuBounds m_bounds;
        MenuManager *m_menuManager;
        osg::Uniform *m_uniform;
        AnimationType m_currentAnimType;
        std::vector<Widget*> m_widgetList;
};

class MenuStart: public Menu
{
    public:
        MenuStart(MenuManager*);
        virtual void createMenu();             
};

class MenuLevelFailed : public Menu
{
    public:
        MenuLevelFailed(MenuManager*);
        virtual void createMenu();
};

class MenuSceneHud: public Menu
{
    public:
        MenuSceneHud(MenuManager*);
        virtual void createMenu();
        void updateScore(int);    
    protected:
       osg::ref_ptr <Widget> m_score;
};

#endif

