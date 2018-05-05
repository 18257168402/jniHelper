#ifndef JNICLASS
#define JNICLASS
#include<map>
#include<string>
#include<jni.h>
#include"JniField.h"
#include"JniMethod.h"
#include<mutex>
using namespace std;
class JniClass{//请尽量将jclass全局引用的数量限制，如果JniClass是全局对象，那么请将jclass置为全局引用
public:
    JniClass(JNIEnv* env,jclass clazz,bool localRef=true);
    JniClass(JNIEnv *env, jobject obj, bool localRef=true);
    JniClass(JNIEnv* env,const char* classname,bool localRef=true);

    JniClass(jclass clazz,bool localRef=true);
    JniClass(jobject obj, bool localRef=true);
    JniClass(const char* classname,bool localRef=true);

    JniClass(const JniClass& rh);//复制构造函数
    JniClass& operator =(const JniClass& rh);
    JniClass(JniClass&& rh);//右值引用

    virtual ~JniClass();
    void toGlobalRef();
    void toLocalRef();
    bool isLocalRef();
    bool isValid();
    jclass getJClass();
    void updateEnv(JNIEnv* env);
    JNIEnv* getEnv();



    JniStaticIntField* getStaticIntField(const char* fieldName);
    JniStaticLongField* getStaticLongField(const char* fieldName);
    JniStaticObjectField* getStaticObjectField(const char* fieldName,const char* fieldSig);
    JniStaticFloatField* getStaticFloatField(const char* fieldName);
    JniStaticDoubleField* getStaticDoubleField(const char* fieldName);
    JniStaticShortField* getStaticShortField(const char* fieldName);
    JniStaticBooleanField* getStaticBooleanField(const char* fieldName);
    JniStaticByteField* getStaticByteField(const char* fieldName);
    JniStaticCharField* getStaticCharField(const char* fieldName);



    JniStaticVoidMethod* getStaticVoidMethod(const char* MethodName,const char* Signal);
    JniStaticIntMethod* getStaticIntMethod(const char* MethodName,const char* Signal);
    JniStaticBooleanMethod* getStaticBooleanMethod(const char* MethodName, const char* Signal);
    JniStaticByteMethod* getStaticByteMethod(const char* MethodName, const char* Signal);
    JniStaticCharMethod* getStaticCharMethod(const char* MethodName, const char* Signal);
    JniStaticObjectMethod* getStaticObjectMethod(const char* MethodName, const char* Signal);
    JniStaticFloatMethod* getStaticFloatMethod(const char* MethodName, const char* Signal);
    JniStaticDoubleMethod* getStaticDoubleMethod(const char* MethodName, const char* Signal);
    JniStaticShortMethod* getStaticShortMethod(const char* MethodName, const char* Signal);
    JniStaticLongMethod* getStaticLongMethod(const char* MethodName, const char* Signal);
    friend class JniObject;
private:
    void initClassNameFromJClass(jobject obj);
    bool checkCanCacheID();
    JniClass();
    //if no this field in jobject ,you will get a null pointer
    JniStringField* getStringField(const char* fieldName);
    JniIntField* getIntField(const char* fieldName);
    JniLongField* getLongField(const char* fieldName);
    JniObjectField* getObjectField(const char* fieldName,const char* fieldSig);
    JniFloatField* getFloatField(const char* fieldName);
    JniDoubleField* getDoubleField(const char* fieldName);
    JniShortField* getShortField(const char* fieldName);
    JniBooleanField* getBooleanField(const char* fieldName);
    JniByteField* getByteField(const char* fieldName);
    JniCharField* getCharField(const char* fieldName);

    //if no this method in jobject ,you will get a null pointer
    JniVoidMethod* getVoidMethod(const char* MethodName,const char* Signal);
    JniIntMethod* getIntMethod(const char* MethodName,const char* Signal);
    JniBooleanMethod* getBooleanMethod(const char* MethodName, const char* Signal);
    JniByteMethod* getByteMethod(const char* MethodName, const char* Signal);
    JniCharMethod* getCharMethod(const char* MethodName, const char* Signal);
    JniObjectMethod* getObjectMethod(const char* MethodName, const char* Signal);
    JniFloatMethod* getFloatMethod(const char* MethodName, const char* Signal);
    JniDoubleMethod* getDoubleMethod(const char* MethodName, const char* Signal);
    JniShortMethod* getShortMethod(const char* MethodName, const char* Signal);
    JniLongMethod* getLongMethod(const char* MethodName, const char* Signal);
    JniField* findAndStoreField(const char* fieldName,const  char* sig,int type);
    JniMethod* findAndStoreMehod(const char* methodName, const char* sig, int type);

    jclass mClass;
    bool bLocalRef;
    bool bValid;
    JNIEnv* mEnv;
    std::string mClassName;
    std::mutex  mMethodMapLock;
    std::mutex  mFieldMapLock;
    map<string,JniField*> mFieldMap;
    map<string,JniMethod*> mMethodMap;


    static std::mutex  gMethodMapLock;
    static std::mutex  gFieldMapLock;
    static  map<string,jfieldID>  gFieldIDMap;
    static  map<string,jmethodID> gMethodIDMap;
    static jfieldID findGFieldId(std::string key);
    static void setGFieldId(std::string key,jfieldID id);
    static jmethodID findGMethodId(std::string key);
    static void setGMethodId(std::string key,jmethodID id);
};

#endif // JNICLASS

