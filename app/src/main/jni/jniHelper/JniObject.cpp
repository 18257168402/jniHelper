#include"JniObject.h"
#include"JniHelper.h"
#include"./utils/jniutils.h"
#include"JniDataDef_in.h"
#include"JniClass.h"
#include<stdlib.h>
static bool clearException(JNIEnv* env){
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}
static jobject createDefaultObj(JNIEnv* env,jclass clazz){
    jmethodID construId=env->GetMethodID(clazz,"<init>","()V");
    if(construId == nullptr){
        env->ExceptionClear();
        return nullptr;
    }
    jobject obj=env->NewObject(clazz,construId);
    if(obj==nullptr){
        env->ExceptionClear();
        return nullptr;
    }
    return obj;
}

static jobject createObj(JNIEnv* env,jclass clazz,const char* constructSig,va_list args){
    jmethodID construId=env->GetMethodID(clazz,"<init>",constructSig);
    if(construId == nullptr){
        JniLOGE("Can not find construct sig:%s",constructSig);
        env->ExceptionClear();
        return nullptr;
    }
    jobject obj=env->NewObjectV(clazz,construId,args);
    if(obj==nullptr){
        env->ExceptionClear();
        return nullptr;
    }
    return obj;
}

#define ToJobjImp(Type,Clazz,ConstruSig,CType) JObj JniObject::c##CType##To##J(JNIEnv* env,Type value){\
    jclass clazz = env->FindClass(Clazz);       \
    jmethodID mid=env->GetMethodID(clazz,"<init>",ConstruSig);\
    jobject obj = env->NewObject(clazz,mid,(Type)value);\
    env->DeleteLocalRef(clazz);\
    return JObj(env,obj);\
    }\
    JObj JniObject::c##CType##To##J(Type value){\
        JNIEnv* env=JniHelper::getEnv();\
        jclass clazz = env->FindClass(Clazz);       \
        jmethodID mid=env->GetMethodID(clazz,"<init>",ConstruSig);\
        jobject obj = env->NewObject(clazz,mid,(Type)value);\
        env->DeleteLocalRef(clazz);\
        return JObj(env,obj);\
        }


ToJobjImp(jint,    CLASS_ObjInt,     SIG_VoidMethod_1(SIG_Int),int)
ToJobjImp(jshort,  CLASS_ObjShort,   SIG_VoidMethod_1(SIG_Short),short)
ToJobjImp(jlong,   CLASS_ObjLong,    SIG_VoidMethod_1(SIG_Long),long)
ToJobjImp(jfloat,  CLASS_ObjFloat,   SIG_VoidMethod_1(SIG_Float),float)
ToJobjImp(jdouble, CLASS_ObjDouble,  SIG_VoidMethod_1(SIG_Double),double)
ToJobjImp(jchar,   CLASS_ObjChar,    SIG_VoidMethod_1(SIG_Char),char)
ToJobjImp(jbyte,   CLASS_ObjByte,    SIG_VoidMethod_1(SIG_Byte),byte)
ToJobjImp(jboolean,CLASS_ObjBool,    SIG_VoidMethod_1(SIG_Boolean),boolean)

JStrObj JniObject::cstringToJ(JNIEnv* env,const char* value){
    jstring jstr=env->NewStringUTF(value);
    return JStrObj(env,jstr);
}

JByteArrObj JniObject::cbyteArrToJ(JNIEnv *env, int len, jbyte *src){
    return JByteArrObj(env,len,src);
}
JStrObj JniObject::cstringToJ(const char* value){
    return cstringToJ(JniHelper::getEnv(),value);
}

JByteArrObj JniObject::cbyteArrToJ( int len, jbyte *src){
    return cbyteArrToJ(JniHelper::getEnv(),len,src);
}

