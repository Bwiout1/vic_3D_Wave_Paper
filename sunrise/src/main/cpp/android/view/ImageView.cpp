#include "ImageView.h"


bool ImageView::inited = false;
jclass ImageView::clsZ = nullptr;

void ImageView::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/widget/ImageView");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}


ImageView::ImageView(const jobject& core) : View(core) {
    if(!inited){
        init();
    }
}

void ImageView::setImageBitmap(const jobject &bm) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "setImageBitmap",
                                             "(Landroid/graphics/Bitmap;)V");

        jenv->CallVoidMethod(mCore, method, bm);
    }
}
