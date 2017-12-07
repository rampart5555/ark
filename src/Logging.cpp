
#include "Logging.h"

#ifdef __ANDROID__

static int pfd[2];
static pthread_t loggingThread;

static void *loggingFunction(void*) {
    ssize_t readSize;
    char buf[128];

    while((readSize = read(pfd[0], buf, sizeof buf - 1)) > 0) {
        if(buf[readSize - 1] == '\n') {
            --readSize;
        }

        buf[readSize] = 0;  // add null-terminator

        __android_log_write(ANDROID_LOG_DEBUG, LOG_TAG, buf); // Set any log level you want
    }

    return 0;
}

int runLoggingThread() { // run this function to redirect your output to android log
    setvbuf(stdout, 0, _IOLBF, 0); // make stdout line-buffered
    setvbuf(stderr, 0, _IONBF, 0); // make stderr unbuffered

    /* create the pipe and redirect stdout and stderr */
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);

    /* spawn the logging thread */
    if(pthread_create(&loggingThread, 0, loggingFunction, 0) == -1) {
        return -1;
    }

    pthread_detach(loggingThread);

    return 0;
}
#else
static LogLevel debug_log_level = LOG_INFO;
int runLoggingThread()
{
    return 0;
}

void debug_log(LogLevel level, const char *fmt, ...)
{    
    if(level > debug_log_level)
        return;
    if(level==LOG_ERROR) printf("%s", "ERROR: ");
    if(level==LOG_WARN) printf("%s", "WARN: ");
    if(level==LOG_INFO) printf("%s", "INFO: ");
    if(level==LOG_DEBUG) printf("%s", "DEBUG: ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
#endif
