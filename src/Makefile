
#BUILD_DIR=debug_static_gles2
BUILD_DIR=debug_dynamic_gles2
OSG_INCLUDE := $(HOME)/g_dev/gmengine/src/OpenSceneGraph/include
OSG_PLATFORM_INCLUDE := $(HOME)/g_dev/gmengine/builds/$(BUILD_DIR)/include
OSG_LIB_PATH := $(HOME)/g_dev/gmengine/builds/$(BUILD_DIR)/lib
OSG_STATIC_LIB=\
    $(OSG_LIB_PATH)/libosgdb_freetyped.a\
    $(OSG_LIB_PATH)/libosgdb_jpegd.a\
    $(OSG_LIB_PATH)/libosgdb_osgd.a\
    $(OSG_LIB_PATH)/libosgViewerd.a\
    $(OSG_LIB_PATH)/libosgdb_serializers_osganimationd.a\
    $(OSG_LIB_PATH)/libosgAnimationd.a\
    $(OSG_LIB_PATH)/libosgGAd.a\
    $(OSG_LIB_PATH)/libosgTextd.a\
    $(OSG_LIB_PATH)/libosgUtild.a\
    $(OSG_LIB_PATH)/libosgDBd.a\
    $(OSG_LIB_PATH)/libosgdb_serializers_osgd.a\
    $(OSG_LIB_PATH)/libosgd.a\
    $(OSG_LIB_PATH)/libOpenThreadsd.a

OSG_DYNAMIC_LIB= -losgAnimationd -losgDBd -losgGAd -losgTextd -losgViewerd -losgd -lOpenThreadsd   

GLFW_LIB=$(HOME)/g_dev/gmengine/local/lib/libglfw3.a
SDL_LIB=$(HOME)/g_dev/gmengine/local/lib/libSDL2.a
JPEG_LIB=$(HOME)/g_dev/gmengine/local/lib/libjpeg.a
FREETYPE_LIB=$(HOME)/g_dev/gmengine/local/lib/libfreetype.a
LUA_LIB=$(HOME)/g_dev/gmengine/local/lib/liblua.a
BOX2D_LIB=$(HOME)/g_dev/gmengine/local/lib/libBox2D.a

NON_OSG_STATIC_LIB = $(BOX2D_LIB) $(JPEG_LIB) $(FREETYPE_LIB) $(LUA_LIB) $(GLFW_LIB)

LOCAL_SRC_INCLUDE = -I. -I$(HOME)/g_dev/gmengine/local/include -I$(OSG_INCLUDE) -I$(OSG_PLATFORM_INCLUDE)


SHARED_LIB = -ldl -lGLESv2 -lEGL -pthread -lz -lX11 -lXrandr -lXxf86vm -lXcursor -lXinerama

LOCAL_SRC_FILES:=main.cpp\
    AssetsManager.cpp\
    AssetsManagerLua.cpp\
    Logging.cpp\
    MouseHandler.cpp\
    EngineCallback.cpp\
    ui/Animation.cpp\
    ui/Menu.cpp\
    ui/MenuManager.cpp\
    ui/Widget.cpp\
    scene/Scene.cpp\
    scene/SceneData.cpp\
    scene/Entity.cpp\
    scene/EntityBall.cpp\
    scene/EntityBullet.cpp\
    scene/EntityPaddle.cpp\
    scene/EntityManager.cpp\
    

LOCAL_OBJ_FILES:=main.o\
    Utils.o\
    Logging.o\
    OsgMain.o\
    AssetsManager.o\
    AssetsManagerLua.o\
    MouseHandler.o\
    EngineCallback.o\
    OsgNativeLib.o\
    ui/Animation.o\
    ui/Menu.o\
    ui/MenuManager.o\
    ui/Widget.o\
    scene/Scene.o\
    scene/LevelManager.o\
    scene/Entity.o\
    scene/EntityBall.o\
    scene/EntityBullet.o\
    scene/EntityPaddle.o\
    scene/EntityManager.o\
    scene/EntityAnimation.o
    

all:debug_dynamic_gles2
#all: $(OBJ)
	
debug_static_gles2:$(LOCAL_OBJ_FILES) test_lua	
	g++ -Wall -g -ggdb $(LOCAL_SRC_INCLUDE) -o main \
        $(LOCAL_OBJ_FILES) $(OSG_STATIC_LIB) $(NON_OSG_STATIC_LIB) $(SHARED_LIB)

debug_dynamic_gles2:$(LOCAL_OBJ_FILES) 
	g++ -Wall -g -ggdb $(LOCAL_SRC_INCLUDE) -L$(OSG_LIB_PATH) -o main \
        $(LOCAL_OBJ_FILES) $(NON_OSG_STATIC_LIB) $(OSG_DYNAMIC_LIB) $(SHARED_LIB)    

# test compile issues	
%.o: %.cpp 
	g++ -c -o $@ $< -Wall -ggdb $(LOCAL_SRC_INCLUDE) 
test_lua:
	$(HOME)/g_dev/gmengine/local/bin/lua ../assets/lua/menudef.lua	
clean:
	rm -f *.o scene/*.o ui/*.o
#export LD_LIBRARY_PATH=$HOME/g_dev/gmengine/builds/debug_dynamic_gles2/lib	
	

    
