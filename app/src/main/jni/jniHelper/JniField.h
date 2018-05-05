#ifndef JNIFIELD
#define JNIFIELD
#include<jni.h>
#include"JniGlobalDef.h"
#include"./utils/jniutils.h"

class JniObject;
class JniClass;
class JniField{
public:
   JniField(JniClass* targetclz,jfieldID fid);
   virtual ~JniField();
protected:
   JniClass* mClass;
   jfieldID mFid;
};



#define FieldClazzDecl(Clazz,SetRetV) class Clazz:public JniField{\
    public:\
        Clazz(JniClass* targetObj,jfieldID fieldId);\
        SetRetV get(JniObject* targetobj);\
        JniObject* set(JniObject* targetobj,SetRetV value);\
};

FieldClazzDecl(JniIntField,jint)
FieldClazzDecl(JniLongField,jlong)
FieldClazzDecl(JniFloatField,jfloat)
FieldClazzDecl(JniDoubleField,jdouble)
FieldClazzDecl(JniObjectField,jobject)
FieldClazzDecl(JniCharField,jchar)
FieldClazzDecl(JniByteField,jbyte)
FieldClazzDecl(JniShortField,jshort)
FieldClazzDecl(JniBooleanField,jboolean)
class JniStringField:public JniField{
public:
    JniStringField(JniClass* targetclz,jfieldID fid);
    jstring get(JniObject* targetobj);
    char* getCharStr(JniObject* targetobj);//transform jstring field to char* out，you should releaseChar()it or just free it
    void releaseChar(char* strField);
    JniObject* set(JniObject* targetobj,JNI_IN const char* value);//char*
    JniObject* setGbStr(JniObject* targetobj,JNI_IN const char* value);
    JniObject* setUtf8Str(JniObject* targetobj,JNI_IN const char* value);
    JniObject* set(JniObject* targetobj,JNI_IN jstring value);
};

#define StaticFieldClazzDecl(Clazz,SetRetV) class Clazz:public JniField{\
    public:\
        Clazz(JniClass* targetObj,jfieldID fieldId);\
        SetRetV get();\
        JniClass* set(SetRetV value);\
};

StaticFieldClazzDecl(JniStaticIntField,jint)
StaticFieldClazzDecl(JniStaticShortField,jshort)
StaticFieldClazzDecl(JniStaticCharField,jchar)
StaticFieldClazzDecl(JniStaticByteField,jbyte)
StaticFieldClazzDecl(JniStaticBooleanField,jboolean)
StaticFieldClazzDecl(JniStaticLongField,jlong)
StaticFieldClazzDecl(JniStaticFloatField,jfloat)
StaticFieldClazzDecl(JniStaticDoubleField,jdouble)
StaticFieldClazzDecl(JniStaticObjectField,jobject)


#define FieldClazzWrapDecl(Clazz,SetRetV,WrapClz)  class Clazz{\
public:\
    Clazz(JniObject* targetObj,WrapClz* wrapobj){\
        mTargetObj=targetObj;\
        mField=wrapobj; \
    }\
    JniObject* set(SetRetV value){\
        mField->set(mTargetObj,value);\
        return mTargetObj;\
    }\
    SetRetV get(){\
        return mField->get(mTargetObj);\
    }\
private:\
    JniObject* mTargetObj;\
    WrapClz*   mField;\
};
FieldClazzWrapDecl(JniIntFieldWrap,jint,JniIntField)
FieldClazzWrapDecl(JniLongFieldWrap,jlong,JniLongField)
FieldClazzWrapDecl(JniFloatFieldWrap,jfloat,JniFloatField)
FieldClazzWrapDecl(JniDoubleFieldWrap,jdouble,JniDoubleField)
FieldClazzWrapDecl(JniObjectFieldWrap,jobject,JniObjectField)
FieldClazzWrapDecl(JniCharFieldWrap,jchar,JniCharField)
FieldClazzWrapDecl(JniByteFieldWrap,jbyte,JniByteField)
FieldClazzWrapDecl(JniShortFieldWrap,jshort,JniShortField)
FieldClazzWrapDecl(JniBooleanFieldWrap,jboolean,JniBooleanField)

class JniStringFieldWrap{
public:
    JniStringFieldWrap(JniObject* targetObj,JniStringField* wrapobj);
    jstring get();
    char* getCharStr();//transform jstring field to char* out，you should releaseChar()it or just free it
    void releaseChar(char* strField);
    JniObject* set(JNI_IN const char* value);//char*
    JniObject* setGbStr(JNI_IN const char* value);
    JniObject* setUtf8Str(JNI_IN const char* value);
    JniObject* set(JNI_IN jstring value);
private:
        JniObject* mTargetObj;
        JniStringField*   mField;
};
#endif // JNIFIELD

