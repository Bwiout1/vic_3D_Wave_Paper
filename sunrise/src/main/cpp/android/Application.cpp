#include <cstring>
#include "Application.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/LocalFrame.h"
#include "../util/Logger.h"
#include "../reflect/JavaMethod.h"


bool Application::inited = false;
jclass Application::clsZ = nullptr;
Application* Application::inst = nullptr;

void Application::init() {
    if(!inited) {
        LOGV("Application::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            inited = true;

            jclass tmp = jenv->FindClass("android/app/Application");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Application& Application::getInstance() {
    if(!inited){
        init();
    }

    if(inst == nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            LocalFrame frame(jenv);

            jclass activity_thread_clz = jenv->FindClass("android/app/ActivityThread");
            jmethodID currentApplication = jenv->GetStaticMethodID(
                    activity_thread_clz, "currentApplication", "()Landroid/app/Application;");

            jobject app = jenv->CallStaticObjectMethod(activity_thread_clz,
                                                       currentApplication);

            inst = new Application(app);
        }
    }

    return *inst;
}

Application::Application(const jobject &core) : Context(core){
    if(!inited){
        init();
    }

    LOGV("Application::Application, constructor");
}

void Application::registerActivityLifecycleCallbacks(jobject callback) {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && callback!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ,
                                              "registerActivityLifecycleCallbacks",
                                              "(Landroid/app/Application$ActivityLifecycleCallbacks;)V") ;

        jenv ->CallVoidMethod(mCore, method, callback);
    }
}


///////////////////////////Application::ActivityLifecycleCallbacks//////////////////////////////////
bool Application::ActivityLifecycleCallbacks::inited = false;
jclass Application::ActivityLifecycleCallbacks::clsZ = nullptr;


void Application::ActivityLifecycleCallbacks::init() {
    if (!inited) {
        LOGV("Application::ActivityLifecycleCallbacks::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/Application$ActivityLifecycleCallbacks");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}

Application::ActivityLifecycleCallbacks::ActivityLifecycleCallbacks()
        : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv ->NewGlobalRef(generateProxy(clsZ)) ;
    }
}

jobject Application::ActivityLifecycleCallbacks::invoke(jobject method, jobjectArray objects) {
    JNIEnv *jenv ;

    if(method!= nullptr && objects!= nullptr && (jenv =PthreadJNIEnvHelper::getJNIEnv()) != nullptr) {
        jstring method_name = JavaMethod::getName(method);
        int paramNum = jenv ->GetArrayLength(objects);

        if (method_name != nullptr && paramNum >= 1) {
            const char * name_c = jenv ->GetStringUTFChars(method_name, nullptr);

            jobject firstParam = jenv ->GetObjectArrayElement(objects, 0);


            if(paramNum==2 && !memcmp("onActivityCreated", name_c, sizeof("onActivityCreated"))) {
                jobject secondParam = jenv ->GetObjectArrayElement(objects, 1);
                onActivityCreated(firstParam, secondParam);

            } else if(paramNum==1 && !memcmp("onActivityStarted", name_c, sizeof("onActivityStarted"))) {
                onActivityStarted(firstParam);

            } else if(paramNum==1 && !memcmp("onActivityResumed", name_c, sizeof("onActivityResumed"))) {
                onActivityResumed(firstParam);

            } else if(paramNum==1 && !memcmp("onActivityPaused", name_c, sizeof("onActivityPaused"))) {
                onActivityPaused(firstParam);

            } else if(paramNum==1 && !memcmp("onActivityStopped", name_c, sizeof("onActivityStopped"))) {
                onActivityStopped(firstParam);

            } else if(paramNum==1 && !memcmp("onActivityDestroyed", name_c, sizeof("onActivityDestroyed"))) {
                onActivityDestroyed(firstParam);
            }

            jenv ->ReleaseStringUTFChars(method_name, name_c);
        }
    }

    return nullptr;
}
