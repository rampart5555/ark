#include <stdlib.h>
#include <stdio.h>
//#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include "OsgMain.h"

OsgMain &osg_main = OsgMain::instance();

double xpos = 0, ypos = 0;
int mouse_drag;   

char* osg_read_file(const char *filename, unsigned int *file_len)
{
    char *buffer = 0;
    long length;
    FILE *fp = fopen (filename, "rb");

    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = (char*)malloc (length);
        fread (buffer, 1, length, fp);
        fclose (fp);
    }
    *file_len = length;
    return buffer;
}    

osg::Node* osg_create_model()
{
    char *buf = NULL;
    unsigned int buf_size = 0;
        
    buf = osg_read_file("model.osgt", &buf_size);
    
    if(buf==NULL)
        return NULL;
        
    std::stringstream ss(std::string(reinterpret_cast<char const*>(buf), buf_size)); 
    
    osgDB::ReaderWriter *wr = osgDB::Registry::instance()->getReaderWriterForExtension(std::string("osgt"));    
    if(wr==NULL)
    {
        printf("AssetsManager::loadObject=> Invalid plugin for extension %s\n","osgt");
        return NULL;
    }
    osgDB::ReaderWriter::ReadResult rr = wr->readObject(ss);        
    
    if (rr.validObject())
    {         
    }
    if (rr.error()) 
    {
        printf("%s\n", rr.message().c_str());
        return NULL;
    }
    if (rr.notEnoughMemory()) 
    {
        printf("AssetsManager::loadObject=> Not enought memory to load file\n");
        return NULL;
        
    }  
    return  dynamic_cast<osg::Node*>(rr.takeObject());  
}

static void osg_mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x,y;
    glfwGetCursorPos(window, &x, &y);    
    if ((button == GLFW_MOUSE_BUTTON_LEFT) && action == GLFW_PRESS)
    {                
        //printf("Mouse PRESS: (%f,%f)\n",xpos,ypos);
        
        osg_main.mouseButtonPress( (int)x, (int)y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON );
        mouse_drag=1;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        osg_main.mouseButtonRelease( (int)x, (int)y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON );
        //printf("Mouse release: (%f,%f)\n",xpos,ypos);
        mouse_drag=0;
    }
}

static void osg_cursor_callback(GLFWwindow* window, double x, double y)
{
    if(mouse_drag==1)
    {
        osg_main.mouseMotion(x,y);
    }
    xpos=x;
    ypos=y;
}

static void osg_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        osg_main.cleanup();
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else
    {   
        if(key=='S')     
            osg_main.keyPress( 's');
        else
            osg_main.keyPress(key);
    }
}

static void reshape_callback(GLFWwindow* window, int width, int height)
{
    osg_main.reshape(width,height);
}

int main(void)
{
    GLFWwindow* window;
    int init_width = 480;
    int init_height = 680;
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
        
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(init_width, init_height, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetCursorPosCallback(window, osg_cursor_callback);
    glfwSetMouseButtonCallback(window, osg_mouse_callback);
    glfwSetKeyCallback(window, osg_key_callback);
    glfwSetFramebufferSizeCallback(window, reshape_callback);
    
    glfwMakeContextCurrent(window);      
    glfwSwapInterval(1);
        
    osg_main.init(0,0, init_width, init_height);    


    while (!glfwWindowShouldClose(window))
    {        
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);        
        //glViewport(0, 0, width, height);
        //glClearColor(0.0,0.0,1.0,1.0);
        //glClear(GL_COLOR_BUFFER_BIT);   
        osg_main.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    exit(EXIT_SUCCESS);

}
