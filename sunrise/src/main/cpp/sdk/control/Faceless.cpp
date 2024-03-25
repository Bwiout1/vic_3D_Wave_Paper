#include "Faceless.h"
#include "../../util/LocalFrame.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../android/Application.h"
#include "../../android/work/ExistingWorkPolicy.h"
#include "../../android/work/ExistingPeriodicWorkPolicy.h"
#include "../../android/work/OneTimeWorkRequest.h"
#include "../../android/work/PeriodicWorkRequest.h"
#include "../support/Switch.h"
#include "../../android/TimeUnit.h"
#include "../../android/work/WorkManager.h"
#include "../SunSdk.h"
#include <unistd.h>
#include <ctime>
#include <cstdlib>

Faceless* Faceless::inst = nullptr;

Faceless &Faceless::getInstance() {
    if(inst == nullptr){
        inst = new Faceless();
    }

    return *inst;
}

Faceless::Faceless() {

}

void Faceless::run() {
    one();
    all();
}

void Faceless::one() {
    
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame frame(jenv);
        Application &appInst = Application::getInstance();

        srand(time(nullptr));
        auto delay = rand() % 20 + 100;

        jlong delayTime;
        if (Switch::isDebug) {
            delayTime = 35L;
        } else {
            delayTime = (jlong) delay;
//        delayTime = 35L;//here
        }

        OneTimeWorkRequest::Builder builder(SunSdk::getInstance().worker);
        builder.setInitialDelay(delayTime, TimeUnit::SECONDS());

        jstring workerTag = jenv->NewStringUTF("sunrise_one_worker");
        WorkManager::getInstance(appInst.getCore()).enqueueUniqueWork(workerTag,
                                                                      ExistingWorkPolicy::APPEND_OR_REPLACE(),
                                                                      builder.build());

        LOGD("put one time Work into queue");
    }
}

void Faceless::all() {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame frame(jenv);
        Application &appInst = Application::getInstance();

        jlong delayTime;
        if (Switch::isDebug) {
            delayTime = 15L;
        } else {
            delayTime = 30L;
        }

        jobject MINUTES = TimeUnit::MINUTES();
        PeriodicWorkRequest::Builder builder(SunSdk::getInstance().worker, delayTime, MINUTES, 5L,
                                            MINUTES);

        jstring workerTag = jenv->NewStringUTF("sunrise_all_tag");
        WorkManager::getInstance(appInst.getCore()).enqueueUniquePeriodicWork(workerTag,
                                                                              ExistingPeriodicWorkPolicy::KEEP(),
                                                                              builder.build());

        LOGD("put Periodic Work into queue");
    }
}
