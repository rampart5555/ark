#ifndef __LOGGING__
#define __LOGGING__

#ifdef __ANDROID__
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#define  LOG_TAG    "OsgNativeLib"
#define  LOG_DEBUG(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOG_INFO(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOG_STATE(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOG_WARN(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOG_ERROR(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#else
#include <stdio.h>
#include <stdarg.h>
typedef enum
{
    LOG_ERROR,
    LOG_WARN,
    LOG_STATE,
    LOG_INFO,
    LOG_DEBUG    
}LogLevel;
void debug_log(LogLevel , const char *, ...);

#define LOG_ERROR(fmt, ...) debug_log(LOG_ERROR,fmt,__VA_ARGS__)
#define LOG_WARN(fmt, ...)  debug_log(LOG_WARN,fmt, __VA_ARGS__)
#define LOG_STATE(fmt,...)  debug_log(LOG_STATE,fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)  debug_log(LOG_INFO,fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...) debug_log(LOG_DEBUG,fmt,__VA_ARGS__)

#endif
int runLoggingThread();

#endif
