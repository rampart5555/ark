#include <stdio.h>
#include "Callbacks.h"
#include "OsgMain.h"
/* return false in mouse handler for show_statistics callback*/
bool show_statistics(void *args)
{
    printf("Callback: show_statistics\n");
    OsgMain::instance().keyPress((osgGA::GUIEventAdapter::KeySymbol)'s');    
    return true;
}

bool play_animation(void *args)
{
    printf("Callback: play_animation\n");
    Scene *scene = OsgMain::instance().getCurrentScene();
    if(scene==NULL)
    {
        printf("Null scene\n");
        return false;
    }
    scene->handleSceneEvent();
    return true;
}

