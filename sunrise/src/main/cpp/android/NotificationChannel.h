#ifndef AND_NOTIFICATIONCHANNEL_H
#define AND_NOTIFICATIONCHANNEL_H


#include "AndroidClassWrapperBase.h"

class NotificationChannel : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    NotificationChannel(const jobject& core);

    NotificationChannel(const char* id, const char* name, jint importance);
    NotificationChannel(const NotificationChannel&)=delete;
    NotificationChannel& operator=(const NotificationChannel&)=delete;

    void enableLights(jboolean lights);
    void setShowBadge(jboolean showBadge);
    void enableVibration(jboolean vibration);
    void setSound(jobject sound, jobject audioAttributes);
};


#endif //AND_NOTIFICATIONCHANNEL_H
