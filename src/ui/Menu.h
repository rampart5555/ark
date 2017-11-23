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
        virtual Widget* addWidget(const char*, const char*, float, float);
    protected:
        osg::MatrixTransform *m_transform;
        MenuBounds m_bounds;
        MenuManager *m_menuManager;
        osg::Uniform *m_uniform;
        AnimationType m_currentAnimType;
};

class MenuEpisode: public Menu
{
    public:
        MenuEpisode(MenuManager*);
        virtual void createMenu();
        virtual Widget* addWidget(const char*, const char*, float, float );        
        void setEpisodeId(int);
        int  getEpisodeId();
        void loadShaders();
        void loadWidgets();
        void setPosition(float x, float y, float z);
        
    protected:        
        int m_episodeId;
        osg::Vec3f m_position;

};

class MenuStart: public Menu
{
    public:
        MenuStart(MenuManager*);
        virtual void createMenu();             
};

class MenuLevelSelect : public Menu
{
    public:
        MenuLevelSelect(MenuManager*);
        virtual void createMenu();        
        void nextEpisode();
        void prevEpisode();
        virtual void show();
        virtual void hide(EngineCallback, void*);
    protected:
        std::vector <MenuEpisode*> m_menuList;
        int m_menuIndex;
        osg::Vec3f m_position;
};

class MenuLevelComplete : public Menu
{
    public:
        MenuLevelComplete(MenuManager*);
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

