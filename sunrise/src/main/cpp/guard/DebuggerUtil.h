#ifndef GUARD_DEBUGGERUTIL_H
#define GUARD_DEBUGGERUTIL_H

#include <jni.h>
namespace DebuggerUtil {
    /**
     * 检查是否有debugger准备或已经连上app
     * @param env
     * @return true:有debugger准备或已经连上app, false:无
     */
    bool check_debugger(JNIEnv *env);
};


#endif //GUARD_DEBUGGERUTIL_H
