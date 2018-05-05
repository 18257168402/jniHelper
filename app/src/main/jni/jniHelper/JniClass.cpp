
#include"JniClass.h"
#include"./utils/jniutils.h"
#include"JniHelper.h"
#include"JniGlobalDef.h"
#include"JniDataDef_in.h"
static bool clearException(JNIEnv* env){
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}

static jclass buildJClassByClassname(JNIEnv* env,const char* classname){
    char *jniclass=(char*)malloc(sizeof(char)*(strlen(classname)+1));
    memset(jniclass,0,sizeof(char)*(strlen(classname)+1));
    transJavaClassToJniClass(classname,jniclass);
    jclass clazz=env->FindClass(jniclass);
    if(clazz==nullptr){
        env->ExceptionClear();
        clazz=JniHelper::findClass(jniclass);
        if(clazz==nullptr){
            JniLOGE("Can not find class:%s",jniclass);
            env->ExceptionClear();
        }
    }
    free(jniclass);
    return clazz;
}

std::mutex  JniClass::gMethodMapLock;
std::mutex  JniClass::gFieldMapLock;
map<string,jfieldID>  JniClass::gFieldIDMap;
map<string,jmethodID> JniClass::gMethodIDMap;
jfieldID JniClass::findGFieldId(std::string key){
    jfieldID retId = NULL;
    gFieldMapLock.lock();
    map<string,jfieldID>::const_iterator itr=gFieldIDMap.find(key);
    if(itr == gFieldIDMap.end()){
        retId=NULL;
    }else{
        retId=itr->second;
    }
    gFieldMapLock.unlock();
    //JniLOGE("findGFieldId key:%s retId:%ld",key.c_str(),(long)retId);
    return retId;
}
void JniClass::setGFieldId(std::string key,jfieldID id){
    gFieldMapLock.lock();
    gFieldIDMap.insert(make_pair(key,id));
    gFieldMapLock.unlock();
}
jmethodID JniClass::findGMethodId(std::string key){
    jmethodID retId=NULL;
    gMethodMapLock.lock();
    map<string,jmethodID>::const_iterator itr=gMethodIDMap.find(key);
    if(itr == gMethodIDMap.end()){
        retId=NULL;
    }else{
        retId=itr->second;
    }
    gMethodMapLock.unlock();
    //JniLOGE("findGMethodId key:%s retId:%ld",key.c_str(),(long)retId);
    return retId;
}
void JniClass::setGMethodId(std::string key,jmethodID id){
    gMethodMapLock.lock();
    gMethodIDMap.insert(make_pair(key,id));
    gMethodMapLock.unlock();
}

JniClass::JniClass(){
    bValid=false;
    mEnv=nullptr;
    mClass = nullptr;
}
JniClass::JniClass(JNIEnv *env, jobject obj, bool localRef){
    mClass=nullptr;
    bValid=false;
    bLocalRef=localRef;
    mEnv=env;
    if(env==nullptr||obj==nullptr){
        return;
    }
    jclass clazz=env->GetObjectClass(obj);
    localRefCountAdd();
    if(clazz==nullptr){
        clearException(env);
        return;
    }
    if(bLocalRef){
        mClass=clazz;
    }else{
        mClass=(jclass)GlobalRefAdd(env,clazz);
        LocalRefRemove(env,clazz);
    }
    initClassNameFromJClass(obj);
    bValid=true;
}

