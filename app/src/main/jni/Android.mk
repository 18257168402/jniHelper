LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := testlib
LOCAL_CFLAGS    := -Werror
LOCAL_CPPFLAGS +=-frtti -std=c++11

#LOCAL_CPPFLAGS += -D__DEBUG__
#LOCAL_CFLAGS += -D__DEBUG__

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/app/*.cpp)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS := -llog -landroid
LOCAL_LDLIBS += -latomic
#-lGLESv2 -ldl -lc -lm
LOCAL_C_INCLUDES += $(LOCAL_PATH)/app
			
LOCAL_STATIC_LIBRARIES   :=   jnihelper

include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))

