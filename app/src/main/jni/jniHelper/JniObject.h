#ifndef JNIOBJECT
#define JNIOBJECT

#include<map>
#include<string>
#include"JniField.h"
#include"JniMethod.h"
#include"JniGlobalDef.h"
#include"JniClass.h"
#include"JniDataDefs.h"
using namespace std;
/*
 * @author lss
 * Wrapper Jni jobject
 * A smart , more safety and more effctive jni framework
 * ------------------
 * 2015-2016 in puwell
 */

class JniObject{

    public:
    /*
     * Trans Base type to java Object
     * for example: jint  -> Integer
     * JObj is a auto wrapper,will delete local ref of jobject when destory
     * */
    static JObj cintToJ(JNIEnv* env,jint value);
    static JObj cshortToJ(JNIEnv* env,jshort value);
    static JObj clongToJ(JNIEnv* env,jlong value);
    static JObj cfloatToJ(JNIEnv* env,jfloat value);
    static JObj cdoubleToJ(JNIEnv* env,jdouble value);
    static JObj ccharToJ(JNIEnv* env,jchar value);
    static JObj cbyteToJ(JNIEnv* env,jbyte value);
    static JObj cbooleanToJ(JNIEnv* env,jboolean value);
    static JStrObj cstringToJ(JNIEnv* env,const char* value);
    static JByteArrObj cbyteArrToJ(JNIEnv *env, int len, jbyte *src);

    static JNITempCStr jstringToC(JNIEnv* env,jstring str);
    static JNITempByteArr jbyteArrToC(JNIEnv* env,jbyteArray bytearr);
    static JNITempIntArr jintArrToC(JNIEnv* env,jintArray bytearr);
    static JNITempLongArr jlongArrToC(JNIEnv* env,jlongArray bytearr);
    static JNITempBooleanArr jbooleanArrToC(JNIEnv* env,jbooleanArray bytearr);
    static JNITempFloatArr jfloatArrToC(JNIEnv* env,jfloatArray bytearr);
    static JNITempDoubleArr jdoubleArrToC(JNIEnv* env,jdoubleArray bytearr);
    static JNITempCharArr jcharArrToC(JNIEnv* env,jcharArray bytearr);
    static JNITempShortArr jshortArrToC(JNIEnv* env,jshortArray bytearr);


    static JObj cintToJ(jint value);
    static JObj cshortToJ(jshort value);
    static JObj clongToJ(jlong value);
    static JObj cfloatToJ(jfloat value);
    static JObj cdoubleToJ(jdouble value);
    static JObj ccharToJ(jchar value);
    static JObj cbyteToJ(jbyte value);
    static JObj cbooleanToJ(jboolean value);
    static JStrObj cstringToJ(const char* value);
    static JByteArrObj cbyteArrToJ(int len, jbyte *src);

    static JNITempCStr jstringToC(jstring str);
    static JNITempByteArr jbyteArrToC(jbyteArray bytearr);
    static JNITempIntArr jintArrToC(jintArray bytearr);
    static JNITempLongArr jlongArrToC(jlongArray bytearr);
    static JNITempBooleanArr jbooleanArrToC(jbooleanArray bytearr);
    static JNITempFloatArr jfloatArrToC(jfloatArray bytearr);
    static JNITempDoubleArr jdoubleArrToC(jdoubleArray bytearr);
    static JNITempCharArr jcharArrToC(jcharArray bytearr);
    static JNITempShortArr jshortArrToC(jshortArray bytearr);

    public:
    //JniObject(JNIEnv* env);
    JniObject(JNIEnv* env,JniClass* clazz,bool localRef=true);//请保证clazz不会再JniObject对象释放前释放
    JniObject(JNIEnv* env,JniClass* clazz,jobject obj,bool localRef=true);//请保证clazz不会再JniObject对象释放前释放

    //JniObject(JNIEnv* env,jclass   clazz,bool localRef=true);

    JniObject(JNIEnv* env,jobject  obj,bool localRef=true);
    JniObject(JNIEnv* env,const char* classname,bool localRef=true);
    JniObject(JNIEnv* env,const char* classname,const char* constructSigm,int localRef=true,...);

