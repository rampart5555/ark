OSG_BUILD_TYPE=debug_dynamic_gles2
#OSG_BUILD_TYPE=debug_dynamic
OSG_INCLUDE=$HOME/g_dev/gmengine/src/OpenSceneGraph/include
OSG_PLATFORM_INCLUDE=$HOME/g_dev/gmengine/builds/$OSG_BUILD_TYPE/include
OSG_LIB_PATH=$HOME/g_dev/gmengine/builds/$OSG_BUILD_TYPE/lib

OSG_DYNAMIC_LIBS="-losgGAd -losgd -lOpenThreadsd -losgUtild -losgViewerd -losgDBd -losgTextd -losgParticled -losgAnimationd -losgShadowd"
#SHARED_LIB="-ldl -lGL -lX11 -lXrandr -lpthread -lXxf86vm -lXcursor -lXinerama"
SHARED_LIB="-ldl -lGLESv2 -lEGL -lX11 -lXrandr -lpthread -lXxf86vm -lXcursor -lXinerama"
g++ -Wall -g -ggdb -I$OSG_INCLUDE -I$OSG_PLATFORM_INCLUDE -L$OSG_LIB_PATH osgshadow.cpp ArkShadowMap.cpp -o main $OSG_DYNAMIC_LIBS $SHARED_LIB

#export LD_LIBRARY_PATH=$HOME/g_dev/gmengine/builds/debug_dynamic/lib
#export LD_LIBRARY_PATH=$HOME/g_dev/gmengine/builds/debug_dynamic_gles2/lib
