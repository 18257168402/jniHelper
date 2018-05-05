
#include"JniMethod.h"
#include"JniObject.h"
#include"JniClass.h"
#include"./utils/jniutils.h"
void exceptionObjClear(JniObject* jniobj){
    if(jniobj->getEnv()->ExceptionCheck()){
            jniobj->getEnv()->ExceptionDescribe();
            jniobj->getEnv()->ExceptionClear();
    }
}
JniMethod::JniMethod(JniClass* clazz, jmethodID mid, const char* methodname, const char* sig){
    mTargetClass=clazz;
    mMid=mid;
	int len = strlen(methodname);
	mMethodname = (char*)malloc(len + 1);
	memset(mMethodname, 0, len + 1);
	strcpy(mMethodname, methodname);
	len = strlen(sig);
	mSig = (char*)malloc(len + 1);
	memset(mSig, 0, len + 1);
	strcpy(mSig, sig);
}
JniMethod::~JniMethod(){
	if (mMethodname){
		free(mMethodname);
		mMethodname = NULL;
	}
	if (mSig){
		free(mSig);
		mSig = NULL;
	}
}
jmethodID JniMethod::getMid(){
    return mMid;
}
void JniMethod::exceptionClear(bool* isExc){
    if(isExc==NULL){
        return;
    }
    if(mTargetClass->getEnv()->ExceptionCheck()){
        mTargetClass->getEnv()->ExceptionDescribe();
        mTargetClass->getEnv()->ExceptionClear();
        *isExc=true;
        return;
    }
     *isExc=false;
}
#define ConstructImp(Clazz) Clazz::Clazz(JniClass* targetObj, \
    jmethodID mid,const char* methodname, const char* sig):\
    JniMethod(targetObj, mid, methodname, sig){}
#define CallImp(Clazz,RetType,CallType) RetType Clazz::call(JniObject* targetObj,bool *isExceptionOccur, va_list args){\
    JNIEnv* env= targetObj->getEnv();\
    RetType ires= (RetType)env->Call##CallType##MethodV(targetObj->obj(),mMid,args);\
    exceptionClear(isExceptionOccur);\
    return ires;\
}
#define CallStaticImp(Clazz,RetType,CallType) RetType Clazz::call(bool *isExceptionOccur, ...){\
    JNIEnv* env= mTargetClass->getEnv();\
    va_list vl;\
    va_start(vl, isExceptionOccur);\
    RetType ires= (RetType)env->CallStatic##CallType##MethodV(mTargetClass->getJClass(),mMid,vl);\
    va_end(vl);exceptionClear(isExceptionOccur);\
    return ires;\
}

ConstructImp(JniIntMethod)
CallImp(JniIntMethod,jint,Int)
ConstructImp(JniBooleanMethod)
CallImp(JniBooleanMethod,jboolean,Boolean)
ConstructImp(JniObjectMethod)
CallImp(JniObjectMethod,jobject,Object)
ConstructImp(JniLongMethod)
CallImp(JniLongMethod,jlong,Long)
ConstructImp(JniShortMethod)
CallImp(JniShortMethod,jshort,Short)
ConstructImp(JniCharMethod)
CallImp(JniCharMethod,jchar,Char)
ConstructImp(JniFloatMethod)
CallImp(JniFloatMethod,jfloat,Float)
ConstructImp(JniDoubleMethod)
CallImp(JniDoubleMethod,jdouble,Double)
ConstructImp(JniByteMethod)
CallImp(JniByteMethod,jbyte,Byte)


ConstructImp(JniStaticIntMethod)
CallStaticImp(JniStaticIntMethod,jint,Int)
ConstructImp(JniStaticBooleanMethod)
CallStaticImp(JniStaticBooleanMethod,jboolean,Boolean)
ConstructImp(JniStaticObjectMethod)
CallStaticImp(JniStaticObjectMethod,jobject,Object)
ConstructImp(JniStaticLongMethod)
CallStaticImp(JniStaticLongMethod,jlong,Long)
ConstructImp(JniStaticShortMethod)
CallStaticImp(JniStaticShortMethod,jshort,Short)
ConstructImp(JniStaticCharMethod)
CallStaticImp(JniStaticCharMethod,jchar,Char)
ConstructImp(JniStaticFloatMethod)
CallStaticImp(JniStaticFloatMethod,jfloat,Float)
ConstructImp(JniStaticDoubleMethod)
CallStaticImp(JniStaticDoubleMethod,jdouble,Double)
ConstructImp(JniStaticByteMethod)
CallStaticImp(JniStaticByteMethod,jbyte,Byte)

ConstructImp(JniVoidMethod)
void JniVoidMethod::call(JniObject* targetObj,bool* isExceptionOccur, va_list args){
    JNIEnv* env= targetObj->getEnv();
    env->CallVoidMethodV(targetObj->obj(),mMid,args);
    exceptionClear(isExceptionOccur);
    return;
}
ConstructImp(JniStaticVoidMethod)
void JniStaticVoidMethod::call(bool* isExceptionOccur, ...){
    JNIEnv* env= mTargetClass->getEnv();
    va_list vl;
    va_start(vl, isExceptionOccur);
    env->CallStaticVoidMethodV(mTargetClass->getJClass(),mMid,vl);
    va_end(vl);
    exceptionClear(isExceptionOccur);
    return;
}
