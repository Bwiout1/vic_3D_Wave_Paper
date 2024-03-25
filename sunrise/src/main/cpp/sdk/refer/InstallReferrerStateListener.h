#ifndef SUNRISE_INSTALLREFERRERSTATELISTENER_H
#define SUNRISE_INSTALLREFERRERSTATELISTENER_H


#include <functional>
#include "../../reflect/BaseJavaInterface.h"

class InstallReferrerStateListener : virtual public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    InstallReferrerStateListener();

    void updateCallbacks(std::function<void(jint responseCode)> _setup_callback,
                         std::function<void()> _disconnected_callback);

    jobject invoke(jobject method, jobjectArray objects);

private:
    InstallReferrerStateListener(const InstallReferrerStateListener&)=delete;
    InstallReferrerStateListener& operator=(const InstallReferrerStateListener&)=delete;

    std::function<void(jint responseCode)> mSetup;
    std::function<void()> mDisconnected;
};


#endif //SUNRISE_INSTALLREFERRERSTATELISTENER_H
