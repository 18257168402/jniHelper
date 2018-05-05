#ifndef JNIDATADEFS_H
#define JNIDATADEFS_H
#include<jni.h>
#include<stdlib.h>
#include"JniHelper.h"
#define JArrObjDecl(Type,ArrType,SrcType) class J##Type##ArrObj{\
private:\
    JNIEnv* mEnv;\
    ArrType mByteArr;\
    int mStart;\
    int mLen;\
public:\
    J##Type##ArrObj(JNIEnv*env,int len,SrcType* src){\
        mEnv=env;\
        mStart=0;\
        mLen=len;\
        mByteArr=(ArrType)mEnv->New##Type##Array(len);\
        mEnv->Set##Type##ArrayRegion(mByteArr,0,len,src);\
    }\
    J##Type##ArrObj(int len,SrcType* src){\
        mEnv=JniHelper::getEnv();\
        mStart=0;\
        mLen=len;\
        mByteArr=(ArrType)mEnv->New##Type##Array(len);\
        mEnv->Set##Type##ArrayRegion(mByteArr,0,len,src);\
    }\
    virtual ~J##Type##ArrObj(){\
        if(mByteArr!=nullptr){\
            mEnv->DeleteLocalRef(mByteArr);\
        }\
    }\
    J##Type##ArrObj(const J##Type##ArrObj& rh){\
        this->mEnv = rh.mEnv;\
        this->mStart=rh.mStart;\
        this->mLen=rh.mLen;\
        if(rh.mByteArr!=nullptr){\
            this->mByteArr = (ArrType)mEnv->NewLocalRef(rh.mByteArr);\
        }\
    }\
    J##Type##ArrObj& operator =(const J##Type##ArrObj& rh){\
        if(this == &rh){\
            return *this;\
        }\
        this->mEnv=rh.mEnv;\
        this->mStart=rh.mStart;\
        this->mLen=rh.mLen;\
        if(rh.mByteArr!=nullptr){\
            this->mByteArr=(ArrType)mEnv->NewLocalRef(rh.mByteArr);\
        }\
        return *this;\
    }\
    J##Type##ArrObj(J##Type##ArrObj&& rh){\
        this->mEnv = rh.mEnv;\
        this->mStart=rh.mStart;\
        this->mLen=rh.mLen;\
        if(rh.mByteArr!=nullptr){\
            this->mByteArr =rh.mByteArr;\
            rh.mByteArr=nullptr;\
        }\
    }\
    int start(){\
        return mStart;\
    }\
    int size(){\
        return mLen;\
    }\
    ArrType get(){\
        return mByteArr;\
    }\
    ArrType newLocalRef(){\
        return (ArrType)mEnv->NewLocalRef(mByteArr);\
    }\
};
JArrObjDecl(Byte,jbyteArray,jbyte)
JArrObjDecl(Char,jcharArray,jchar)
JArrObjDecl(Int,jintArray,jint)
JArrObjDecl(Long,jlongArray,jlong)
JArrObjDecl(Float,jfloatArray,jfloat)
JArrObjDecl(Double,jdoubleArray,jdouble)
JArrObjDecl(Short,jshortArray,jshort)
JArrObjDecl(Boolean,jbooleanArray,jboolean)


class JObj{
protected:
    JNIEnv* mEnv;
    jobject mobj;
public:
    JObj(JNIEnv*env,jobject obj){mEnv=env;mobj=obj;}
    JObj(jobject obj){new(this) JObj(JniHelper::getEnv(),obj);}
    /*
     *Deep copy,prevent double delete local ref problem from shallow copying
     * */
    JObj(const JObj& rh){
        this->mEnv = rh.mEnv;
        if(rh.mobj!=nullptr){
            this->mobj = mEnv->NewLocalRef(rh.mobj);
        }
    }
    JObj& operator =(const JObj& rh){
        if(this == &rh){
            return *this;
        }
        this->mEnv=rh.mEnv;
        if(rh.mobj!=nullptr){
            this->mobj=mEnv->NewLocalRef(rh.mobj);
        }
        return *this;
    }
    /*
     *R value copy,a new feature of c++11
     * */
    JObj(JObj&& rh){
        this->mEnv = rh.mEnv;
        if(rh.mobj!=nullptr){
            this->mobj =rh.mobj;
            rh.mobj=nullptr;
        }
    }
    ~JObj(){if(mobj==nullptr){return;}mEnv->DeleteLocalRef(mobj);}
    jobject get(){return mobj;}
    //When you return a jobject to java,call this method
    jobject newLocalRef(){return mEnv->NewLocalRef(mobj);}
};

