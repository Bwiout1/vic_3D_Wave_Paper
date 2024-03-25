#include "WorkManager.h"


bool WorkManager::inited = false;
WorkManager *WorkManager::inst = nullptr;
jclass WorkManager::clsZ = nullptr;

void WorkManager::init() {
    if (!inited) {
        LOGV("WorkManager::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/work/WorkManager");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}

WorkManager &WorkManager::getInstance(const jobject context) {
    if (!inited) {
        init();
    }

    if (inst == nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr && context != nullptr) {
            jmethodID method = jenv->GetStaticMethodID(clsZ, "getInstance",
                                                       "(Landroid/content/Context;)Landroidx/work/WorkManager;");
            inst = new WorkManager(jenv->CallStaticObjectMethod(clsZ, method, context));
        }
    }

    return *inst;
}

WorkManager::WorkManager(const jobject &core) {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && core != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject
WorkManager::enqueueUniqueWork(const jstring uniqueWorkName, const jobject existingWorkPolicy,
                               const jobject work) {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore != nullptr
        && uniqueWorkName != nullptr && existingWorkPolicy != nullptr && work != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "enqueueUniqueWork",
                                             "(Ljava/lang/String;Landroidx/work/ExistingWorkPolicy;Landroidx/work/OneTimeWorkRequest;)Landroidx/work/Operation;");
        ret = jenv->CallObjectMethod(mCore, method, uniqueWorkName, existingWorkPolicy, work);
    }

    return ret;
}

jobject WorkManager::enqueueUniquePeriodicWork(const jstring uniqueWorkName,
                                               const jobject existingPeriodicWorkPolicy,
                                               const jobject periodicWork) {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "enqueueUniquePeriodicWork",
                                             "(Ljava/lang/String;Landroidx/work/ExistingPeriodicWorkPolicy;Landroidx/work/PeriodicWorkRequest;)Landroidx/work/Operation;");
        ret = jenv->CallObjectMethod(mCore, method, uniqueWorkName, existingPeriodicWorkPolicy,
                                     periodicWork);
    }

    return ret;
}