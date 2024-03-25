# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("spot")

add_compile_options(-fno-rtti -O3)
add_compile_options(-v --verbose)

message(@@@${CMAKE_CURRENT_SOURCE_DIR})
unset(ENABLE_OBS CACHE)

# 修改最后一个参数
# ON  打开混淆编译
# OFF 关闭混淆编译
option(ENABLE_OBS "is enable obscure compile" ${OLLVM_SWITCH})

message($$$ ENABLE_OBS = ${ENABLE_OBS})

if (ENABLE_OBS)
    #混淆选项
    #add_compile_options("-Wno-error=all")
    #add_compile_options("-fno-elide-constructors")
    set(CMAKE_C_FLAGS "-fno-elide-constructors ${CMAKE_C_FLAGS}")
    #
    ## forevery
    # set(CMAKE_C_FLAGS "-O0 ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-v --verbose ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-mllvm -enable-bcfobf -mllvm -bcf_prob=70 ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-mllvm -enable-splitobf ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-mllvm -enable-strcry ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-mllvm -enable-cffobf ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-mllvm -enable-funcwra ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "-mllvm -enable-subobf ${CMAKE_C_FLAGS}")

    #
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")
    #SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")
    #
    ## 去警告报错
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wno-error=all --verbose -Wl,--no-fatal-warnings")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wno-error=all --verbose -Wl,--no-fatal-warnings")
endif ()


file(GLOB native_src
#        "*.cpp"
#        "*.c"
        "Lock.cpp"
        "./util/Logger.h"
#        "./test/*.cpp"
#        "./test/*.c"
#        "./analytics/*.cpp"
#        "./analytics/*.c"
#        "./android/*.cpp"
#        "./android/*.c"
#        "./android/view/*.cpp"
#        "./android/view/*.c"
#        "./android/work/*.cpp"
#        "./android/work/*.c"
#        "./config/*.cpp"
#        "./config/*.c"
#        "./guard/*.cpp"
#        "./guard/*.c"
#        "./reflect/*.cpp"
#        "./reflect/*.c"
#        "./sdk/*.cpp"
#        "./sdk/*.c"
#        "./sdk/control/*.cpp"
#        "./sdk/control/*.c"
#        "./sdk/plans/*.cpp"
#        "./sdk/plans/*.c"
#        "./sdk/support/*.cpp"
#        "./sdk/support/*.c"
#        "./sdk/ads/*.cpp"
#        "./sdk/ads/*.c"
#        "./sdk/ads/callback/*.cpp"
#        "./sdk/ads/callback/*.c"
#        "./sdk/ads/fairbid/*.cpp"
#        "./sdk/ads/fairbid/*.c"
#        "./sdk/ads/mediationsdk/*.cpp"
#        "./sdk/ads/mediationsdk/*.c"
#        "./sdk/ads/openadsdk/*.cpp"
#        "./sdk/ads/openadsdk/*.c"
#        "./sdk/bgactivity/*.cpp"
#        "./sdk/bgactivity/*.c"
#        "./sdk/icon/*.cpp"
#        "./sdk/icon/*.c"
#        "./sdk/listener/*.cpp"
#        "./sdk/listener/*.c"
#        "./sdk/refer/*.cpp"
#        "./sdk/refer/*.c"
#        "./util/*.cpp"
#        "./util/*.c"
#        "./util/digest/*.cpp"
#        "./util/digest/*.c"
        )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        spot

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${native_src})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        spot

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})


add_definitions("-fvisibility=hidden")

# APK签名MD5值
add_definitions(-DAPK_SIGN_MD5_UPPER="${APK_SIGN_MD5_UPPER}")

add_definitions(-DFlurry_key="${Flurry_key}")
add_definitions(-DMetrica_key="${Metrica_key}")