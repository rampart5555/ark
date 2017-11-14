
#include "Widget.h"
#include "Config.h"
#include "MouseHandler.h"



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
            /* 
                ret_code if handle is handled or not for show statsistics
                ret code showuld be false;
            */
            bool ret_code = false;            
            if(w == NULL)
                printf("Inavlid widget\n");
            else                            
                ret_code = w->runCallback(); 
            return ret_code;
        }
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
                    if(hitr->drawable.valid())
                    {                        
                        printf("Scene handle event: %s \n",  hitr->drawable->getName().c_str());
                        osg::Vec3f world_int = hitr->getWorldIntersectPoint();
                        Event_XY event_xy;
                        event_xy.x = world_int.x();
                        event_xy.y = world_int.y();
                        //Scene_paddle_select(&event_xy);                        
                        
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
                            return true;
                            //Scene_paddle_move(&event_xy);       
                            break;
                        }
                    }
                }

            }                        
            return false;
        }
        case(osgGA::GUIEventAdapter::RELEASE):
            //EngineManager::instance().handleEvent(SCENE_PADDLE_UNSELECT, NULL);
            //Scene_paddle_unselect(NULL);
            return false;
        default:
            break;
    }
    return false;
}

bool MouseHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{ 
    if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
    {        
        if(handleMenuEvent(ea,aa)==true)
            return true;
        if(handleSceneEvent(ea,aa) == true)
            return true;
    }
#if 0    
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
#endif    
    return false;
}

