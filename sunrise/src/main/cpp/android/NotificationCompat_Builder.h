#ifndef AND_NOTIFICATIONCOMPAT_BUILDER_H
#define AND_NOTIFICATIONCOMPAT_BUILDER_H


#include "AndroidClassWrapperBase.h"

class NotificationCompat_Builder : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    NotificationCompat_Builder(const jobject& core);

    NotificationCompat_Builder(jobject context, const char* channelId);
    NotificationCompat_Builder(const NotificationCompat_Builder&)=delete;
    NotificationCompat_Builder& operator=(const NotificationCompat_Builder&)=delete;

    NotificationCompat_Builder& setContentTitle(const char* title);
    NotificationCompat_Builder& setContentText(const char* text);
    NotificationCompat_Builder& setSound(jobject sound) ;
    NotificationCompat_Builder& setSmallIcon(jint icon) ;
    NotificationCompat_Builder& setPriority(jint pri);
    NotificationCompat_Builder& setCategory(const char* category);
    NotificationCompat_Builder& setCustomContentView(jobject contentView);
    NotificationCompat_Builder& setFullScreenIntent(jobject intent, jboolean highPriority);

    jobject build();
};


#endif //AND_NOTIFICATIONCOMPAT_BUILDER_H
