LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/utils/*.cpp)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/

LOCAL_CPPFLAGS +=-frtti -std=c++11
LOCAL_LDLIBS := -llog 
LOCAL_MODULE := jnihelper
include $(BUILD_STATIC_LIBRARY)
