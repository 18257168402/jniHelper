#include "./testlib.h"
#include"LogUtil.h"
#include"_native_defs.h"
#include<thread>
#include<pthread.h>
/**
 * jstring的几种转换形式
 */
void testStrfunc(JNIEnv* env,jobject obj,jstring str,jint ivalue){
    JNITempCStr tempCstr(str);//利用JNITempCStr对象，函数结束的时候释放cstr内存
    const char* cstr1 = tempCstr.get();//获取cstr
    const char* cstr2 = (const char*)tempCstr;//隐式转换

    LOGE("testfunc str1:%s str2:%s str3:%s i:%d",
         cstr1,cstr2,
         (char*)JNITempCStr(str),//临时对象的使用,打印结束释放cstr内存
         ivalue);
}

jstring getString(JNIEnv* env,jobject obj){
    const char* cstr = "hello java!";
    return JniObject::cstringToJ(cstr).newStrLocalRef();//对象返回java层使用，必须新生成一个ref
}
jobject getObject(JNIEnv* env,jobject obj){
    JniObject jobj(ClassCore(TestObject));
    return jobj
            .getStringField("str").set("hello java!")
            ->getIntField("ivalue").set(2048)
            ->newLocalrefObj();//链式赋值,爽到爆
}

void testCalljava(JNIEnv* env,jobject obj){
    JniObject jobj(obj);
    const char* cstr = "hello java!";
    jobj.getVoidMethod("onFromNative",SIG_VoidMethod_2(SIG_Str,SIG_Int))
            .call(NULL,JniObject::cstringToJ(cstr).getstr(),2048);
        //对象通过函数发回java层，函数调用期间引用不会销毁，因此不需要生成ref
}
void testCallStaticjava(JNIEnv* env,jobject obj){
    JniObject jobj(ClassCore(MainActivity));
    const char* cstr = "hello java!";
    jobj.getStaticVoidMethod("onStaticFromNative",SIG_VoidMethod_2(SIG_Str,SIG_Int))
            ->call(NULL,JniObject::cstringToJ(cstr).getstr(),2048);
}

jobject callback;
std::mutex CbLock;
void* thr_fn(void* args){
    JNIEnv* env = JniHelper::getEnv();//取回env，这里自动规避线程attach问题

    CbLock.lock();
    if(JNICheckIfFreed(env,callback)){//检查java对象是否还存在
        CbLock.unlock();
        return NULL;
    }
    JniObject jobj(callback);
    CbLock.unlock();

    const char* cstr = "hello java!";
    jobj.getVoidMethod("onTestCallback",SIG_VoidMethod_2(SIG_Str,SIG_Int))
            .call(NULL,JniObject::cstringToJ(cstr).getstr(),2048);
    return NULL;
}

void testListener(JNIEnv* env,jobject obj,jobject listener){
    std::lock_guard<std::mutex> lck(CbLock);//锁是要加的，万一崩溃了呢

    if(callback!=NULL && !JNICheckIfFreed(env,callback)){
        GlobalWeakRefRemove(env,callback);//移除前一个监听
        callback = NULL;
    }
    callback = GlobalWeakRefAdd(env,listener);//弱全局引用

    pthread_t thread;
    pthread_create(&thread,NULL,thr_fn,NULL);
}

