#ifndef __TESTLIB_H
#define __TESTLIB_H
#include"JniHelper.h"
#include"JniObject.h"

void testStrfunc(JNIEnv* env,jobject obj,jstring str,jint ivalue);
jstring getString(JNIEnv* env,jobject obj);
jobject getObject(JNIEnv* env,jobject obj);
void testCalljava(JNIEnv* env,jobject obj);
void testCallStaticjava(JNIEnv* env,jobject obj);
void testListener(JNIEnv* env,jobject obj,jobject listener);
#endif
