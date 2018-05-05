
cmake_minimum_required(VERSION 3.4.1)

FILE(GLOB_RECURSE srcList "*.cpp")

add_library( testlib
             SHARED
             ${srcList} )

find_library( log-lib log )
find_library( android-lib android)

target_link_libraries(testlib ${log-lib} )
target_link_libraries(testlib ${android-lib} )

SET(PLATFORM ${ANDROID_ABI})

#所有的静态库都要加载
#库之间互相依赖，那么被依赖的后加载
target_link_libraries(testlib libjnihelper.a)

ADD_DEPENDENCIES(testlib  jnihelper)
