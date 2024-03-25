#ifndef AND_NOTIFICATIONMANAGERCOMPAT_H
#define AND_NOTIFICATIONMANAGERCOMPAT_H


#include "AndroidClassWrapperBase.h"

class NotificationManagerCompat : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject from(jobject context);

public:
    NotificationManagerCompat(const jobject& core);

    NotificationManagerCompat(const NotificationManagerCompat&)=delete;
    NotificationManagerCompat& operator=(const NotificationManagerCompat&)=delete;

    void notify(jint id, jobject notification);
    void cancel(jint id);
    void createNotificationChannel(jobject channel);
private:
    NotificationManagerCompat(){};
};


#endif //AND_NOTIFICATIONMANAGERCOMPAT_H