JNITempCStr JniObject::jstringToC(JNIEnv* env,jstring str){
    return JNITempCStr(env,str);
}
JNITempCStr JniObject::jstringToC(jstring str){
    return JNITempCStr(JniHelper::getEnv(),str);
}


#define JNITempArrToCImp(JType,TempClass) TempClass JniObject::JType##ArrToC(JNIEnv* env,JType##Array bytearr){\
    return TempClass(env,bytearr);\
}\
TempClass JniObject::JType##ArrToC(JType##Array bytearr){\
        return TempClass(JniHelper::getEnv(),bytearr);\
}
JNITempArrToCImp(jbyte,JNITempByteArr)
JNITempArrToCImp(jint,JNITempIntArr)
JNITempArrToCImp(jlong,JNITempLongArr)
JNITempArrToCImp(jfloat,JNITempFloatArr)
JNITempArrToCImp(jdouble,JNITempDoubleArr)
JNITempArrToCImp(jchar,JNITempCharArr)
JNITempArrToCImp(jshort,JNITempShortArr)
JNITempArrToCImp(jboolean,JNITempBooleanArr)

bool JniObject::initAndAllocDeafault(JNIEnv* env,JniClass* clazz,bool localRef){
    mEnv=env;
    mbLocalRef = localRef;
    mbValid=false;
    mJniClass=clazz;
    mJniClass->updateEnv(env);
    jobject obj=createDefaultObj(mEnv,mJniClass->getJClass());
    localRefCountAdd();
    if(clearException(env)){
        JniLOGE("JniObject--can not alloc object");
        mbValid=false;
        if(obj!=nullptr){
             LocalRefRemove(env,obj);
        }
        obj=nullptr;
    }
    if(obj==nullptr){
        //JniLOGE("JniObject--obj==nullptr--");
        return false;
    }
    if(mbLocalRef){
        mObj=obj;
    }else{
        mObj=GlobalRefAdd(env,obj);
        LocalRefRemove(env,obj);
    }
    if(mObj==nullptr){
        //JniLOGE("JniObject--obj==nullptr2--");
        mbValid=false;
        return false;
    }
    mbValid=true;
    return true;
}
bool JniObject::init(JNIEnv* env,JniClass* clazz,jobject obj,bool localRef){
    mbValid=false;
    mEnv=env;
    mJniClass=clazz;
    mbLocalRef=localRef;
    if(mbLocalRef){
        mObj=LocalRefAdd(env,obj);
    }else{
        mObj=GlobalRefAdd(env,obj);
    }
    if(mObj==nullptr){
        mbValid=false;
        return false;
    }
    mbValid=true;
    return true;
}

bool JniObject::initAndAlloc(JNIEnv* env,JniClass* clazz,const char* constructSig,bool localRef,va_list args){
    mEnv=env;
    mObj=NULL;
    mbLocalRef=localRef;
    mJniClass=clazz;
    if(!mJniClass->isValid()){
        mbValid=false;
        return false;
    }
    jobject obj=createObj(env,mJniClass->getJClass(),constructSig,args);
    if(obj==nullptr){
        env->ExceptionClear();
        mbValid=false;
        return false;
    }
    if(mbLocalRef){
        mObj=obj;
        localRefCountAdd();
    }else{
        mObj=GlobalRefAdd(env,obj);
        LocalRefRemove(env,obj);
    }
    mbValid=true;
    return true;
}

