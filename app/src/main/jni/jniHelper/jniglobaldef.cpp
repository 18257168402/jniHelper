#include"JniGlobalDef.h"
#include<jni.h>
#include"./utils/jniutils.h"
#include<mutex>
int localRefCount;
int globalRefCount;
std::mutex mRefLock;
std::mutex mGlobalRefLock;
void localRefCountAdd(){
//    mRefLock.lock();
//    localRefCount++;
//    mRefLock.unlock();
//    JniLOGE("cur local ref:%d",(int)localRefCount);
}

jobject LocalRefAdd(JNIEnv* env,jobject obj){
//     mRefLock.lock();
//    localRefCount++;
//     mRefLock.unlock();
//    JniLOGE("cur local ref:%d",(int)localRefCount);
    return env->NewLocalRef(obj);
}

void LocalRefRemove(JNIEnv* env,jobject obj){
//     mRefLock.lock();
//    localRefCount--;
//    mRefLock.unlock();
//    JniLOGE("cur local ref:%d",(int)localRefCount);
    return env->DeleteLocalRef(obj);
}
jobject GlobalRefAdd(JNIEnv* env,jobject obj){
//    mGlobalRefLock.lock();
//    globalRefCount++;
//    mGlobalRefLock.unlock();
//   JniLOGE("cur global ref:%d",(int)globalRefCount);
    return env->NewGlobalRef(obj);
}

jobject GlobalWeakRefAdd(JNIEnv* env,jobject obj){
    return env->NewWeakGlobalRef(obj);
}
void GlobalWeakRefRemove(JNIEnv* env,jobject obj){
    return env->DeleteWeakGlobalRef(obj);
}
bool JNICheckIfFreed(JNIEnv* env,jobject weakref){
    return env->IsSameObject(weakref,NULL);
}
void GlobalRefRemove(JNIEnv* env,jobject obj){
//    mGlobalRefLock.lock();
//    globalRefCount--;
//    mGlobalRefLock.unlock();
//    JniLOGE("cur global ref:%d",(int)globalRefCount);
    return env->DeleteGlobalRef(obj);
}