JniClass::JniClass(JNIEnv* env,jclass clazz,bool localRef){
    if(env==nullptr||clazz==nullptr){
        mClass=nullptr;
        bValid=false;
        return;
    }
    //JniLOGE("JniClass::JniClass by jclass");
    bLocalRef = localRef;
    mEnv = env;
    bValid=false;
    mClass = nullptr;

    if(localRef){
        mClass = (jclass)LocalRefAdd(mEnv,clazz);
    }else{
        mClass = (jclass)GlobalRefAdd(mEnv,clazz);
    }
    if(mClass!=nullptr){
        bValid=true;
    }
}
bool JniClass::checkCanCacheID(){
    return mClassName.length()>0;
}
void JniClass::initClassNameFromJClass(jobject obj){
    if(mClass==nullptr){
        return;
    }
    if(!mEnv->IsInstanceOf(obj,mClass)){
        return;
    }

    jmethodID getClassMid  = findGMethodId("java_getClass");
    //JniLOGE("=====initClassNameFromJClass getClassMid:%ld",(long)getClassMid);
    if(getClassMid==NULL){
         getClassMid=mEnv->GetMethodID(mClass,"getClass",SIG_Method_0(SIG_CLS));
         if(getClassMid==NULL){
            //JniLOGE("=====getClassMid==NULL");
            return;
         }
         setGMethodId("java_getClass",getClassMid);
    }

    jobject javaClazzobj = mEnv->CallObjectMethod(obj,getClassMid);
    if(javaClazzobj==NULL){
        //JniLOGE("=====javaClazzobj==NULL");
        return;
    }
    jclass javaClazzClazz=mEnv->GetObjectClass(javaClazzobj);
    if(javaClazzClazz==NULL){
        mEnv->DeleteLocalRef(javaClazzobj);
        //JniLOGE("=====javaClazzClazz==NULL");
        return;
    }
    jmethodID getCanonicalNameMid = findGMethodId("java_getCanonicalName");
    //JniLOGE("=====initClassNameFromJClass getCanonicalNameMid:%ld",(long)getCanonicalNameMid);
    if(getCanonicalNameMid==NULL){
          getCanonicalNameMid=mEnv->GetMethodID(javaClazzClazz,"getCanonicalName",SIG_Method_0(SIG_Str));
          if(getCanonicalNameMid==NULL){
             mEnv->DeleteLocalRef(javaClazzobj);
             mEnv->DeleteLocalRef(javaClazzClazz);
             //JniLOGE("=====getCanonicalNameMid==NULL");
             return;
          }
          setGMethodId("java_getCanonicalName",getCanonicalNameMid);
    }

    jstring objClassname = (jstring)mEnv->CallObjectMethod(javaClazzobj,getCanonicalNameMid);
    if(objClassname!=nullptr){//匿名对象将没有这个属性（没有类名），所以匿名对象不使用id缓存
        const char* classname = mEnv->GetStringUTFChars(objClassname,NULL);
        char* tempclassname =(char*)malloc(strlen(classname)+1);
        transJavaClassToSimpleClassname(classname,tempclassname);
        mClassName = tempclassname;
        free(tempclassname);
         mEnv->ReleaseStringUTFChars(objClassname,classname);
         mEnv->DeleteLocalRef(objClassname);
    }else{
        //JniLOGE("=====objClassname ==null");
        mClassName.clear();
    }

    //JniLOGE("=====classname:%s",tempclassname);
    mEnv->DeleteLocalRef(javaClazzClazz);
    mEnv->DeleteLocalRef(javaClazzobj);
}


