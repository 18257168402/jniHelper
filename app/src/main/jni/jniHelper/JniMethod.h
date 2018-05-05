#ifndef JNIMETHOD
#define JNIMETHOD
#include<jni.h>
#include<list>
#include<stdlib.h>

using namespace std;



class JniObject;
class JniClass;
void exceptionObjClear(JniObject* jniobj);
class JniMethod{
 public:
    JniMethod(JniClass* clazz,jmethodID mid,const char* methodname,const char* sig);
	virtual ~JniMethod();
    jmethodID getMid();
protected:
    void exceptionClear(bool* isExc);
    JniClass*  mTargetClass;
	jmethodID mMid;   
	char* mMethodname;
	char* mSig;
};
#define MethodDecl(Clazz,RetType) class Clazz :public JniMethod{\
public:\
    Clazz(JniClass* targetObj, jmethodID mid, const char* methodname, const char* sig);\
    RetType call(JniObject* targetObj,bool* isExceptionOccur, va_list args);\
};

MethodDecl(JniVoidMethod,void)
MethodDecl(JniIntMethod,jint)
MethodDecl(JniByteMethod,jbyte)
MethodDecl(JniCharMethod,jchar)
MethodDecl(JniBooleanMethod,jboolean)
MethodDecl(JniLongMethod,jlong)
MethodDecl(JniShortMethod,jshort)
MethodDecl(JniFloatMethod,jfloat)
MethodDecl(JniDoubleMethod,jdouble)
MethodDecl(JniObjectMethod,jobject)


#define StaticMethodDecl(Clazz,RetType) class Clazz :public JniMethod{\
public:\
    Clazz(JniClass* targetObj, jmethodID mid, const char* methodname, const char* sig);\
    RetType call(bool *isExceptionOccur,...);\
};
StaticMethodDecl(JniStaticVoidMethod,void)
StaticMethodDecl(JniStaticIntMethod,jint)
StaticMethodDecl(JniStaticByteMethod,jbyte)
StaticMethodDecl(JniStaticCharMethod,jchar)
StaticMethodDecl(JniStaticBooleanMethod,jboolean)
StaticMethodDecl(JniStaticLongMethod,jlong)
StaticMethodDecl(JniStaticShortMethod,jshort)
StaticMethodDecl(JniStaticFloatMethod,jfloat)
StaticMethodDecl(JniStaticDoubleMethod,jdouble)
StaticMethodDecl(JniStaticObjectMethod,jobject)

class JniMethodWrap{
public:
    JniMethodWrap(JniObject* targetObj,JniMethod* method){
        mTargetObj=targetObj;
        mTargetMethod=method;
        if(method==nullptr || targetObj==nullptr){
            mIsValid=false;
        }else{
            mIsValid=true;
        }
    }
    jmethodID getMid(){
        return mTargetMethod->getMid();
    }
    bool isValid(){
        return mIsValid;
    }

protected:
   bool       mIsValid;
   JniObject* mTargetObj;
   JniMethod* mTargetMethod;
};
class JniVoidMethodWrap:public JniMethodWrap{
public:
    JniVoidMethodWrap(JniObject* targetObj,JniVoidMethod* method):
        JniMethodWrap(targetObj,method){
    }
    void call(bool *isExceptionOccur,...){
        if(mIsValid){
            va_list vl;
            va_start(vl, isExceptionOccur);
            JniVoidMethod* method=(JniVoidMethod*)mTargetMethod;
            method->call(mTargetObj,isExceptionOccur,vl);
            va_end(vl);
            exceptionObjClear(mTargetObj);
        }
    }
};
#define MethodWrapDecl(Clazz,RetType,WrapClazz) class Clazz:public JniMethodWrap{\
public:\
    Clazz(JniObject* targetObj, WrapClazz* wrapobj):JniMethodWrap(targetObj,wrapobj){}\
    RetType call(bool *isExceptionOccur,...){\
    va_list vl;\
    va_start(vl, isExceptionOccur);\
    WrapClazz* method=(WrapClazz*)mTargetMethod;\
    RetType retv=method->call(mTargetObj,isExceptionOccur,vl);\
    va_end(vl);\
    exceptionObjClear(mTargetObj);\
    return retv;\
    }\
};
MethodWrapDecl(JniIntMethodWrap,jint,JniIntMethod)
MethodWrapDecl(JniByteMethodWrap,jbyte,JniByteMethod)
MethodWrapDecl(JniCharMethodWrap,jchar,JniCharMethod)
MethodWrapDecl(JniBooleanMethodWrap,jboolean,JniBooleanMethod)
MethodWrapDecl(JniLongMethodWrap,jlong,JniLongMethod)
MethodWrapDecl(JniShortMethodWrap,jshort,JniShortMethod)
MethodWrapDecl(JniFloatMethodWrap,jfloat,JniFloatMethod)
MethodWrapDecl(JniDoubleMethodWrap,jdouble,JniDoubleMethod)
MethodWrapDecl(JniObjectMethodWrap,jobject,JniObjectMethod)

#endif // JNIMETHOD

