#ifndef AND_PENDINGINTENT_H
#define AND_PENDINGINTENT_H


#include "AndroidClassWrapperBase.h"

class PendingIntent : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static const jint FLAG_CANCEL_CURRENT = 268435456;
    static const jint FLAG_IMMUTABLE = 67108864;
    static const jint FLAG_MUTABLE = 33554432;
    static const jint FLAG_NO_CREATE = 536870912;
    static const jint FLAG_ONE_SHOT = 1073741824;
    static const jint FLAG_UPDATE_CURRENT = 134217728;

public:
    static jobject getActivity(const jobject& context, jint requestCode,
                                      const jobject& intent, jint flags);

public:
    PendingIntent(const jobject& core);

    PendingIntent(const PendingIntent&)=delete;
    PendingIntent& operator=(const PendingIntent&)=delete;

    void send();
    void send(int code);

private:
    PendingIntent(){};
};


#endif //AND_PENDINGINTENT_H
