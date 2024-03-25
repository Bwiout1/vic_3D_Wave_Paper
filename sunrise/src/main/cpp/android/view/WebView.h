#ifndef AND_WEBVIEW_H
#define AND_WEBVIEW_H


#include "../../android/Context.h"
#include "ViewGroup.h"

class WebView : public ViewGroup {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    static jobject constructor(const jobject& context);
    static void setBackgroundColor(const jobject& core, jint color);
    static jobject getSettings(const jobject& core);

public:
    WebView(const jobject& core);

    WebView(Context& context);
    WebView(const WebView&)= default;
    WebView& operator=(const WebView&)=delete;

    void setBackgroundColor(jint color);
    jobject getSettings();
    void loadUrl(jstring url);
};


#endif //AND_WEBVIEW_H
