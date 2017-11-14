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

