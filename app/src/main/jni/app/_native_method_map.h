#ifndef _NATIVE_METHOD_MAP_H
#define _NATIVE_METHOD_MAP_H

#include"testlib.h"
#include"JniObject.h"
#include"JniHelper.h"
#include"_native_defs.h"


typedef struct JniMethodMapInfo{
    const char* java_class_name;
    JNINativeMethod *nativems;
    int count;
}JniMMIF,*PJniMMIF;

JNINativeMethod test_methods[]={
    {
       "testStrfunc",
        SIG_VoidMethod_2(SIG_Str,SIG_Int),
        (void*)testStrfunc
    },
    {
        "getString",
        SIG_Method_0(SIG_Str),
        (void*)getString
    },
    {
        "getObject",
        SIG_Method_0(SigCore(TestObject)),
        (void*)getObject
    },
    {
        "testCallJava",
        SIG_VoidMethod_0(),
        (void*)testCalljava
    },
    {
        "testCallStaticJava",
        SIG_VoidMethod_0(),
        (void*)testCallStaticjava
    },
    {
        "testListener",
        SIG_VoidMethod_1(SigCore(TestListener)),
        (void*)testListener
    }
};

JniMMIF methods[]={
    {//"ble.lss.com.jnihelper.MainActivity"
        ClassCore(MainActivity),
        test_methods,
        sizeof(test_methods)/sizeof(JNINativeMethod)
    }
};

#endif // _NATIVE_METHOD_MAP_H

