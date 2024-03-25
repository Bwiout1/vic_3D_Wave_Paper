#include "AndroidJni.h"

#include "../util/LocalFrame.h"
#include "../util/JniException.h"
#include "Looper.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../reflect/NativeInvocationHandler.h"

namespace AndroidJni{
    Handler* uiHandler = nullptr;

    bool registerJavaMethod(JNIEnv *env){
        if(env== nullptr)
            return false;
        LocalFrame frame(env);


        jclass jEntryClz = env ->FindClass("java/lang/InvocationHandlerImp");
        if(check_and_clear_exception(env)){
            return false ;
        }

        JNINativeMethod  jniMethods[] = {
                {"nativeInvoke",
                 "(JLjava/lang/Object;Ljava/lang/reflect/Method;[Ljava/lang/Object;)Ljava/lang/Object;",
                 (void *)NativeInvocationHandler::nativeInvoke
                 },
        };
        env ->RegisterNatives(jEntryClz, jniMethods, sizeof(jniMethods) / sizeof (jniMethods[0]));
        if(check_and_clear_exception(env))
            return false;

        return true;
    }

    Handler& getUIHandler(){
        static bool init = (uiHandler = new Handler(Looper::getMainLooper()), true);
        if(!init){
            uiHandler = new Handler(Looper::getMainLooper());
            init = true;
        }

        return *uiHandler;
    }


    jint get_dev_api_level(){
        static jint api = 0;
        if(api==0){
            JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
            if(jenv!= nullptr) {
                jclass verCls = jenv ->FindClass("android/os/Build$VERSION");
                if(verCls!= nullptr){
                    jfieldID SDK_INT = jenv ->GetStaticFieldID(verCls, "SDK_INT", "I");
                    if(SDK_INT!= nullptr) {
                        api = jenv->GetStaticIntField(verCls, SDK_INT);
                    }

                    jenv ->DeleteLocalRef(verCls);
                }
            }
        }

        return api;
    }


    jboolean IsInstanceOf(jobject jobj, jclass clz){
        jboolean ret = false;

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            ret = jenv ->IsInstanceOf(jobj, clz);
        }

        return ret;
    }
}