JniClass::JniClass(JNIEnv* env,const char* classname,bool localRef){
    if(env==nullptr||classname==nullptr){
        bValid=false;
        return;
    }
    //JniLOGE("JniClass::JniClass by classname");
    if(strlen(classname)==0){
        bValid=false;
        return;
    }
    bLocalRef = localRef;
    mEnv = env;
    mClass = nullptr;
    bValid=false;
    char* tempclassname =(char*)malloc(strlen(classname)+1);
    transJavaClassToSimpleClassname(classname,tempclassname);
    mClassName = tempclassname;
    free(tempclassname);
    jclass localrefclazz=buildJClassByClassname(env,classname);
    if(localrefclazz==nullptr){
        return;
    }
    if(localRef){
        localRefCountAdd();
        mClass =localrefclazz;
    }else{
        mClass = (jclass)GlobalRefAdd(mEnv,localrefclazz);
        LocalRefRemove(mEnv,localrefclazz);
    }
    if(mClass!=nullptr){
        bValid=true;
    }
}
JniClass::JniClass(jclass clazz,bool localRef){
    new(this) JniClass(JniHelper::getEnv(),clazz,localRef);
}
JniClass::JniClass(jobject obj, bool localRef){
    new(this) JniClass(JniHelper::getEnv(),obj,localRef);
}
JniClass::JniClass(const char* classname,bool localRef){
    new(this) JniClass(JniHelper::getEnv(),classname,localRef);
}
JniClass& JniClass::operator =(const JniClass& rh){
    if(this==&rh){
        return *this;
    }
    if(this->mEnv!=nullptr){
        if(this->isValid()){
            if(this->mClass!=nullptr){
                if(this->bLocalRef){
                    LocalRefRemove(mEnv,mClass);
                }else{
                    GlobalRefRemove(mEnv,mClass);
                }
            }
        }
    }
    this->mEnv=rh.mEnv;
    this->bLocalRef=rh.bLocalRef;
    this->bValid=rh.bValid;
    this->mClass = nullptr;
    if(rh.mClass!=nullptr){
        if(this->bLocalRef){
            this->mClass=(jclass)LocalRefAdd(mEnv,rh.mClass);
        }else{
            this->mClass=(jclass)GlobalRefAdd(mEnv,rh.mClass);
        }
    }
    return *this;
}

JniClass::JniClass(const JniClass& rh){//复制构造函数
    this->mEnv=rh.mEnv;
    this->bLocalRef=rh.bLocalRef;
    this->bValid=rh.bValid;
    this->mClass = nullptr;
    if(rh.mClass!=nullptr){
        if(this->bLocalRef){
            this->mClass=(jclass)LocalRefAdd(mEnv,rh.mClass);
        }else{
            this->mClass=(jclass)GlobalRefAdd(mEnv,rh.mClass);
        }
    }
}
JniClass::JniClass(JniClass&& rh){//右值引用
    this->mEnv = rh.mEnv;
    this->bLocalRef=rh.bLocalRef;
    this->bValid=rh.bValid;
    this->mClass = nullptr;
    if(rh.mClass!=nullptr){
        this->mClass = rh.mClass;
        rh.mClass=nullptr;
    }
    map<string,JniField*>::const_iterator itrF;// mFieldMap;
    map<string,JniMethod*>::const_iterator itrM;//mMethodMap;
    for(itrF=rh.mFieldMap.begin();itrF!=rh.mFieldMap.end();itrF++){
        mFieldMap.insert(make_pair(itrF->first,itrF->second));
    }
    for(itrM=rh.mMethodMap.begin();itrM!=rh.mMethodMap.end();itrM++){
        mMethodMap.insert(make_pair(itrM->first,itrM->second));
    }
    rh.mFieldMap.clear();
    rh.mMethodMap.clear();
}

JniClass::~JniClass(){
    if(mClass!=nullptr){
        if(bLocalRef){
             LocalRefRemove(getEnv(),mClass);
        }else{
             GlobalRefRemove(getEnv(),mClass);
        }
    }
    map<string,JniField*>::const_iterator itrF;// mFieldMap;
    map<string,JniMethod*>::const_iterator itrM;//mMethodMap;
    for(itrF=mFieldMap.begin();itrF!=mFieldMap.end();itrF++){
        delete itrF->second;
    }
    for(itrM=mMethodMap.begin();itrM!=mMethodMap.end();itrM++){
        delete itrM->second;
    }
}

void JniClass::toGlobalRef(){
    if(bLocalRef){
        jclass localclz=mClass;
        mClass = (jclass)GlobalRefAdd(getEnv(),localclz);
        LocalRefRemove(getEnv(),localclz);
        bLocalRef=false;
    }
}

void JniClass::toLocalRef(){
    if(!bLocalRef){
        jclass globalclz=mClass;
        mClass = (jclass)LocalRefAdd(getEnv(),globalclz);
        GlobalRefRemove(getEnv(),globalclz);
        bLocalRef=true;
    }
}
bool JniClass::isLocalRef(){
    return bLocalRef;
}
 bool JniClass::isValid(){
     return bValid;
 }

