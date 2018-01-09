OSG_INCLUDE := $(HOME)/g_dev/gmengine/src/OpenSceneGraph/include

OSG_GLES2_PLATFORM_INCLUDE := $(HOME)/g_dev/gmengine/builds/debug_static_gles2/include
OSG_GLES2_LIB_PATH := $(HOME)/g_dev/gmengine/builds/debug_static_gles2/lib

OSG_CORE_PLATFORM_INCLUDE := $(HOME)/g_dev/gmengine/builds/debug_dynamic/include
OSG_CORE_LIB_PATH := $(HOME)/g_dev/gmengine/builds/debug_dynamic/lib
OSG_CORE_DYN_LIB := -losgViewerd  -losgGAd -losgTextd -losgUtild -losgDBd -losgd -lOpenThreadsd 

GLFW_LIB=$(HOME)/g_dev/gmengine/local/lib/libglfw3.a
SDL_LIB=$(HOME)/g_dev/gmengine/local/lib/libSDL2.a
JPEG_LIB=$(HOME)/g_dev/gmengine/local/lib/libjpeg.a
FREETYPE_LIB=$(HOME)/g_dev/gmengine/local/lib/libfreetype.a
LUA_LIB=$(HOME)/g_dev/gmengine/local/lib/liblua.a
BOX2D_LIB=$(HOME)/g_dev/gmengine/local/lib/libBox2D.a


OSG_STATIC_LIB=\
    $(OSG_GLES2_LIB_PATH)/libosgdb_freetyped.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_jpegd.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_osgd.a\
    $(OSG_GLES2_LIB_PATH)/libosgViewerd.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_serializers_osganimationd.a\
    $(OSG_GLES2_LIB_PATH)/libosgAnimationd.a\
    $(OSG_GLES2_LIB_PATH)/libosgGAd.a\
    $(OSG_GLES2_LIB_PATH)/libosgTextd.a\
    $(OSG_GLES2_LIB_PATH)/libosgUtild.a\
    $(OSG_GLES2_LIB_PATH)/libosgDBd.a\
    $(OSG_GLES2_LIB_PATH)/libosgdb_serializers_osgd.a\
    $(OSG_GLES2_LIB_PATH)/libosgd.a\
    $(OSG_GLES2_LIB_PATH)/libOpenThreadsd.a

#SDL

LOCAL_SRC_INCLUDE = -I. -I$(HOME)/g_dev/gmengine/local/include -I$(OSG_INCLUDE) -I$(OSG_GLES2_PLATFORM_INCLUDE)
LOCAL_SRC_LIB = $(OSG_STATIC_LIB) $(BOX2D_LIB) $(JPEG_LIB) $(FREETYPE_LIB) $(LUA_LIB) $(GLFW_LIB)

LOCAL_SRC_LIB_CORE = $(BOX2D_LIB) $(SDL_LIB) $(JPEG_LIB) $(FREETYPE_LIB)

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
    scene/SceneAnimation.o\
    scene/LevelManager.o\
    scene/Entity.o\
    scene/EntityBall.o\
    scene/EntityBullet.o\
    scene/EntityPaddle.o\
    scene/EntityManager.o
    
SHARED_LIB = -ldl -lGLESv2 -lEGL -pthread -lz -lX11 -lXrandr -lXxf86vm -lXcursor -lXinerama


all:debug_gles2
#all: $(OBJ)

debug_core:
	g++  -Wall -g -ggdb $(LOCAL_SRC_INCLUDE) -I$(OSG_CORE_PLATFORM_INCLUDE) -L$(OSG_CORE_LIB_PATH) $(LOCAL_SRC_FILES)  -o main  \
	$(LOCAL_SRC_LIB_CORE) $(SHARED_LIB) $(OSG_CORE_DYN_LIB)
	
debug_gles2:$(LOCAL_OBJ_FILES) test_lua	
	g++ -Wall -g -ggdb $(LOCAL_SRC_INCLUDE) -o main $(LOCAL_OBJ_FILES) $(LOCAL_SRC_LIB) $(SHARED_LIB)

# test compile issues	
%.o: %.cpp 
	g++ -c -o $@ $< -Wall -ggdb $(LOCAL_SRC_INCLUDE) 
test_lua:
	$(HOME)/g_dev/gmengine/local/bin/lua ../assets/lua/menudef.lua	
clean:
	rm -f *.o scene/*.o ui/*.o
	
	

    
