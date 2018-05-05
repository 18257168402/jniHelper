
cmake_minimum_required(VERSION 3.4.1)

FILE(GLOB_RECURSE srcList "*.cpp")

#FOREACH(src ${srcList})
#        MESSAGE("finded: " ${src})
#ENDFOREACH(src ${srcList})

add_library( jnihelper
             STATIC
             ${srcList} )

find_library( log-lib log )
target_link_libraries(jnihelper ${log-lib} )

