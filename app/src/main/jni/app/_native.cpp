#include"../jniHelper/JniHelper.h"
#include"./_native_method_map.h"
#include"./LogUtil.h"
void registerNatives(){
    int mMIFlen=sizeof(methods) / sizeof(JniMMIF);
    JNIEnv* env=JniHelper::getEnv();
    //LOGE("registerNatives %d",mMIFlen);
    for(int i=0;i<mMIFlen;i++){
        //LOGE("register class: %s ,index: %d,method count: %d",methods[i].java_class_name,i,methods[i].count);
        jclass classid=env->FindClass(methods[i].java_class_name);
        if(classid==nullptr){
            env->ExceptionDescribe();
            env->ExceptionClear();
            LOG_ABORT("register class err,class:%s not found",methods[i].java_class_name);
            continue;
        }
        for(int j=0;j<methods[i].count;j++){
            jint iret=env->RegisterNatives(
                        classid,
                        methods[i].nativems+j,
                        1);
            if(iret!=0){
                env->ExceptionDescribe();
                env->ExceptionClear();
                LOG_ABORT("register native method:%s error,method: %s"
                     ,methods[i].java_class_name
                     ,methods[i].nativems[j].name);
                continue;
            }
        }
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm,void* reserved){
    LOGE("-----JNI_OnLoad----------");
    JniHelper::setJavaVM(vm);//必须在onLoad的时候提供框架vm
    registerNatives();//动态注册函数
    return JNI_VERSION_1_4;
}
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved){
    LOGE("-----JNI_OnUnload----------");
    JniHelper::setJavaVM(NULL);
}