class JStrObj:public JObj{
public:
    JStrObj(JNIEnv*env,jobject obj):JObj(env,obj){}
    JStrObj(jobject obj):JObj(JniHelper::getEnv(),obj){}
    jstring getstr(){return (jstring)mobj;}
    jstring newStrLocalRef(){return (jstring)mEnv->NewLocalRef(mobj);}

};
class JNITempCStr{
    JNIEnv* mEnv;
    const char* mCstr;
    jstring mJstr;
    jboolean isCopy;
public:
    JNITempCStr(JNIEnv* env,jstring jstr){
        mEnv = env;
        mJstr =jstr;
        isCopy=false;
        mCstr=env->GetStringUTFChars(jstr,&isCopy);
        // JniLOGE("-JNITempCStr--%s,%d",mCstr,(int)isCopy);
    }
    JNITempCStr(jstring jstr){
        new(this) JNITempCStr(JniHelper::getEnv(),jstr);
    }
    JNITempCStr(const JNITempCStr& rh){
        this->mEnv = rh.mEnv;
        this->isCopy=rh.isCopy;
        if(rh.mJstr!=nullptr){
            this->mJstr = rh.mJstr;
            this->mCstr=mEnv->GetStringUTFChars(this->mJstr,&isCopy);
        }
    }
    JNITempCStr& operator =(const JNITempCStr& rh){
        if(this == &rh){
            return *this;
        }
        this->mEnv=rh.mEnv;
        this->isCopy=rh.isCopy;
        if(rh.mJstr!=nullptr){
            this->mJstr = rh.mJstr;
            this->mCstr=mEnv->GetStringUTFChars(this->mJstr,&isCopy);
        }
        return *this;
    }
    /*
     *R value copy,a new feature of c++11
     * */
    JNITempCStr(JNITempCStr&& rh){
        this->mEnv = rh.mEnv;
        this->isCopy=rh.isCopy;
        if(rh.mJstr!=nullptr){
            this->mJstr =rh.mJstr;
            this->mCstr=rh.mCstr;
            rh.mJstr=nullptr;
            rh.mCstr=nullptr;
        }
    }
    virtual ~JNITempCStr(){
        // JniLOGE("-~JNITempCStr--%s,%d",mCstr,(int)isCopy);
        if(mJstr!=nullptr && mCstr!=nullptr){
            mEnv->ReleaseStringUTFChars(mJstr,mCstr);
        }
    }
    operator const char*() {//隐式类型转换
        return mCstr;
    }
    operator char*(){
        return (char*)mCstr;
    }
    const char* get(){
        return mCstr;
    }
};

#define JNIArrTempTypeClassDeclare(ClazzName,Type,FromType) class JNITemp##ClazzName##Arr{\
    JNIEnv* mEnv;\
    Type* mCtype;\
    FromType mJtype;\
    jsize   mSize;\
    public:\
    JNITemp##ClazzName##Arr(JNIEnv* env,FromType jtype){\
    mEnv = env;\
    mJtype =jtype;\
    mCtype=(Type*)mEnv->Get##ClazzName##ArrayElements(mJtype,NULL);\
    mSize=mEnv->GetArrayLength(mJtype);\
    }\
    JNITemp##ClazzName##Arr(FromType jtype){\
    mEnv = JniHelper::getEnv();\
    mJtype =jtype;\
    mCtype=(Type*)mEnv->Get##ClazzName##ArrayElements(mJtype,NULL);\
    mSize=mEnv->GetArrayLength(mJtype);\
    }\
    JNITemp##ClazzName##Arr(const JNITemp##ClazzName##Arr& rh){\
    this->mEnv = rh.mEnv;\
    this->mSize = rh.mSize;\
    if(rh.mJtype!=nullptr){\
    this->mJtype =rh.mJtype;\
    this->mCtype=(Type*)mEnv->Get##ClazzName##ArrayElements(mJtype,NULL);\
    }\
    }\
    JNITemp##ClazzName##Arr& operator =(const JNITemp##ClazzName##Arr& rh){\
        if(this == &rh){\
        return *this;\
        }\
        this->mEnv=rh.mEnv;\
        this->mSize = rh.mSize;\
        if(rh.mJtype!=nullptr){\
        this->mJtype = rh.mJtype;\
        this->mCtype=(Type*)mEnv->Get##ClazzName##ArrayElements(mJtype,NULL);\
        }\
        return *this;\
    }\
    JNITemp##ClazzName##Arr(JNITemp##ClazzName##Arr&& rh){\
    this->mEnv = rh.mEnv;\
    this->mSize = rh.mSize;\
    if(rh.mJtype!=nullptr){\
    this->mJtype =rh.mJtype;\
    this->mCtype=rh.mCtype;\
    rh.mJtype=nullptr;\
    rh.mCtype=nullptr;\
    }\
    }\
    virtual ~JNITemp##ClazzName##Arr(){\
    if(mJtype!=nullptr&&mCtype!=nullptr)\
    mEnv->Release##ClazzName##ArrayElements(mJtype,mCtype,0);\
    }\
    jsize getSize(){\
    return mSize;\
    }\
    operator Type*(){\
        return mCtype;\
    }\
    Type* get(){\
    return mCtype;\
    }\
    Type at(int index){\
        return (Type)mCtype[index];\
    }\
    };

