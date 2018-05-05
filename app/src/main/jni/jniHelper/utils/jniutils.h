#ifndef __JNIUTILS__
#define __JNIUTILS__
#include<android/log.h>
#include"../JniGlobalDef.h"
#define LOG_TAG "JniHelper"
#ifdef __DEBUG__
#define JniLOGE(fmt,...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG,"[%s %s %ld] " fmt,__FILE__,__FUNCTION__,(long)__LINE__, ##__VA_ARGS__))
#else
#define JniLOGE(fmt,...)
#endif

#define JniLOGD(...)
#define JniLOGI(...)
void transJavaClassToJniClass(const char* src, char* to);
void transJavaClassToSimpleClassname(const char* src,char* to);
#endif