jclass JniClass::getJClass(){
     return mClass;
}
void JniClass::updateEnv(JNIEnv* env){
    mEnv=env;
}
JNIEnv* JniClass::getEnv(){
    JNIEnv*  env=JniHelper::getEnv();
    if(env!=nullptr){
        return env;
    }
    return mEnv;
}

#define StaticFieldStoreCase(Type){\
    case Static##Type##Field: \
        if(fid==NULL){\
            fid = getEnv()->GetStaticFieldID(mClass,fieldName,sig);\
        }\
        if(fid==0){\
            clearException(getEnv());\
            return NULL;\
        }\
        clearException(getEnv());\
        field=new JniStatic##Type##Field(this,fid);\
        break;\
    }
#define FieldStoreCase(Type){\
    case Type##Field: \
        if(fid==NULL){\
            fid = getEnv()->GetFieldID(mClass,fieldName,sig);\
        }\
        if(fid==0){\
            clearException(getEnv());return NULL;\
        }\
        field=new Jni##Type##Field(this,fid);\
        break;\
    }

JniField* JniClass::findAndStoreField(const char* fieldName,const  char* sig,int type){
    std::lock_guard<std::mutex> lck(mFieldMapLock);
    JniField* field=NULL;
    map<string,JniField*>::const_iterator itr=mFieldMap.find(fieldName);
    if(itr==mFieldMap.end()){
        std::string fieldkey;
        jfieldID fid =NULL;
        if(checkCanCacheID()){
            fieldkey.append(mClassName).append(fieldName);
            fid = findGFieldId(fieldkey);
        }else{
            //JniLOGE("checkCanCacheID findAndStoreField error fieldName:%s",fieldName);
        }
        bool needCache=false;
        if(fid==NULL){
            needCache=true;
        }
        switch (type) {
                FieldStoreCase(String)
                FieldStoreCase(Int)
                FieldStoreCase(Object)
                FieldStoreCase(Long)
                FieldStoreCase(Float)
                FieldStoreCase(Double)
                FieldStoreCase(Short)
                FieldStoreCase(Boolean)
                FieldStoreCase(Char)
                FieldStoreCase(Byte)
                StaticFieldStoreCase(Int)
                StaticFieldStoreCase(Object)
                StaticFieldStoreCase(Long)
                StaticFieldStoreCase(Float)
                StaticFieldStoreCase(Double)
                StaticFieldStoreCase(Short)
                StaticFieldStoreCase(Boolean)
                StaticFieldStoreCase(Char)
                StaticFieldStoreCase(Byte)
                default:
            break;
        }
        if(fid!=NULL && needCache && checkCanCacheID()){
           setGFieldId(fieldkey,fid);
        }
        if(field!=NULL){
            mFieldMap.insert(make_pair(fieldName,field));
        }

    }else{
        field=itr->second;
        JniLOGI("########%s is already in map ",fieldName);
    }
    return field;
}
#define MethodStoreCase(Type) {\
    case Type##Method: \
        if(mid==NULL){\
            mid=getEnv()->GetMethodID(mClass,methodName,sig);\
        }\
        if(mid==0){\
            clearException(getEnv());\
            return NULL;\
        }\
        method = new Jni##Type##Method(this, mid, methodName, sig);\
        break;\
    }
#define StaticMethodStoreCase(Type) {\
    case Static##Type##Method: \
        if(mid==NULL){\
            mid=getEnv()->GetStaticMethodID(mClass,methodName,sig);\
        }\
        if(mid==0){\
            clearException(getEnv());\
            return NULL;\
        }\
        method = new JniStatic##Type##Method(this, mid, methodName, sig);\
        break;\
    }