    JniObject(JniClass* clazz,bool localRef=true);//请保证clazz不会再JniObject对象释放前释放
    JniObject(JniClass* clazz,jobject obj,bool localRef=true);//请保证clazz不会再JniObject对象释放前释放
    JniObject(jclass   clazz,bool localRef=true);
    JniObject(jobject  obj,bool localRef=true);
    JniObject(const char* classname,bool localRef=true);
    JniObject(const char* classname,const char* constructSigm,int localRef=true,...);

    JniObject(const JniObject& rh);
    JniObject& operator =(const JniObject& rh);
    JniObject(JniObject&& rh);
    ~JniObject();
    jobject obj();
    operator jobject();
    jobject newLocalrefObj(); //when you return a jobject to java in a method,call this method
    jobject newGlobalrefobj();
    void toLocalRef();
    void toGlobalRef();
    JniClass* getClass();

    //if no this field in jobject ,you will get a null pointer
    JniStringFieldWrap getStringField(const char* fieldName);
    JniIntFieldWrap getIntField(const char* fieldName);
    JniLongFieldWrap getLongField(const char* fieldName);
    JniObjectFieldWrap getObjectField(const char* fieldName,const char* fieldSig);
    JniFloatFieldWrap getFloatField(const char* fieldName);
    JniDoubleFieldWrap getDoubleField(const char* fieldName);
    JniShortFieldWrap getShortField(const char* fieldName);
    JniBooleanFieldWrap getBooleanField(const char* fieldName);
    JniByteFieldWrap getByteField(const char* fieldName);
    JniCharFieldWrap getCharField(const char* fieldName);

    JniStaticIntField* getStaticIntField(const char* fieldName);
    JniStaticLongField* getStaticLongField(const char* fieldName);
    JniStaticObjectField* getStaticObjectField(const char* fieldName,const char* fieldSig);
    JniStaticFloatField* getStaticFloatField(const char* fieldName);
    JniStaticDoubleField* getStaticDoubleField(const char* fieldName);
    JniStaticShortField* getStaticShortField(const char* fieldName);
    JniStaticBooleanField* getStaticBooleanField(const char* fieldName);
    JniStaticByteField* getStaticByteField(const char* fieldName);
    JniStaticCharField* getStaticCharField(const char* fieldName);

    //if no this method in jobject ,you will get a null pointer
    JniVoidMethodWrap getVoidMethod(const char* MethodName,const char* Signal);
    JniIntMethodWrap getIntMethod(const char* MethodName,const char* Signal);
    JniBooleanMethodWrap getBooleanMethod(const char* MethodName, const char* Signal);
    JniByteMethodWrap getByteMethod(const char* MethodName, const char* Signal);
    JniCharMethodWrap getCharMethod(const char* MethodName, const char* Signal);
    JniObjectMethodWrap getObjectMethod(const char* MethodName, const char* Signal);
    JniFloatMethodWrap getFloatMethod(const char* MethodName, const char* Signal);
    JniDoubleMethodWrap getDoubleMethod(const char* MethodName, const char* Signal);
    JniShortMethodWrap getShortMethod(const char* MethodName, const char* Signal);
    JniLongMethodWrap getLongMethod(const char* MethodName, const char* Signal);

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

    void updateEnv(JNIEnv* env);

    JNIEnv* getEnv();
    // If cannot construct a jobject(for example, connot found the class you gived in)
    // ,you will get a invalid state
    bool isValid();

    private:
    bool init(JNIEnv* env,JniClass* clazz,jobject obj,bool localRef);
    bool initAndAllocDeafault(JNIEnv* env,JniClass* clazz,bool localRef);
    bool initAndAlloc(JNIEnv* env,JniClass* clazz,const char* constructSigm,bool localRef,va_list args);
    bool mbValid;
    bool mbLocalRef;
    bool mIsAllocClazzBySelf;
    jobject mObj;
    JNIEnv* mEnv;
    JniClass* mJniClass;
};

#endif // JNIOBJECT

