#include"JniField.h"
#include"JniObject.h"
#include"JniClass.h"

#define ContructImp(Clazz) Clazz::Clazz(JniClass* targetObj,jfieldID fieldId):JniField(targetObj, fieldId){}
#define SetImp(Clazz,SetType,CallType) JniObject* Clazz::set(JniObject* targetobj,SetType value){\
    targetobj->getEnv()->Set##CallType##Field(targetobj->obj(),mFid,value);\
    return targetobj;\
    }
#define GetImp(Clazz,RetType,CallType) RetType Clazz::get(JniObject* targetobj){\
    return (RetType)targetobj->getEnv()->Get##CallType##Field(targetobj->obj(),mFid);\
    }

#define SetStaticImp(Clazz,SetType,CallType) JniClass* Clazz::set(SetType value){\
    mClass->getEnv()->SetStatic##CallType##Field(mClass->getJClass(),mFid,value);\
    return mClass;\
    }
#define GetStaticImp(Clazz,RetType,CallType) RetType Clazz::get(){\
    return (RetType)mClass->getEnv()->GetStatic##CallType##Field(mClass->getJClass(),mFid);\
    }

JniField::JniField(JniClass* targetclz,jfieldID fid){
    mFid=fid;
    mClass=targetclz;
}
JniField::~JniField(){}

ContructImp(JniIntField)
SetImp(JniIntField,jint,Int)
GetImp(JniIntField,jint,Int)

ContructImp(JniStaticIntField)
GetStaticImp(JniStaticIntField,jint,Int)
SetStaticImp(JniStaticIntField,jint,Int)
ContructImp(JniLongField)
SetImp(JniLongField,jlong,Long)
GetImp(JniLongField,jlong,Long)
ContructImp(JniStaticLongField)
GetStaticImp(JniStaticLongField,jlong,Long)
SetStaticImp(JniStaticLongField,jlong,Long)

ContructImp(JniDoubleField)
SetImp(JniDoubleField,jdouble,Double)
GetImp(JniDoubleField,jdouble,Double)

ContructImp(JniStaticDoubleField)
GetStaticImp(JniStaticDoubleField,jdouble,Double)
SetStaticImp(JniStaticDoubleField,jdouble,Double)


ContructImp(JniFloatField)
SetImp(JniFloatField,jfloat,Float)
GetImp(JniFloatField,jfloat,Float)

ContructImp(JniStaticFloatField)
GetStaticImp(JniStaticFloatField,jfloat,Float)
SetStaticImp(JniStaticFloatField,jfloat,Float)



ContructImp(JniBooleanField)
SetImp(JniBooleanField,jboolean,Boolean)
GetImp(JniBooleanField,jboolean,Boolean)

ContructImp(JniStaticBooleanField)
GetStaticImp(JniStaticBooleanField,jboolean,Boolean)
SetStaticImp(JniStaticBooleanField,jboolean,Boolean)

ContructImp(JniShortField)
SetImp(JniShortField,jshort,Short)
GetImp(JniShortField,jshort,Short)

ContructImp(JniStaticShortField)
GetStaticImp(JniStaticShortField,jshort,Short)
SetStaticImp(JniStaticShortField,jshort,Short)

ContructImp(JniCharField)
SetImp(JniCharField,jchar,Char)
GetImp(JniCharField,jchar,Char)

ContructImp(JniStaticCharField)
GetStaticImp(JniStaticCharField,jchar,Char)
SetStaticImp(JniStaticCharField,jchar,Char)

ContructImp(JniByteField)
SetImp(JniByteField,jbyte,Byte)
GetImp(JniByteField,jbyte,Byte)

ContructImp(JniStaticByteField)
GetStaticImp(JniStaticByteField,jbyte,Byte)
SetStaticImp(JniStaticByteField,jbyte,Byte)

ContructImp(JniObjectField)
SetImp(JniObjectField,jobject,Object)
GetImp(JniObjectField,jobject,Object)

ContructImp(JniStaticObjectField)
GetStaticImp(JniStaticObjectField,jobject,Object)
SetStaticImp(JniStaticObjectField,jobject,Object)

ContructImp(JniStringField)


