
#include "Config.h"
#include "ui/Widget.h"
#include "MouseHandler.h"
#include "scene/Scene.h"

bool MouseHandler::handleMenuEvent(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&aa);            
    osgUtil::LineSegmentIntersector::Intersections intersections;

    float x = (ea.getX()-ea.getXmin())/(ea.getXmax()-ea.getXmin())*static_cast<float>(ea.getWindowWidth());
    float y = (ea.getY()-ea.getYmin())/(ea.getYmax()-ea.getYmin())*static_cast<float>(ea.getWindowHeight());
    if (ea.getMouseYOrientation()==osgGA::GUIEventAdapter::Y_INCREASING_DOWNWARDS) y = static_cast<float>(ea.getWindowHeight())-y;
    printf("MouseHandler::handleMenuEvent (%f, %f)\n",x,y);
    osg::Camera* camera = viewer->getCamera();
    osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(camera->getGraphicsContext());
    if (gw)
    {
        viewer->computeIntersections(camera, osgUtil::Intersector::WINDOW, x, y, intersections, MASK_2D);
    }

    if (!intersections.empty())
    {
        osgUtil::LineSegmentIntersector::Intersections::iterator hitr;                
        hitr = intersections.begin();
        if (hitr->drawable.valid())
        {
            printf("Menu handle event: (%f %f) %s \n", x, y, hitr->drawable->getName().c_str());            
            Widget *w  = dynamic_cast<Widget*>(hitr->drawable->getUserData());
            if(w==NULL)
            {
                printf("Inavlid widget\n");
            }
            else
            {
                //w->handle();
                w->runCallback(NULL);
            }
            return true;
        }
    }
    else
    {
        printf("Button not found\n");
    }
    return false;
}

bool MouseHandler::handleSceneEvent(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&aa);  
    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::PUSH):
        {
            osgUtil::LineSegmentIntersector::Intersections intersections;
            if (viewer->computeIntersections(ea,intersections))
            {       
                osgUtil::LineSegmentIntersector::Intersections::iterator hitr;
                for(hitr=intersections.begin();hitr != intersections.end(); ++hitr)
                {
                    if(hitr->drawable.valid() && (hitr->drawable->getName()=="entity_paddle") )
                    {                        
                        printf("Scene handle event: %s \n",  hitr->drawable->getName().c_str());
                        osg::Vec3f world_int = hitr->getWorldIntersectPoint();
                        Event_XY event_xy;
                        event_xy.x = world_int.x();
                        event_xy.y = world_int.y();
                        Scene_paddle_select(&event_xy);                        
                        
                        return true;
                    }
                }
            }
        }
        case (osgGA::GUIEventAdapter::DRAG):
        {
            viewer = dynamic_cast<osgViewer::Viewer*>(&aa);            
            osgUtil::LineSegmentIntersector::Intersections intersections;
            if (viewer->computeIntersections(ea,intersections))
            {
                osgUtil::LineSegmentIntersector::Intersections::iterator hitr;
                for(hitr=intersections.begin();hitr != intersections.end();++hitr)
                {
                    if(hitr->drawable.valid())
                    {                        
                        if( hitr->drawable->getName()=="entity_background" )
                        {                             
                            osg::Vec3f world_int = hitr->getWorldIntersectPoint();
                            Event_XY event_xy;
                            event_xy.x = world_int.x();
                            event_xy.y = world_int.y();
                            Scene_paddle_move(&event_xy);       
                            break;
                        }
                    }
                }

            }                        
            return true;
        }
        case(osgGA::GUIEventAdapter::RELEASE):
            //EngineManager::instance().handleEvent(SCENE_PADDLE_UNSELECT, NULL);
            Scene_paddle_unselect(NULL);
            return true;
        default:
            break;
    }
    return false;
}

bool MouseHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{ 
    if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
    {
        printf("MouseHandler::handle::PUSH \n");
        if(handleMenuEvent(ea,aa)==true)
            return true;
        if(handleSceneEvent(ea,aa) == true)
            return true;
    }
    else if(ea.getEventType() == osgGA::GUIEventAdapter::DRAG)
    {
        if(handleSceneEvent(ea,aa) == true)
            return true;
    }
    else if(ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
    {
        if(handleSceneEvent(ea,aa) == true)
            return true;
    }
    return false;
}

