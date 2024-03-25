#include "StartStrategyAlarmImpl.h"
#include "../../android/Application.h"
#include "../../android/AlarmManager.h"
#include "../../android/PendingIntent.h"
#include "../../android/AndroidJni.h"
#include "../../util/System.h"

long StartStrategyAlarmImpl::delayTimeMills(){
    return 3000;
}

const char* StartStrategyAlarmImpl::getName(){
    return "alarm";
}

bool StartStrategyAlarmImpl::start(jobject context, Intent& intent){

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && intent.getCore() != nullptr){
        LOGV("StartStrategyAlarmImpl::start");

        Application& application = Application::getInstance();

        AlarmManager alarmWrapper(application.getSystemService("alarm")) ;
        PendingIntent activityIntent(PendingIntent::getActivity(application.getCore(),
                                                                10102,
                                                                intent.getCore(),
                                                                PendingIntent::FLAG_UPDATE_CURRENT));


        if(AndroidJni::get_dev_api_level() >= 19){
            alarmWrapper.setExact(0, System::currentTimeMillis()+1000L, activityIntent.getCore());
        }else{
            alarmWrapper.set(0, System::currentTimeMillis()+1000L, activityIntent.getCore());
        }
    }

    return true;
}

StartStrategyAlarmImpl::~StartStrategyAlarmImpl() {
    LOGV("~StartStrategyAlarmImpl");
}
