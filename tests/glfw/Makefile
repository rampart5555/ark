

OSG_INCLUDE := $(HOME)/g_dev/gmengine/src/OpenSceneGraph/include
OSG_GLES2_PLATFORM_INCLUDE := $(HOME)/g_dev/gmengine/builds/debug_static_gles2/include
OSG_GLES2_LIB_PATH := $(HOME)/g_dev/gmengine/builds/debug_static_gles2/lib
OSG_STATIC_LIB=$(OSG_GLES2_LIB_PATH)/libosgdb_freetyped.a\
    $(OSG_GLES2_LIB_PATH)/libosgParticled.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_jpegd.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_osgd.a\
    $(OSG_GLES2_LIB_PATH)/libosgViewerd.a\
    $(OSG_GLES2_LIB_PATH)/libosgGAd.a\
    $(OSG_GLES2_LIB_PATH)/libosgTextd.a\
    $(OSG_GLES2_LIB_PATH)/libosgUtild.a\
    $(OSG_GLES2_LIB_PATH)/libosgDBd.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_serializers_osgd.a\
    $(OSG_GLES2_LIB_PATH)/libosgd.a\
    $(OSG_GLES2_LIB_PATH)/libOpenThreadsd.a    

OSG_DYNAMIC_LIB_PATH = $(HOME)/g_dev/gmengine/builds/debug_dynamic_gles2/lib
OSG_DYNAMIC_LIBS = -losgGAd -losgd -lOpenThreadsd -losgUtild -losgViewerd -losgDBd -losgTextd -losgParticled -losgAnimationd
SHARED_LIB = -ldl -lGLESv2 -lEGL -lX11 -lXrandr -lpthread -lXxf86vm -lXcursor -lXinerama

#GLFW_INCLUDE=/home/dcioata/g_dev/libs/glfw-3.2.1/include 
GLFW_LIB=$(HOME)/g_dev/gmengine/local/lib/libglfw3.a 
GLFW_INCLUDE:=$(HOME)/g_dev/gmengine/local/include

#LOCAL_SRC_FILES:=\
#    glfw_demo.cpp\
#    OsgMain.cpp\
#    Widget.cpp\
#    MouseHandler.cpp\

LOCAL_OBJ_FILES:=\
    glfw_demo.o\
    OsgMain.o\
    Widget.o\
    MouseHandler.o\
    Callbacks.o\
    scenes/Scene.o\
    scenes/Simple.o\
    scenes/Particle.o\
    scenes/Alpha.o\
    scenes/PSystem.o\
    scenes/Userdata.o\
    scenes/Animation.o
    


LOCAL_SRC_INCLUDE= -I. -I$(GLFW_INCLUDE) -I$(GLFW_INCLUDE) -I$(OSG_INCLUDE) -I$(OSG_GLES2_PLATFORM_INCLUDE)
LOCAL_SRC_LIB = $(GLFW_LIB) $(OSG_STATIC_LIB)
 
all: dynamic_gles2
	
static_gles2:$(LOCAL_OBJ_FILES)
	g++ -Wall -g -ggdb $(LOCAL_SRC_INCLUDE) -o main $(LOCAL_OBJ_FILES) $(LOCAL_SRC_LIB) $(SHARED_LIB)
	
dynamic_gles2:$(LOCAL_OBJ_FILES)
	g++ -Wall -g -ggdb $(LOCAL_SRC_INCLUDE) -L$(OSG_DYNAMIC_LIB_PATH) -o main $(LOCAL_OBJ_FILES) $(GLFW_LIB) $(OSG_DYNAMIC_LIBS) $(SHARED_LIB) 

# test compile issues	
%.o: %.cpp     
	g++ -c -o $@ $< -Wall -ggdb $(LOCAL_SRC_INCLUDE) 

clean:
	rm main $(LOCAL_OBJ_FILES)

#export LD_LIBRARY_PATH=$HOME/g_dev/gmengine/builds/debug_dynamic_gles2/lib
    