jstring ctojstring(JNIEnv *env,const char* src,const char* codeset) {
    jclass Class_string;
    jmethodID mid_String;
    jbyteArray bytes;
    jstring codetype,jstr;
    Class_string = env->FindClass("java/lang/String");//获取class
    //先将gbk字符串转为java里的string格式
    mid_String = env->GetMethodID(Class_string, "<init>", "([BLjava/lang/String;)V");
    bytes = env->NewByteArray(strlen(src));
    env->SetByteArrayRegion(bytes, 0, strlen(src), (jbyte*)src);
    codetype = env->NewStringUTF(codeset);
    jstr = (jstring)env->NewObject(Class_string, mid_String, bytes, codetype);
    env->DeleteLocalRef(codetype);
    env->DeleteLocalRef(bytes);
//    //再将string变utf-8字符串。
//    mid_getBytes = env->GetMethodID(Class_string,   "getBytes",   "(Ljava/lang/String;)[B");
//    codetype = env->NewStringUTF("utf-8");
//    bytes=(jbyteArray)env->CallObjectMethod(jstr,mid_getBytes,codetype);
//    env->DeleteLocalRef(codetype);
//    log_utf8=env->GetByteArrayElements(bytes,JNI_FALSE);
//    return env->NewStringUTF((const char*)log_utf8);
    return jstr;
}

char* JniStringField::getCharStr(JniObject* targetobj){
    jstring jstr=(jstring)targetobj->getEnv()->GetObjectField(targetobj->obj(),mFid);
    const char *cStr= targetobj->getEnv()->GetStringUTFChars(jstr,NULL);
    char* value=(char*)malloc(sizeof(char)*(strlen(cStr)+1));
    memset(value,0,sizeof(char)*(strlen(cStr)+1));
    strcpy(value,cStr);
    targetobj->getEnv()->ReleaseStringUTFChars(jstr,cStr);
    return value;
}
void JniStringField::releaseChar(char* strField){
    free(strField);
}
jstring  JniStringField::get(JniObject* targetobj){
    jstring value=(jstring)targetobj->getEnv()->GetObjectField(targetobj->obj(),mFid);
    return value;
}
JniObject* JniStringField::set(JniObject* targetobj,JNI_IN jstring value){
    targetobj->getEnv()->SetObjectField(targetobj->obj(),mFid,value);
    return targetobj;
}
JniObject* JniStringField::set(JniObject* targetobj,JNI_IN const char* value){
    const char* v=value;
    jstring jstr=targetobj->getEnv()->NewStringUTF(v);
    targetobj->getEnv()->SetObjectField(targetobj->obj(),mFid,jstr);
    targetobj->getEnv()->DeleteLocalRef(jstr);
    return targetobj;
}
JniObject* JniStringField::setUtf8Str(JniObject* targetobj,JNI_IN const char* value){
    const char* v=value;
    jstring jstr=ctojstring(targetobj->getEnv(),value,"UTF-8");
    targetobj->getEnv()->SetObjectField(targetobj->obj(),mFid,jstr);
    targetobj->getEnv()->DeleteLocalRef(jstr);
    return targetobj;
}

JniObject* JniStringField::setGbStr(JniObject* targetobj,JNI_IN const char* value){
    const char* v=value;
    jstring jstr=ctojstring(targetobj->getEnv(),value,"GBK");
    targetobj->getEnv()->SetObjectField(targetobj->obj(),mFid,jstr);
    targetobj->getEnv()->DeleteLocalRef(jstr);
    return targetobj;
}


JniStringFieldWrap::JniStringFieldWrap(JniObject* targetObj,JniStringField* wrapobj){
    mTargetObj=targetObj;
    mField=wrapobj;
}

jstring JniStringFieldWrap::get(){
    return mField->get(mTargetObj);
}

char* JniStringFieldWrap::getCharStr(){//transform jstring field to char* out，you should releaseChar()it or just free it
    return mField->getCharStr(mTargetObj);
}
void JniStringFieldWrap::releaseChar(char* strField){
    mField->releaseChar(strField);
}
JniObject* JniStringFieldWrap::set(JNI_IN const char* value){//char*
   //LOGE_T("JniStr","------set char value:%s %d",value,strlen(value));
    mField->set(mTargetObj,value);
    return mTargetObj;
}
JniObject* JniStringFieldWrap::setGbStr(JNI_IN const char* value){
    //LOGE_T("JniStr","------setGbStr char value:%s %d",value,strlen(value));
    mField->setGbStr(mTargetObj,value);
    return mTargetObj;
}

JniObject* JniStringFieldWrap::setUtf8Str(JNI_IN const char* value){
    //LOGE_T("JniStr","------setUtf8Str char value:%s %d",value,strlen(value));
    mField->setUtf8Str(mTargetObj,value);
    return mTargetObj;
}

JniObject* JniStringFieldWrap::set(JNI_IN jstring value){
    //LOGE_T("JniStr","------set jstring value");
    mField->set(mTargetObj,value);
    return mTargetObj;
}
