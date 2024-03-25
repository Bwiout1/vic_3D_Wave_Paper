#ifndef GP_INSTALLREFERRERCLIENT_H
#define GP_INSTALLREFERRERCLIENT_H

#include "../../android/AndroidClassWrapperBase.h"


class InstallReferrerClient : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    static jobject newBuilder(const jobject& context);

public:
    InstallReferrerClient(const jobject& core) ;

    void startConnection(const jobject& listener);
    jstring installReferrer();
    void endConnection();

    class Builder;
    class InstallReferrerResponse;
};

class InstallReferrerClient::Builder : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    Builder(const jobject& core);

    jobject build();

private:
    Builder(){}
    Builder(const Builder&)=delete;
    Builder& operator=(const Builder&)=delete;
};

class InstallReferrerClient::InstallReferrerResponse {
public:
    const static jint SERVICE_DISCONNECTED = -1;
    const static jint OK = 0;
    const static jint SERVICE_UNAVAILABLE = 1;
    const static jint FEATURE_NOT_SUPPORTED = 2;
    const static jint DEVELOPER_ERROR = 3;
    const static jint PERMISSION_ERROR = 4;
};

#endif //GP_INSTALLREFERRERCLIENT_H
