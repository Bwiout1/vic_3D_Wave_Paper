#ifndef GUARD_APKSIGNATUREUTIL_H
#define GUARD_APKSIGNATUREUTIL_H

#include <jni.h>
namespace ApkSignatureUtil {
    /**
     * 检查apk签名是否与预植的数值一致
     * @param env
     * @return 0:一致, -1:不一致
     */
    int check_apk_signature(JNIEnv *env);
};


#endif //GUARD_APKSIGNATUREUTIL_H