JNIArrTempTypeClassDeclare(Byte,jbyte,jbyteArray)
JNIArrTempTypeClassDeclare(Boolean,jboolean,jbooleanArray)
JNIArrTempTypeClassDeclare(Char,jchar,jcharArray)
JNIArrTempTypeClassDeclare(Int,jint,jintArray)
JNIArrTempTypeClassDeclare(Long,jlong,jlongArray)
JNIArrTempTypeClassDeclare(Float,jfloat,jfloatArray)
JNIArrTempTypeClassDeclare(Double,jdouble,jdoubleArray)
JNIArrTempTypeClassDeclare(Short,jshort,jshortArray)

class JNITempObjectArr{
    JNIEnv* mEnv;
   // jobject* mCtype;
    jobjectArray mJtype;
    jsize   mSize;
    jobject* mItems;
    public:
    JNITempObjectArr(JNIEnv* env,jobjectArray jtype){
        mEnv = env;
        mJtype =jtype;
        //mCtype=(Type*)mEnv->GetObjectArrayElement(mJtype,NULL);
        mSize=mEnv->GetArrayLength(mJtype);
        if(mSize>0){
            mItems = new jobject[mSize];
            memset(mItems,0,sizeof(jobject)*mSize);
        }else{
            mItems=NULL;
        }
    }
    JNITempObjectArr(jobjectArray jtype){
        new(this) JNITempObjectArr(JniHelper::getEnv(),jtype);
    }
    JNITempObjectArr(const JNITempObjectArr& rh){
        this->mEnv = rh.mEnv;
        this->mSize = rh.mSize;
        if(rh.mJtype!=nullptr){
            this->mJtype =rh.mJtype;
        }
        if(mSize>0){
            mItems = new jobject[mSize];
            memset(mItems,0,sizeof(jobject)*mSize);
        }else{
            mItems=NULL;
        }
    }
    JNITempObjectArr& operator =(const JNITempObjectArr& rh){
        if(this == &rh){
            return *this;
        }
        this->mEnv=rh.mEnv;
        this->mSize = rh.mSize;
        if(rh.mJtype!=nullptr){
            this->mJtype = rh.mJtype;
        }
        if(mSize>0){
            mItems = new jobject[mSize];
            memset(mItems,0,sizeof(jobject)*mSize);
        }else{
            mItems=NULL;
        }
        return *this;
    }
    JNITempObjectArr(JNITempObjectArr&& rh){
        this->mEnv = rh.mEnv;
        this->mSize = rh.mSize;
        if(rh.mJtype!=nullptr){
            this->mJtype =rh.mJtype;
            rh.mJtype=nullptr;
        }
        if(rh.mItems!=NULL){
            this->mItems = rh.mItems;
            rh.mItems=NULL;
        }
    }
    virtual ~JNITempObjectArr(){
        if(mItems!=NULL){
            for(int i=0;i<mSize;i++){
                jobject item=mItems[i];
                if(item!=NULL){
                    mEnv->DeleteLocalRef(item);
                }
                mItems[i]=NULL;
            }
            delete[] mItems;
            mItems=NULL;
        }
    }
    jsize getSize(){
        return mSize;
    }
    jobject get(int index){
        if(mItems[index]==NULL){
            mItems[index] = mEnv->GetObjectArrayElement(mJtype,index);
        }
        return mItems[index];
    }
    jobject at(int index){
        return get(index);
    }
};

class JNITempObjectRef{
public:
    jobject mRef;
    bool mIsLocal;
    JNIEnv* mEnv;
    JNITempObjectRef(JNIEnv* env,jobject ref,bool isLocal=true){
        mEnv = env;
        mIsLocal = isLocal;
        mRef = ref;
    }
    JNITempObjectRef(jobject ref,bool isLocal=true){
        new(this) JNITempObjectRef(JniHelper::getEnv(),ref,isLocal);
    }
    ~JNITempObjectRef(){
        if(mIsLocal){
            mEnv->DeleteLocalRef(mRef);
        }else{
            mEnv->DeleteGlobalRef(mRef);
        }
    }
    jobject get(){
        return mRef;
    }
private:
    JNITempObjectRef(const JNITempObjectRef& rh){
    }
    JNITempObjectRef& operator =(const JNITempObjectRef& rh){
        return *this;
    }
    JNITempObjectRef(JNITempObjectRef&& rh){
    }
};
#endif // JNIDATADEFS_H

