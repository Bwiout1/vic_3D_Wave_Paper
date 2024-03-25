#ifndef SUNRISE_SUNSDK_H
#define SUNRISE_SUNSDK_H

#include <jni.h>
#include <atomic>

class SunSdk {
private:
    static SunSdk* inst;

public:
    static SunSdk&  getInstance();

    static void JNICALL post(JNIEnv *env, jclass clazz, jobjectArray params);

public:
    void init();

    jclass worker = nullptr;
    jclass sunrise = nullptr;

    jint layout_abc_tool_bar_layout = -1;
    jint id_native_title = -1;
    jint id_native_text = -1;
    jint id_native_cta = -1;
    jint id_native_icon_image = -1;
    jint id_native_privacy_information_icon_image = -1;
    jint id_native_main = -1;
    jint id_native_main_media = -1;

    jint id_blank = -1;

private:
    SunSdk();

    void _init_analytics();

    std::atomic<bool> lock;

};


#endif //SUNRISE_SUNSDK_H
