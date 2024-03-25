#include "Pangle.h"
#include "../control/ControlRoom.h"
#include "../control/Record.h"
#include "../listener/FeedAdListener.h"
#include "../listener/FullScreenVideoAdListener.h"
#include "../listener/AdInteractionListener.h"
#include "openadsdk/AdSlot.h"
#include "openadsdk/TTFeedAd.h"
#include "openadsdk/TTAdSdk.h"
#include "openadsdk/MediationAdapterUtil.h"
#include "openadsdk/TTFullScreenVideoAd.h"
#include "../../android/view/LayoutInflater.h"
#include "../../android/view/TextView.h"
#include "../../android/view/ImageView.h"
#include "../../android/view/OnClickListener.h"
#include "../../android/Activity.h"
#include "../../util/Glide.h"
#include "../../util/ArrayList.h"
#include "../../util/LocalFrame.h"
#include "../../util/String.h"
#include "../SunSdk.h"
#include <pthread.h>
#include <string>
#include <unistd.h>

Pangle* Pangle::inst = nullptr;
Pangle::Pangle() {
    std::atomic_init(&aNative, false);
    std::atomic_init(&aInters, false);
}

Pangle &Pangle::getInstance() {
    if(inst == nullptr){
        inst = new Pangle();
    }

    return *inst;
}

void Pangle::initNext() {
    ttAdNative = TTAdSdk().getAdManager().createAdNative();
    if(Record::getInstance().hasPangle()){
        loadNative();
        loadInters();
    }
}

jboolean Pangle::isLoading() {
    return aNative.load();
}

jstring Pangle::name() {
    jstring ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        ret = jenv->NewStringUTF("Pangle");
    }

    return ret;
}

void Pangle::loadNative() {
    pthread_t tid;
    pthread_create(&tid, nullptr, p_loadNative, this);
}

void *Pangle::p_loadNative(void *args) {
    auto p = (Pangle*)args;
    if(p->hasNative()){
        logd("has native cache");
        return nullptr;
    }

    if(p->ttAdNative != nullptr){
        jstring id = ControlRoom::getInstance().getCapturer().getPangleConfig().getNativeId();

        if(String::length(id) != 0){

            if(!p->aNative.load()){
                p->aNative.store(true);

                AdSlot adSlot(1);
                adSlot.newBuilder()
                        .setCodeId(id)
                        .setSupportDeepLink(true)
                        .setImageAcceptedSize(640, 320)
                        .build();

                if(adSlot.checkSafe()){
                    auto feedAdListener = new FeedAdListener();

                    p->ttAdNative->loadFeedAd(adSlot.getCore(), feedAdListener->getCore());
                    logd("native request");
                }else{
                    loge("native request error");
                }
            }
        }
    }
    return nullptr;
}

void Pangle::loadNextNative() {
    if(Record::getInstance().hasAd() || Record::getInstance().hasWebOffer()){
        usleep(200000);

        logd("load next native");
        loadNative();

//        pthread_t tid;
//        pthread_create(&tid, nullptr, p_loadNextNative, this);
    }
}

//void *Pangle::p_loadNextNative(void *args) {
//    auto p = (Pangle*)args;
//    usleep(200000);
//    logd("load next native");
//
//    p_loadNative(p);
//
//    return nullptr;
//}

void Pangle::fillNative(ViewGroup& container) {
    auto adInteractionListener = new AdInteractionListener("fillNative");
    fillNative2(container, adInteractionListener);
}

