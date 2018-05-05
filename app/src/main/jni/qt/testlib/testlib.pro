QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += \
    ../../app \
    ../../jniHelper

SOURCES += \
    ../../app/_native.cpp \
    ../../app/testlib.cpp \
    ../../jniHelper/JniClass.cpp \
    ../../jniHelper/JniField.cpp \
    ../../jniHelper/jniglobaldef.cpp \
    ../../jniHelper/JniHelper.cpp \
    ../../jniHelper/JniMethod.cpp \
    ../../jniHelper/JniObject.cpp \
    ../../jniHelper/utils/jniutils.cpp

HEADERS += \
    ../../app/_native_method_map.h \
    ../../app/LogUtil.h \
    ../../app/testlib.h \
    ../../jniHelper/JniClass.h \
    ../../jniHelper/JniDataDef_in.h \
    ../../jniHelper/JniDataDefs.h \
    ../../jniHelper/JniField.h \
    ../../jniHelper/JniGlobalDef.h \
    ../../jniHelper/JniHelper.h \
    ../../jniHelper/JniMethod.h \
    ../../jniHelper/JniObject.h \
    ../../jniHelper/utils/jniutils.h \
    ../../app/_native_defs.h

DISTFILES += \
    ../../app/CMakeLists.txt \
    ../../jniHelper/CMakeLists.txt \
    ../../jniHelper/Android.mk \
    ../../CMakeLists.txt \
    ../../Android.mk \
    ../../Application.mk
