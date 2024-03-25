#ifndef AND_WEBSETTINGS_H
#define AND_WEBSETTINGS_H


#include "../AndroidClassWrapperBase.h"

class WebSettings : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject LAYOUT_ALGORITHM_NORMAL ;

    static const jint LOAD_DEFAULT = -1 ;
    static const jint LOAD_NORMAL = 0;
    static const jint LOAD_CACHE_ELSE_NETWORK = 1;
    static const jint LOAD_NO_CACHE = 2;
    static const jint LOAD_CACHE_ONLY = 3;


public:
    WebSettings(const jobject& core);

    WebSettings(const WebSettings&)= default;
    WebSettings& operator=(const WebSettings&)=delete;

    void setJavaScriptEnabled(jboolean flag);
    void setUseWideViewPort (jboolean use);
    void setLoadWithOverviewMode (jboolean overview);
    void setSupportZoom(jboolean support);
    void setBuiltInZoomControls(jboolean enabled);
    void setMediaPlaybackRequiresUserGesture(jboolean require);
    void setDisplayZoomControls(jboolean enabled);
    void setDomStorageEnabled(jboolean flag);
    void setGeolocationEnabled(jboolean flag);
    void setJavaScriptCanOpenWindowsAutomatically(jboolean flag);
    void setNeedInitialFocus(jboolean flag);
    void setSaveFormData(jboolean save);
    void setSupportMultipleWindows(jboolean support);
    void setLayoutAlgorithm(const jobject l);
    void setAppCachePath(jstring path);
    void setAppCacheEnabled(jboolean enabled);
    void setDatabaseEnabled(jboolean enabled);
    void setCacheMode(jint mode);

private:
    WebSettings(){}
};


#endif //AND_WEBSETTINGS_H