void Pangle::fillNative2(ViewGroup &container, AdInteractionListener* listener) {
    jint abc_tool_bar_layout = SunSdk::getInstance().layout_abc_tool_bar_layout;
    jint native_title = SunSdk::getInstance().id_native_title;
    jint native_text = SunSdk::getInstance().id_native_text;
    jint native_cta = SunSdk::getInstance().id_native_cta;
    jint native_icon_image = SunSdk::getInstance().id_native_icon_image;
    jint native_privacy_information_icon_image = SunSdk::getInstance().id_native_privacy_information_icon_image;
    jint native_main = SunSdk::getInstance().id_native_main;
    jint native_main_media = SunSdk::getInstance().id_native_main_media;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject _ttFeedAd = mCacheNative->poll();
    jobject context = container.getContext();
    if(_ttFeedAd != nullptr && context != nullptr && jenv != nullptr){
        LocalFrame localFrame(jenv);

        auto* ttFeedAd = new TTFeedAd(_ttFeedAd);
        LayoutInflater layoutInflater(LayoutInflater::from(container.getContext()));

        jobject _adView = layoutInflater.inflate(abc_tool_bar_layout, container, false);
        View adView(_adView);

        TextView titleView(adView.findViewById(native_title));
        titleView.setText(ttFeedAd->getTitle());

        TextView descripView(adView.findViewById(native_text));
        descripView.setText(ttFeedAd->getDescription());

        TextView ctaView(adView.findViewById(native_cta));
        ctaView.setText(ttFeedAd->getButtonText());

        jobject icon = ttFeedAd->getIcon();
        if(icon != nullptr && ttFeedAd->isValid(icon)){
            Activity activity(context);
            if(!activity.isDestroyed() && !activity.isFinishing()){
                Glide glide = Glide();

                glide.with(context)
                        .load(ttFeedAd->getImageUrl(icon))
                        .into(adView.findViewById(native_icon_image));
            }
        }

        jobject adLogo = ttFeedAd->getAdLogo();
        if(adLogo == nullptr){
            ImageView imageView(adView.findViewById(native_privacy_information_icon_image));
            imageView.setImageBitmap(adLogo);

            auto onClickListener = new OnClickListener();
            onClickListener->overrideOnClick(
                    [ttFeedAd](){
                        ttFeedAd->showPrivacyActivity();
                    }
            );

            imageView.setOnClickListener(onClickListener->getCore());
        }

        ViewGroup mainMediaContainer(adView.findViewById(native_main));
        auto size = (jstring)container.getTag();
        const char* size_c = jenv->GetStringUTFChars(size, nullptr);

        if(std::string(size_c).find("big") == 0){
            mainMediaContainer.setVisibility(View::VISIBLE);

            jint imageMode = ttFeedAd->getImageMode();
            View video(ttFeedAd->getAdView());//it.adView

            if(imageMode == 5 || imageMode == 50){
                if(video.getParent() == nullptr){
                    mainMediaContainer.removeAllViews();
                    mainMediaContainer.addView(video.getCore());
                }
            }else{
                View mediaView(adView.findViewById(native_main_media));
                mediaView.setVisibility(View::VISIBLE);

                MediationAdapterUtil::addNativeFeedMainView(
                        context,
                        imageMode,
                        mediaView.getCore(),
                        video.getCore(),
                        ttFeedAd->getImageList()
                );
            }
        }else{
            mainMediaContainer.setVisibility(View::GONE);
        }

        ArrayList clickViewList(nullptr);
        clickViewList.add(adView.getCore());

        ArrayList creativeViewList(nullptr);
        creativeViewList.add(ctaView.getCore());

        ttFeedAd->registerViewForInteraction(adView.getCore(), clickViewList, creativeViewList, listener->getCore());

        container.removeAllViews();
        container.addView(adView.getCore());

        logd("-pangle: native fill container:%d %s", container.hashCode(), size_c);

        jenv->ReleaseStringUTFChars(size, size_c);

        loadNextNative();
    }
}

jboolean Pangle::hasNative() {
    return !mCacheNative->isEmpty();
}

void Pangle::clearNative(ViewGroup& container) {
//nothing
}

void Pangle::fillCleanNative(ViewGroup &container) {
    auto adInteractionListener = new AdInteractionListener("fillCleanNative");
    fillNative2(container, adInteractionListener);
}


/*-----------------------------------------inters-------------------------------------------------*/


void Pangle::loadInters() {
    pthread_t tid;
    pthread_create(&tid, nullptr, p_loadInters, this);
}

void *Pangle::p_loadInters(void *args) {
    auto p = (Pangle*)args;
    if(p->hasInters()){
        logd("has inters cache");
        return nullptr;
    }

    if(p->ttAdNative != nullptr){

        jstring id = ControlRoom::getInstance().getCapturer().getPangleConfig().getIntersId();
        if(String::length(id) != 0){

            if(!p->aInters.load()){
                p->aInters.store(true);

                AdSlot adSlot(2);
                adSlot.newBuilder()
                    .setCodeId(id)
                    .setSupportDeepLink(true)
                    .setImageAcceptedSize(1080, 1920)
                    .build();

                if(adSlot.checkSafe()){
                    auto fullScreenVideoAdListener = new FullScreenVideoAdListener("loadInters");
                    p->ttAdNative->loadFullScreenVideoAd(adSlot.getCore(), fullScreenVideoAdListener->getCore());
                    //todo:第二个listener

                    logd("inters load");
                }else{
                    loge("inters load error");
                }
            }
        }
    }


    return nullptr;
}

void Pangle::showInters(const jobject& activity, IntersShowListener& callback) {
    jobject _ttFullScreenVideoAd = mCacheInters->poll();
    if(_ttFullScreenVideoAd != nullptr){
        TTFullScreenVideoAd ttFullScreenVideoAd(_ttFullScreenVideoAd);

        //todo:listener?

        ttFullScreenVideoAd.showFullScreenVideoAd(activity);

        logd("inters show");
    }else{
        loge("inters show fail");

        callback.onShowFail(name());
        loadNextInters();
    }
}

void Pangle::loadNextInters() {
    if(Record::getInstance().hasPangle()){
        logd("load next inters");

        loadInters();
    }
}

jboolean Pangle::hasInters() {
    return !mCacheInters->isEmpty();
}
