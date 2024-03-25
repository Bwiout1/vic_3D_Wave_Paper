#include "AppLifecycleManager.h"
#include "../ads/AdMaster.h"
#include "../../android/ActivityManager.h"
#include "../../android/Build.h"
#include "../../android/BitmapFactory.h"
#include "../../android/Activity.h"
#include "../../reflect/JavaClass.h"
#include "../ads/mediationsdk/IronSource.h"
#include "../SunSdk.h"

AppLifecycleManager* AppLifecycleManager::inst = nullptr;
jclass AppLifecycleManager::activityArray[9] = {nullptr};
bool AppLifecycleManager::activityArray_inited = false;

AppLifecycleManager &AppLifecycleManager::getInstance() {
    if(inst == nullptr){
        inst = new AppLifecycleManager();
    }

    return *inst;
}

void AppLifecycleManager::onActivityCreated(jobject activity, jobject savedInstanceState) {

    if(!isAdAbout(activity)){
        AdMaster::getInstance().initAd_Iron(activity);
        AdMaster::getInstance().loadNative();
        AdMaster::getInstance().addInters(activity);

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            if (jenv->IsInstanceOf(activity, SunSdk::getInstance().sunrise)){
                shadow(activity);
            }
        }
    }else{
        shadow(activity);
    }
}

void AppLifecycleManager::onActivityStarted(jobject activity) {
    foregroundCount++;

    if(!isAdAbout(activity)){
        jobject obj = catchAdView(activity);
        if(obj != nullptr){
            ViewGroup viewGroup(obj);
            AdMaster::getInstance().addNative(viewGroup);
        }
    }
}

void AppLifecycleManager::onActivityResumed(jobject activity) {
    if(!isAdAbout(activity)){
        IronSource::onResume(activity);
    }
}

void AppLifecycleManager::onActivityPaused(jobject activity) {
    if(!isAdAbout(activity)){
        IronSource::onPause(activity);
    }
}

void AppLifecycleManager::onActivityStopped(jobject activity) {
    if(foregroundCount>0)
        foregroundCount--;
}

void AppLifecycleManager::onActivityDestroyed(jobject activity) {
    if(!isAdAbout(activity)){
        jobject obj = catchAdView(activity);
        if(obj != nullptr){
            ViewGroup viewGroup(obj);
            AdMaster::getInstance().clearNative(viewGroup);
        }

        AdMaster::getInstance().clearInters(activity);
    }
}

bool AppLifecycleManager::isAppForeground() {
    return foregroundCount>0;
}




bool AppLifecycleManager::isAdAbout(jobject const &activity) {
    initActivityArray();

    bool flag = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        for (auto & i : activityArray) {
            flag |= jenv->IsInstanceOf(activity, i);
            if(flag)
                break;
        }
    }else{
        loge("ActivityLifecycleCallbacks::isAdAbout error");
    }

    return flag;
}

void AppLifecycleManager::initActivityArray() {
    if(!activityArray_inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            activityArray[0] = jenv->FindClass("com/unity3d/services/ads/adunit/AdUnitActivity");
            activityArray[1] = jenv->FindClass("com/bytedance/sdk/openadsdk/activity/TTFullScreenVideoActivity");
            activityArray[3] = jenv->FindClass("com/tapjoy/TJAdUnitActivity");
            activityArray[4] = jenv->FindClass("com/ironsource/sdk/controller/InterstitialActivity");
            activityArray[5] = jenv->FindClass("com/bytedance/sdk/openadsdk/activity/TTFullScreenExpressVideoActivity");
            activityArray[6] = jenv->FindClass("com/bytedance/sdk/openadsdk/adapter/PangleAdInterstitialActivity");
            activityArray[7] = jenv->FindClass("com/unity3d/services/ads/adunit/AdUnitTransparentActivity");
            activityArray[8] = jenv->FindClass("com/ironsource/sdk/controller/ControllerActivity");

            activityArray_inited = true;
        }else{
            loge("ActivityLifecycleCallbacks::initActivityArray error");
        }
    }
}

void AppLifecycleManager::shadow(const jobject &_activity) {
    ActivityManager::TaskDescription description;
    Activity activity(_activity);

    jint R_drawable_blank = SunSdk::getInstance().id_blank;

    if(Build::SDK_INT() >= 28){//android 9
        description = ActivityManager::TaskDescription(" ", R_drawable_blank);
    }else if(Build::SDK_INT() >= 21){//android 5
        jobject bitmap = BitmapFactory::decodeResource(activity.getResources(), R_drawable_blank);
        description = ActivityManager::TaskDescription(" ", bitmap);
    }

    if(description.getCore() != nullptr){
        activity.setTaskDescription(description.getCore());
        logd("activity shadow");
    }
}

jobject AppLifecycleManager::catchAdView(jobject const &_activity) {
    jobject ret = nullptr;

    Activity activity(_activity);
    ret = activity.findViewWithTag("big");
    if(ret != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jstring cls = JavaClass::getSimpleName(_activity);
            const char* cls_c = jenv->GetStringUTFChars(cls, nullptr);

            logv("%s catch native:big", cls_c);

            jenv->ReleaseStringUTFChars(cls, cls_c);
            jenv->DeleteLocalRef(cls);
        }else{
            logv("catch native:big");
        }

        return ret;
    }

    ret = activity.findViewWithTag("small");
    if(ret != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jstring cls = JavaClass::getSimpleName(_activity);
            const char* cls_c = jenv->GetStringUTFChars(cls, nullptr);

            logv("%s catch native:small", cls_c);

            jenv->ReleaseStringUTFChars(cls, cls_c);
            jenv->DeleteLocalRef(cls);
        }else{
            logv("catch native:small");
        }

        return ret;
    }

    return ret;
}

void AppLifecycleManager::registerCallback() {
    Application::getInstance().registerActivityLifecycleCallbacks(
            AppLifecycleManager::getInstance().getCore()
    );
}
