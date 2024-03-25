#ifndef AND_ANDROIDCLASSWRAPPERBASE_H
#define AND_ANDROIDCLASSWRAPPERBASE_H

#include <jni.h>
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"
#include "../util/DeferredGC.h"

class AndroidClassWrapperBase : public virtual DeferredGC::VirtualDestructor{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    jobject getCore();

    jint hashCode();

protected:
    jobject mCore = nullptr;

    ~AndroidClassWrapperBase(){
        LOGV("AndroidClassWrapperBase::~AndroidClassWrapperBase");

        if (mCore != nullptr) {
            JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
            if (jenv != nullptr) {
                jenv->DeleteGlobalRef(mCore);
                mCore = nullptr;
            }
        }
    };
};


#endif //AND_ANDROIDCLASSWRAPPERBASE_H
