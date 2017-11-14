#ifndef __MOUSE_HANDLER__
#define __MOUSE_HANDLER__
#include <osgViewer/ViewerEventHandlers>
class MouseHandler : public osgGA::GUIEventHandler 
{
    public:
        MouseHandler(){}
        bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
        bool handleMenuEvent(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
        bool handleSceneEvent(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
};

#endif
