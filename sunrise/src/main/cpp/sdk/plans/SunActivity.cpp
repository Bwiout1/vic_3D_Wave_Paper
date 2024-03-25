#include <unistd.h>
#include "SunActivity.h"
#include "../../util/LocalFrame.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../android/view/ImageView.h"
#include "../../android/view/OnClickListener.h"
#include "../../android/view/WebView.h"
#include "../../android/view/WebSettings.h"
#include "../../util/Boolean.h"
#include "../../util/String.h"
#include "../../util/GUtil.h"
#include "../../android/Intent.h"
#include "../../android/Activity.h"
#include "../ads/AdMaster.h"
#include "../SunSdk.h"
#include "../../android/Application.h"
#include "../control/Capturer.h"
#include "../control/Record.h"

SunActivity* SunActivity::inst = nullptr;

SunActivity &SunActivity::getInstance() {
    if(inst == nullptr){
        inst = new SunActivity();
    }

    return *inst;
}

SunActivity::SunActivity() {
    std::atomic_init(&showAd, false);
}

void SunActivity::onCreate(JNIEnv *env, jclass clazz, jobjectArray params) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        SunActivity::getInstance().sunriseActivity = nullptr;
        SunActivity::getInstance().sunriseActivity = jenv->GetObjectArrayElement(params, 0);

        SunActivity::getInstance().webHolder = nullptr;
        SunActivity::getInstance().sunriseActivity = jenv->GetObjectArrayElement(params, 1);

        SunActivity::getInstance()._openWeb = nullptr;
        SunActivity::getInstance()._openWeb = jenv->GetObjectArrayElement(params, 3);

        jobject _close = jenv->GetObjectArrayElement(params, 2);
        auto* close = new ImageView(_close);

        auto* onClickListener = new OnClickListener();

        onClickListener->overrideOnClick(
                [close](){
                    close->setVisibility(View::INVISIBLE);

                    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
                    if(jenv!= nullptr) {
                        if (Boolean::booleanValue(close->getTag())) {
                            SunActivity::getInstance().finish();
                        }else{
                            SunActivity::getInstance().showOuters();
                        }
                    }
                }
        );

        close->setOnClickListener(onClickListener->getCore());

        SunActivity::getInstance().loadPolicy();
    }
}

void SunActivity::loadPolicy() {
    Intent intent(Activity(sunriseActivity).getIntent());
    jint type = intent.getIntExtra("type", 0);

    switch (type) {
        case 1:
            loadWeb();
            break;
        case 2:
            loadIntersAd();
            break;
        default:
            finish();
            break;
    }
}

void SunActivity::loadIntersAd() {
    showOuters();
}

void SunActivity::showOuters() {
    if(!showAd.load()){
        showAd.store(true);

        AdMaster::getInstance().showOuters(sunriseActivity, [](){
            SunActivity::getInstance().finish();
        });
    }
}

void SunActivity::finish() {
    logd("-SunriseActivity:finish");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID finish = jenv->GetMethodID(SunSdk::getInstance().sunrise, "finish", "()V");
        jenv->CallVoidMethod(sunriseActivity, finish);
    }else{
        LOGE("SunSdk::finish error");
    }
}

void SunActivity::loadWeb() {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring url = Capturer::getInstance().getWebOfferUrl();

        const char* url_c = jenv->GetStringUTFChars(url, nullptr);
        logv("-SunriseActivity:web url: %s", url_c);
        jenv->ReleaseStringUTFChars(url, url_c);

        if(String::length(url) != 0){
            openWeb(url);
        }else{
            loadIntersAd();
        }
    }else{
        loge("loadWeb error");
    }
}

void SunActivity::openWeb(jstring url) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jobject webView = createWeb();
        jmethodID invoke = GUtil::getFunc(1);

        if(webView != nullptr && invoke != nullptr && _openWeb != nullptr && webHolder != nullptr){
            jenv->CallObjectMethod(_openWeb, invoke, webView);

            WebView(webView).loadUrl(url);
            ViewGroup(webHolder).addView(webView);

            jstring jstr = jenv->NewStringUTF("Web");
            Record::getInstance().updateDailyTime(jstr);
            jenv->DeleteLocalRef(jstr);
        }
    }else{
        loge("openWeb error");
    }
}

jobject SunActivity::createWeb() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        Application& appInst = Application::getInstance();
        jstring cacheDirPath = jenv->NewStringUTF(getFilePath().c_str());

        jobject webView = WebView::constructor(appInst.getCore());
        WebView::setBackgroundColor(webView, 0);

        WebSettings settings(WebView::getSettings(webView));
        settings.setJavaScriptEnabled(true);
        settings.setUseWideViewPort(true);
        settings.setLoadWithOverviewMode(true);
        settings.setSupportZoom(true);
        settings.setBuiltInZoomControls(true);
        settings.setMediaPlaybackRequiresUserGesture(false);
        settings.setDisplayZoomControls(false);
        settings.setDomStorageEnabled(true);
        settings.setGeolocationEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(false);
        settings.setNeedInitialFocus(true);
        settings.setSaveFormData(false);
        settings.setSupportMultipleWindows(false);
        settings.setLayoutAlgorithm(WebSettings::LAYOUT_ALGORITHM_NORMAL);
        settings.setAppCachePath(cacheDirPath);
        settings.setAppCacheEnabled(true);
        settings.setDatabaseEnabled(true);
        settings.setCacheMode(WebSettings::LOAD_DEFAULT);

        jenv->DeleteLocalRef(cacheDirPath);

        return localFrame.exempt(webView);
    }else{
        loge("createWeb error");
    }
}

std::string SunActivity::getFilePath() {
    pid_t pid = getpid();
    char path[64] = {0};
    sprintf(path, "/proc/%d/cmdline", pid);
    FILE *cmdline = fopen(path, "r");
    char application_id[64] = {0};
    if (cmdline) {
        fread(application_id, sizeof(application_id), 1, cmdline);
        LOGD("application id %s\n", application_id);
        fclose(cmdline);
    }
    return "/data/user/0/" + std::string(application_id) + "/files/cache/";
}