JniObject::JniObject(JNIEnv* env,jobject obj,bool localRef){
    //JniLOGE("JniObject::JniObject by obj");

    if(obj==nullptr){
        mObj=nullptr;
        mJniClass=nullptr;
        mbValid=false;
        return;
    }
    mIsAllocClazzBySelf=true;
    mJniClass = new JniClass(env,obj,localRef);
    if(!mJniClass->isValid()){
        mbValid=false;
        return;
    }
    jclass clazz=mJniClass->getJClass();
    if(clazz==NULL){
        mbValid=false;
        return;
    }
    if(clearException(env)){
        JniLOGE("can not get class from object");
        mbValid=false;
        return;
    }
    localRefCountAdd();


    init(env,mJniClass,obj,localRef);
    //LocalRefRemove(env,clazz);
}
JniObject::JniObject(JNIEnv* env,JniClass* clazz,bool localRef){
    mIsAllocClazzBySelf=false;
    initAndAllocDeafault(env,clazz,localRef);
}
JniObject::JniObject(JNIEnv* env,JniClass* clazz,jobject obj,bool localRef){
    if(env==nullptr||clazz==nullptr||obj==nullptr){
        mObj =nullptr;
        mJniClass=nullptr;
        mbValid=false;
    }
    mIsAllocClazzBySelf=false;
    mJniClass=clazz;
    mJniClass->updateEnv(env);
    mbLocalRef=localRef;
    mEnv=env;
    if(mbLocalRef){
        mObj=LocalRefAdd(env,obj);
    }else{
        mJniClass->toGlobalRef();
        mObj=GlobalRefAdd(env,obj);
    }
    mbValid=true;
}

//JniObject::JniObject(JNIEnv* env,jclass clazz,bool localRef){
//    mIsAllocClazzBySelf=true;

//    initAndAllocDeafault(env,new JniClass(env,clazz,localRef),localRef);
//}
JniObject::JniObject(JNIEnv* env,const char* classname,bool localRef){
    //JniLOGE("JniObject::JniObject by classname");
    mIsAllocClazzBySelf=true;
    initAndAllocDeafault(env,new JniClass(env,classname,localRef),localRef);

}

JniObject::JniObject(JNIEnv* env,const char* classname,const char* constructSig,int localRef,...){
    va_list vl;
    va_start(vl, localRef);
    mIsAllocClazzBySelf=true;
    initAndAlloc(env,new JniClass(env,classname,localRef),constructSig,localRef, vl);
    va_end(vl);

}

JniObject::JniObject(JniClass* clazz,bool localRef){
    new(this) JniObject(JniHelper::getEnv(),clazz,localRef);
}
JniObject::JniObject(JniClass* clazz,jobject obj,bool localRef){//请保证clazz不会再JniObject对象释放前释放
     new(this) JniObject(JniHelper::getEnv(),obj,localRef);
}
JniObject::JniObject(jclass   clazz,bool localRef){
    new(this) JniObject(JniHelper::getEnv(),clazz,localRef);
}
JniObject::JniObject(jobject  obj,bool localRef){
    new(this) JniObject(JniHelper::getEnv(),obj,localRef);
}
JniObject::JniObject(const char* classname,bool localRef){
    new(this) JniObject(JniHelper::getEnv(),classname,localRef);
}
JniObject::JniObject(const char* classname,const char* constructSig,int localRef,...){
    va_list vl;
    va_start(vl, localRef);
    mIsAllocClazzBySelf=true;
    JNIEnv* env=JniHelper::getEnv();
    initAndAlloc(env,new JniClass(env,classname,localRef),constructSig,localRef, vl);
    va_end(vl);
}

bool JniObject::isValid(){
    return mbValid;
}
JniObject& JniObject::operator =(const JniObject& rh){
    // JniLOGE("JniObject::JniObject operator =");
    if(this==&rh){
        return *this;
    }
    this->mEnv = rh.mEnv;
    this->mbLocalRef = rh.mbLocalRef;
    this->mJniClass = rh.mJniClass;
    this->mbValid=rh.mbValid;
    if(rh.mObj==nullptr){
        this->mObj=nullptr;
        return *this;
    }
    if(this->mbLocalRef){
        this->mObj=LocalRefAdd(mEnv,rh.mObj);
    }else{
        this->mObj=GlobalRefAdd(mEnv,rh.mObj);
    }
    if(rh.mJniClass!=nullptr){
        if(!rh.mIsAllocClazzBySelf){
            this->mJniClass=rh.mJniClass;
            this->mIsAllocClazzBySelf=rh.mIsAllocClazzBySelf;
        }else{
            this->mJniClass=new JniClass(*rh.mJniClass);
            this->mIsAllocClazzBySelf=true;
        }
    }

    if(this->mObj==nullptr){
        this->mbValid=false;
    }
    return *this;
}

