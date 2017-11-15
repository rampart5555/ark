#include <string.h>
#include <jni.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/keycodes.h>
#include "OsgMain.h"
#include "Logging.h"

#define  LOG_TAG    "OsgNativeLib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

OsgMain mainApp;
static JavaVM* mJavaVM;
static jobject mAssetsManager;


extern "C" {
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_mouseButtonPressEvent(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint button);
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_mouseButtonReleaseEvent(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint button);
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_mouseMoveEvent(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_keyboardDown(JNIEnv * env, jobject obj, jint key);
    JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_readAssets(JNIEnv *env, jobject thiz, jobject assetManager);
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);    
};

/* Library init */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{

    JNIEnv *env;
    mJavaVM = vm;
    LOGI("JNI_OnLoad called");
    if (mJavaVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("Failed to get the environment using GetEnv()");
        return -1;
    }
    return JNI_VERSION_1_4;

}

    
JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_init(JNIEnv * env, jobject obj, jint width, jint height)
{
    runLoggingThread();
    mainApp.init(0,0,width,height);
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_step(JNIEnv * env, jobject obj)
{
    mainApp.draw();
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_mouseButtonPressEvent(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint button)
{
    mainApp.mouseButtonPress(x,y,button);
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_mouseButtonReleaseEvent(JNIEnv * env, jobject obj, jfloat x, jfloat y, jint button)
{
    mainApp.mouseButtonRelease(x,y,button);
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_mouseMoveEvent(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
    mainApp.mouseMotion(x,y);
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_keyboardDown(JNIEnv * env, jobject obj, jint key)
{
    if(key == AKEYCODE_S)
        mainApp.keyPress('s');
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_keyboardUp(JNIEnv * env, jobject obj, jint key)
{
    
}

JNIEXPORT void JNICALL Java_com_osg_ark_OsgNativeLib_readAssets(JNIEnv *env, jobject thiz, jobject assetManager)
{
    LOGI("Java_com_osg_ark_OsgNativeLib_addAsset\n");
    mAssetsManager = env->NewGlobalRef(assetManager);
#if 0
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AAsset* asset = AAssetManager_open(mgr, "scene_models.osgt", AASSET_MODE_UNKNOWN);
    if(asset != NULL)
    {
        size_t asset_len = AAsset_getLength(asset);
        LOGI("Asset file size: %u\n", asset_len);
        char* buffer = (char*) malloc(asset_len + 1);
        AAsset_read(asset, buffer, asset_len);
        buffer[asset_len] = 0;

        //__android_log_print(ANDROID_LOG_INFO, "Test Asset Manager", "The value is %s", buffer);
        //mainApp.addModel(buffer,asset_len);
        AAsset_close(asset);
        free(buffer);
    }
    else
    {
        LOGI("Test Asset Manager:%s\n", "Cannot open file");
    }
#endif    
}
#if 1
/* 
Read more about threading and 
pthread_setspecific(mThreadKey, (void*) env);

*/
char* readBinaryFile(const char *filename, unsigned int *filesize)
{
    JNIEnv *env;
    LOGI("readFile\n");
    if(mJavaVM==NULL)
    {
        LOGE("mJavaVM is NULL");
        return NULL;
    }
    if(mAssetsManager==NULL)
    {
        LOGE("mAssetsManager is NULL");
        return NULL;
    }
    
    int status = mJavaVM->AttachCurrentThread(&env,NULL);
    if(status < 0)
    {
        LOGE("failed to attach current thread");
        return NULL;
    }
    AAssetManager* mgr = AAssetManager_fromJava(env, mAssetsManager);
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
    char* buffer;
    if(asset != NULL)
    {
        LOGI("Found assets: %s\n", filename);
        size_t asset_len = AAsset_getLength(asset);
        LOGI("Asset file size: %u\n", asset_len);
        buffer = (char*) malloc(asset_len + 1);
        AAsset_read(asset, buffer, asset_len);
        buffer[asset_len] = 0;
        *filesize = asset_len;
        AAsset_close(asset);        
    }
    else
    {
        LOGE("Asset not found:%s\n",filename);
    }
    return buffer;    
}

#endif