JniMethod* JniClass::findAndStoreMehod(const char* methodName, const char* sig, int type){
    std::lock_guard<std::mutex> lck(mMethodMapLock);
    JniMethod *method=NULL;
    string strkey(methodName);
    strkey.append(sig);
    map<string,JniMethod*>::const_iterator itr=mMethodMap.find(strkey);
    if(itr==mMethodMap.end()){
        std::string methodkey;
        jmethodID mid=NULL;
        if(checkCanCacheID()){
            methodkey.append(mClassName).append(strkey);
            mid = findGMethodId(methodkey);
        }else{
            //JniLOGE("checkCanCacheID findAndStoreMehod error strkey:%s",strkey.c_str());
        }
        bool needCache=false;
        if(mid==NULL){
            needCache=true;
        }
        switch (type) {
                MethodStoreCase(Int)
                MethodStoreCase(Object)
                MethodStoreCase(Long)
                MethodStoreCase(Float)
                MethodStoreCase(Double)
                MethodStoreCase(Short)
                MethodStoreCase(Boolean)
                MethodStoreCase(Char)
                MethodStoreCase(Byte)
                MethodStoreCase(Void)

                StaticMethodStoreCase(Int)
                StaticMethodStoreCase(Object)
                StaticMethodStoreCase(Long)
                StaticMethodStoreCase(Float)
                StaticMethodStoreCase(Double)
                StaticMethodStoreCase(Short)
                StaticMethodStoreCase(Boolean)
                StaticMethodStoreCase(Char)
                StaticMethodStoreCase(Byte)
                StaticMethodStoreCase(Void)
                default:
            break;
        }
        if(mid!=NULL && needCache && checkCanCacheID()){
            setGMethodId(methodkey,mid);
        }
        if(method!=NULL)
            mMethodMap.insert(make_pair(strkey,method));
    }else{
        method=itr->second;
        JniLOGI("########%s is already in map ",methodName);
    }
    return method;
}

#define FiledGetImp(Type,Sig) Jni##Type##Field* JniClass::get##Type##Field(const char* fieldName){\
        JniField* field=findAndStoreField(fieldName,Sig,Type##Field);\
        return dynamic_cast<Jni##Type##Field*>(field);\
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
JniObjectField* JniClass::getObjectField(const char* fieldName,const char* fieldSig){
    JniField* field=findAndStoreField(fieldName,fieldSig,ObjectField);
    return dynamic_cast<JniObjectField*>(field);
}
FiledGetImp(StaticInt,SIG_Int)
FiledGetImp(StaticLong,SIG_Long)
FiledGetImp(StaticFloat,SIG_Float)
FiledGetImp(StaticDouble,SIG_Double)
FiledGetImp(StaticShort,SIG_Short)
FiledGetImp(StaticBoolean,SIG_Boolean)
FiledGetImp(StaticByte,SIG_Byte)
FiledGetImp(StaticChar,SIG_Char)
JniStaticObjectField* JniClass::getStaticObjectField(const char* fieldName,const char* fieldSig){
    JniField* field=findAndStoreField(fieldName,fieldSig,ObjectField);
    return dynamic_cast<JniStaticObjectField*>(field);
}
////杩欓噷閲囩敤浜厓妯″紡锛屼繚瀛樻墍鏈夎幏鍙栬繃鐨凧niField锛屾瀽鏋勭殑鏃跺�欑粺涓�閲婃斁锛屽娆¤幏鍙栦娇鐢↗niField鐨勫悓涓�涓璞�

#define MethodGetImp(Type) Jni##Type##Method* JniClass::get##Type##Method(const char* MethodName, const char* Signal){\
    JniMethod* m = findAndStoreMehod(MethodName, Signal,Type##Method);\
    return dynamic_cast<Jni##Type##Method*>(m);\
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

MethodGetImp(StaticInt)
MethodGetImp(StaticVoid)
MethodGetImp(StaticBoolean)
MethodGetImp(StaticByte)
MethodGetImp(StaticChar)
MethodGetImp(StaticObject)
MethodGetImp(StaticFloat)
MethodGetImp(StaticDouble)
MethodGetImp(StaticShort)
MethodGetImp(StaticLong)