JniObject::JniObject(const JniObject& rh){
    // JniLOGE("JniObject::JniObject copy");
    this->mEnv = rh.mEnv;
    this->mbLocalRef = rh.mbLocalRef;
    this->mJniClass = rh.mJniClass;
    this->mbValid=rh.mbValid;
    if(rh.mObj==nullptr){
        this->mObj=nullptr;
        return;
    }
    if(this->mbLocalRef){
        this->mObj=LocalRefAdd(mEnv,rh.mObj);
    }else{
        this->mObj=GlobalRefAdd(mEnv,rh.mObj);
    }
    if(rh.mJniClass!=nullptr){
        if(!rh.mIsAllocClazzBySelf){
            this->mJniClass=rh.mJniClass;
            this->mIsAllocClazzBySelf=rh.mIsAllocClazzBySelf;
        }else{
            this->mJniClass=new JniClass(*rh.mJniClass);
            this->mIsAllocClazzBySelf=true;
        }
    }

    if(this->mObj==nullptr){
        this->mbValid=false;
    }
}
JniObject::JniObject(JniObject&& rh){
    //JniLOGE("JniObject::JniObject r copy");
    this->mEnv = rh.mEnv;
    this->mbLocalRef = rh.mbLocalRef;
    this->mIsAllocClazzBySelf=rh.mIsAllocClazzBySelf;
    this->mbValid=rh.mbValid;
    if(rh.mObj!=nullptr){
        this->mObj =rh.mObj;
        rh.mObj=nullptr;
    }
    if(rh.mJniClass!=nullptr){
        this->mJniClass = rh.mJniClass;
        rh.mJniClass=nullptr;
    }
}
JniObject::~JniObject(){
    //JniLOGE("JniObject::~JniObject %d",mbLocalRef);
    if(mObj!=nullptr){
        if(mbLocalRef){
            LocalRefRemove(mEnv,mObj);
            mObj=nullptr;
        }else{
            GlobalRefRemove(mEnv,mObj);
            mObj=nullptr;
        }
    }
    if(mIsAllocClazzBySelf){
        if(mJniClass!=nullptr){
            delete mJniClass;
            mJniClass=nullptr;
        }
    }
}

jobject JniObject::obj(){
    return mObj;
}
JniObject::operator jobject(){
    return mObj;
}
jobject JniObject::newLocalrefObj(){
    return getEnv()->NewLocalRef(mObj);
}
jobject JniObject::newGlobalrefobj(){
    return getEnv()->NewGlobalRef(mObj);
}
void JniObject::toLocalRef(){
    if(!mbLocalRef){
        if(mIsAllocClazzBySelf){
            mJniClass->toLocalRef();
        }
        jobject obj=mObj;
        mObj=LocalRefAdd(getEnv(),obj);
        GlobalRefRemove(getEnv(),obj);
        mbLocalRef=true;
    }
}

void JniObject::toGlobalRef(){
    if(mbLocalRef){
        mJniClass->toGlobalRef();//只要有一个对象是全局的，那么其类也要转化成全局类
        jobject obj=mObj;
        mObj=GlobalRefAdd(getEnv(),obj);
        LocalRefRemove(getEnv(),obj);
        mbLocalRef=false;
    }
}

JniClass* JniObject::getClass(){
    return mJniClass;
}



void JniObject::updateEnv(JNIEnv* env){
    if(mJniClass->isValid()){
        mJniClass->updateEnv(env);
    }
    mEnv=env;
}
JNIEnv* JniObject::getEnv(){
    JNIEnv*  env=JniHelper::getEnv();
    if(env!=nullptr){
        return env;
    }
    return mEnv;
}




