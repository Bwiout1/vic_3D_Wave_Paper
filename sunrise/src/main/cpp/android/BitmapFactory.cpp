#include "BitmapFactory.h"
#include "../util/PthreadJNIEnvHelper.h"

bool BitmapFactory::inited = false;
jclass BitmapFactory::clsZ = nullptr;

void BitmapFactory::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/graphics/BitmapFactory");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject BitmapFactory::decodeResource(jobject const &resources, jint id) {
    init();

    jobject ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "decodeResource", "(Landroid/content/res/Resources;I)Landroid/graphics/Bitmap;");
        ret = jenv->CallStaticObjectMethod(clsZ, method, resources, id);
    }

    return ret;
}
