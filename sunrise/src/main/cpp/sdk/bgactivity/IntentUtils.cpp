#include <ctime>
#include <cstdlib>
#include "IntentUtils.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../android/Context.h"
#include "../../android/AndroidJni.h"
#include "../../android/PendingIntent.h"
#include "../../util/LocalFrame.h"
#include "../../android/ActivityManager.h"
#include "../../util/List.h"
#include "../../util/Iterator.h"
#include "../../android/ComponentName.h"
#include "../../util/String.h"

jboolean IntentUtils::lb(JNIEnv *env, jclass clazz, jobject context, jobject intent, jboolean isPending) {
    return b(context, intent, isPending);
}

jboolean IntentUtils::b(jobject const &context_, jobject const &intent_, jboolean isPending) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv== nullptr)
        return false;

    if(context_ != nullptr && intent_ != nullptr){
        Context context(context_);
        Intent intent(intent_);

        if(AndroidJni::IsInstanceOf(context_, Context::getActivityClass())){
            context.startActivity(intent_);

            LOGV("IntentUtils::b");
            return true;
        }

        intent.addFlags(Intent::FLAG_ACTIVITY_NEW_TASK);
        if(isPending){
            srand(time(nullptr));
            PendingIntent pdIntent(
                    PendingIntent::getActivity(context_,
                                               (jint)(rand() % 1000000 + 125),
                                               intent.getCore(),
                                               PendingIntent::FLAG_ONE_SHOT)
                    );
            pdIntent.send();

            LOGV("IntentUtils::b");
            return true;
        }

        context.startActivity(intent.getCore());
        LOGV("IntentUtils::b");
        return true;
    }

    return false;
}

void IntentUtils::moveAppToFront(jobject const &context_) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    LocalFrame localFrame(jenv);
    if(jenv != nullptr && context_ != nullptr){
        Context context(context_);
        jobject am = context.getSystemService("activity");
        if(am == nullptr)
            return;

        ActivityManager activityManager(am);
        jobject runningTasks = activityManager.getRunningTasks(100);
        if(runningTasks == nullptr)
            return;

        Iterator it(List(runningTasks).iterator());
        while (it.hasNext()){
            ActivityManager::RunningTaskInfo taskInfo(it.next()) ;
            if(taskInfo.topActivity != nullptr){
                ComponentName topActivityComponent(taskInfo.topActivity);
                if(String::equals(topActivityComponent.getPackageName(), context.getPackageName())){

                    activityManager.moveTaskToFront(taskInfo.id, 0);

                    LOGV("IntentUtils::moveAppToFront");
                    return;
                }
            }
        }

    }
}

jboolean IntentUtils::isAppInForeground(jobject const &context_) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    LocalFrame localFrame(jenv);
    if(jenv != nullptr && context_ != nullptr){
        Context context(context_);
        jobject am = context.getSystemService("activity");
        if(am == nullptr)
            return false;

        ActivityManager activityManager(am);

        jobject runningAppProcesses = activityManager.getRunningAppProcesses();
        if(runningAppProcesses == nullptr)
            return false;

        Iterator it(List(runningAppProcesses).iterator());
        while(it.hasNext()){
            ActivityManager::RunningAppProcessInfo processInfo(it.next());
            if(String::equals(context.getProcessName(),processInfo.processName)
                && processInfo.importance == ActivityManager::RunningAppProcessInfo::IMPORTANCE_FOREGROUND){

                LOGV("IntentUtils::isAppInForeground");
                return true;
            }
        }
    }

    return false;
}