#define FiledGetImp(Type,Sig) Jni##Type##FieldWrap JniObject::get##Type##Field(const char* fieldName){\
    JniField* field=mJniClass->get##Type##Field(fieldName);\
    return Jni##Type##FieldWrap(this,(Jni##Type##Field*)field);\
    }

FiledGetImp(String,SIG_Str)
FiledGetImp(Int,SIG_Int)
FiledGetImp(Long,SIG_Long)
FiledGetImp(Float,SIG_Float)
FiledGetImp(Double,SIG_Double)
FiledGetImp(Short, SIG_Short)
FiledGetImp(Boolean,SIG_Boolean)
FiledGetImp(Byte,SIG_Byte)
FiledGetImp(Char,SIG_Char)

JniObjectFieldWrap JniObject::getObjectField(const char* fieldName,const char* fieldSig){
    JniField* field=mJniClass->getObjectField(fieldName,fieldSig);
    return JniObjectFieldWrap(this,(JniObjectField*)field);
}

#define StaticFiledGetImp(Type,Sig) Jni##Type##Field* JniObject::get##Type##Field(const char* fieldName){\
    JniField* field=mJniClass->get##Type##Field(fieldName);\
    return (Jni##Type##Field*)field;\
    }
StaticFiledGetImp(StaticInt,SIG_Int)
StaticFiledGetImp(StaticLong,SIG_Long)
StaticFiledGetImp(StaticFloat,SIG_Float)
StaticFiledGetImp(StaticDouble,SIG_Double)
StaticFiledGetImp(StaticShort,SIG_Short)
StaticFiledGetImp(StaticBoolean,SIG_Boolean)
StaticFiledGetImp(StaticByte,SIG_Byte)
StaticFiledGetImp(StaticChar,SIG_Char)
JniStaticObjectField* JniObject::getStaticObjectField(const char* fieldName,const char* fieldSig){
    JniField* field=mJniClass->getStaticObjectField(fieldName,fieldSig);
    return dynamic_cast<JniStaticObjectField*>(field);
}
////杩欓噷閲囩敤浜厓妯″紡锛屼繚瀛樻墍鏈夎幏鍙栬繃鐨凧niField锛屾瀽鏋勭殑鏃跺�欑粺涓�閲婃斁锛屽娆¤幏鍙栦娇鐢↗niField鐨勫悓涓�涓璞�

#define MethodGetImp(Type) Jni##Type##MethodWrap JniObject::get##Type##Method(const char* MethodName, const char* Signal){\
    JniMethod* m = mJniClass->get##Type##Method(MethodName, Signal);\
    return Jni##Type##MethodWrap(this,(Jni##Type##Method*)m);\
    }
MethodGetImp(Int)
MethodGetImp(Void)
MethodGetImp(Boolean)
MethodGetImp(Byte)
MethodGetImp(Char)
MethodGetImp(Object)
MethodGetImp(Float)
MethodGetImp(Double)
MethodGetImp(Short)
MethodGetImp(Long)

#define StaticMethodGetImp(Type) Jni##Type##Method* JniObject::get##Type##Method(const char* MethodName, const char* Signal){\
    JniMethod* m = mJniClass->get##Type##Method(MethodName, Signal);\
    return (Jni##Type##Method*)m;\
    }

StaticMethodGetImp(StaticInt)
StaticMethodGetImp(StaticVoid)
StaticMethodGetImp(StaticBoolean)
StaticMethodGetImp(StaticByte)
StaticMethodGetImp(StaticChar)
StaticMethodGetImp(StaticObject)
StaticMethodGetImp(StaticFloat)
StaticMethodGetImp(StaticDouble)
StaticMethodGetImp(StaticShort)
StaticMethodGetImp(StaticLong)

