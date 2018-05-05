#ifndef LOGUTIL_H
#define LOGUTIL_H

#ifdef __ANDROID__
#include<android/log.h>
#define JNITAG "testlib"
#else
#include<stdio.h>
#endif

#define __DEBUG__
#define LOGFUNC //打印文件

#ifdef __DEBUG__
    #ifdef __ANDROID__
        #ifdef LOGFUNC
            #define LOGE(fmt,...) ((void)__android_log_print(ANDROID_LOG_ERROR,JNITAG ,"[%s %s %d], " fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__))
            #define LOGE_T(tag,fmt,...) ((void)__android_log_print(ANDROID_LOG_ERROR, tag ,"[%s %s %d], " fmt,__FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__))
        #else
            #define LOGE(fmt,...) ((void)__android_log_print(ANDROID_LOG_ERROR,JNITAG ,fmt,##__VA_ARGS__))
            #define LOGE_T(tag,fmt,...) ((void)__android_log_print(ANDROID_LOG_ERROR, tag,fmt , ##__VA_ARGS__))
        #endif
    #else
        #ifdef LOGFUNC
            #define LOGE(fmt,...)       printf("[%s %s %d], " fmt "\n",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
            #define LOGE_T(tag,fmt,...) printf("[%s %s %d], " tag fmt "\n",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
        #else
            #define LOGE(fmt,...)       printf(fmt,##__VA_ARGS__)
            #define LOGE_T(tag,fmt,...) printf(tag fmt,##__VA_ARGS__)
        #endif
    #endif
#else
    #define LOGE_T(tag,fmt,...)
    #define LOGE(...)
#endif
    #ifdef __ANDROID__
        #define LOG_ABORT(fmt,...) ((void)__android_log_print(ANDROID_LOG_ERROR,JNITAG ,"[%s %s %d], " fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__))
    #else
        #define LOG_ABORT(fmt,...)  printf(fmt,##__VA_ARGS__)
    #endif
#endif
