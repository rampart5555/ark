#include "OsgMain.h"
int main(void)
{
    OsgMain osg_main;
    osg_main.init(0,0,640,480);
    osg_main.addDemoScene();
    osg_main.run();
}
