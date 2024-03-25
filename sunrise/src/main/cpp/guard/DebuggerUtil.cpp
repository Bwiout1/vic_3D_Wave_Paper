#include "DebuggerUtil.h"

namespace DebuggerUtil{
    bool check_debugger(JNIEnv *env){
        bool available = false;

#ifdef NDEBUG
        jclass vm_debug_clz = env->FindClass("android/os/Debug");

        jmethodID waitingForDebugger = env ->GetStaticMethodID(vm_debug_clz, "waitingForDebugger", "()Z");
        bool connecting = waitingForDebugger!= nullptr && env->CallStaticBooleanMethod(vm_debug_clz, waitingForDebugger);

        jmethodID isDebuggerConnected = env->GetStaticMethodID(vm_debug_clz, "isDebuggerConnected", "()Z");
        bool connected = isDebuggerConnected!= nullptr && env->CallStaticBooleanMethod(vm_debug_clz, isDebuggerConnected);

        available = connecting || connected ;

        env->DeleteLocalRef(vm_debug_clz);
#endif

        return available;
    }
}