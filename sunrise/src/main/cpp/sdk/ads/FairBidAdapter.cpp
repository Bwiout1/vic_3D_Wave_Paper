#include "FairBidAdapter.h"
#include "fairbid/FairBid.h"
#include "fairbid/UserInfo.h"
#include "../../android/Application.h"
#include "fairbid/MediationStartedListener.h"

namespace FairBidAdapter {
    MediationStartedListener *sdk_listener = nullptr;

    void init(const jobject &activity) {
        Application &appInst = Application::getInstance();

        UserInfo::setGdprConsent(true, appInst.getCore());
        UserInfo::setLgpdConsent(true, appInst.getCore());

        FairBid fairbid(FairBid::configureForAppId("app_id_passed_by_macro"));
        fairbid.setUserAChild(false);
#ifndef NDEBUG
        fairbid.enableLogs();
#endif

        sdk_listener = new MediationStartedListener(//utilize lambda function
                [](jobject mediatedNetwork) {//因FairBid不支持native，为支持Mintegral的native广告，须监听该平台是否初始化，然后才能请求/渲染native广告，

                },
                [](jobject mediatedNetwork, jstring errMsg) {

                });
        fairbid.withMediationStartedListener(sdk_listener->getCore());
        fairbid.start(activity);
    }
}