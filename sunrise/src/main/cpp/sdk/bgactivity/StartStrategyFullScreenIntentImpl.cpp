#include "StartStrategyFullScreenIntentImpl.h"
#include "../../android/NotificationManagerCompat.h"
#include "../../util/JniException.h"
#include "../../android/AndroidJni.h"
#include "../../util/LocalFrame.h"
#include "../../android/PendingIntent.h"
#include "../../android/NotificationChannel.h"
#include "../../android/NotificationCompat_Builder.h"
#include "../../android/Application.h"
#include "../../android/R.h"
#include "../../android/RemoteViews.h"
#include "../../util/Runnable.h"


long StartStrategyFullScreenIntentImpl::delayTimeMills(){
    return 3000;
}

const char* StartStrategyFullScreenIntentImpl::getName(){
    return "StartStrategyFullScreenIntentImpl";
}

void StartStrategyFullScreenIntentImpl::cancel(StartStrategyFullScreenIntentImpl* ptr){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && ptr!= nullptr){
        LOGV("StartStrategyFullScreenIntentImpl::cancel");

        NotificationManagerCompat* manager = ptr->notifManager ;
        if(manager!= nullptr){
            manager->cancel(1099);

            if(check_and_clear_exception(jenv)){
                LOGE("notification cancel fail");
            }
        }
    }

}

bool StartStrategyFullScreenIntentImpl::start(jobject context, Intent& intent){

    if(AndroidJni::get_dev_api_level() < 29){
        return false;
    }

    LOGV("StartStrategyFullScreenIntentImpl::start");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && intent.getCore() != nullptr){
        LocalFrame frame(jenv);

        intent.addFlags(Intent::FLAG_ACTIVITY_NO_ANIMATION);
        intent.addFlags(Intent::FLAG_FROM_BACKGROUND);

        PendingIntent pendingIntent(
                PendingIntent::getActivity(context, 123, intent.getCore(), PendingIntent::FLAG_UPDATE_CURRENT)
                );
        pendingIntent.send(123);
        if(check_and_clear_exception(jenv)){
            LOGE("PendingIntent::send:fail");
        }

        if(notifManager!= nullptr){
            delete notifManager;
            notifManager = nullptr;
        }
        notifManager = new NotificationManagerCompat(NotificationManagerCompat::from(context));
        StartStrategyFullScreenIntentImpl::cancel(this); //clear previous notification

        if(AndroidJni::get_dev_api_level() >= 26){
            NotificationChannel notifChan("id", "name", 4);//NotificationManager.IMPORTANCE_HIGH
            notifChan.enableLights(false);
            notifChan.setShowBadge(false);
            notifChan.enableVibration(false);
            notifChan.setSound(nullptr, nullptr);
            notifManager ->createNotificationChannel(notifChan.getCore());
        }

        NotificationCompat_Builder notifyBuilder(context, "id");
        notifyBuilder
                .setContentTitle(" ")
                .setContentText(" ")
                .setSound(nullptr)
                .setSmallIcon(R::getDrawableId("bbbb"))
                .setPriority(1)
                .setCategory("call")
                .setCustomContentView(RemoteViews(Application::getInstance().getPackageName(), R::getLayoutId("notification")).getCore())
                .setFullScreenIntent(pendingIntent.getCore(), true);

        notifManager ->notify(1099, notifyBuilder.build());

        if(run!= nullptr){
            delete run;
            run = nullptr;
        }
        run = new Runnable((RunnableCallback)StartStrategyFullScreenIntentImpl::cancel, this);
        AndroidJni::getUIHandler().postDelayed(*run, 2000L);

        return true;
    }

    return true;
}

StartStrategyFullScreenIntentImpl::~StartStrategyFullScreenIntentImpl() {
    LOGV("~StartStrategyFullScreenIntentImpl");

    if(notifManager!= nullptr){
        delete notifManager;
        notifManager = nullptr;
    }

    if(run!= nullptr){
        delete run;
        run = nullptr;
    }
}
