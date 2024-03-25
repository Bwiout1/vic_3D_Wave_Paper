#ifndef AND_WORKMANAGER_H
#define AND_WORKMANAGER_H


#include "../AndroidClassWrapperBase.h"

class WorkManager : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static WorkManager* inst;
    static void init();

public:
    static WorkManager& getInstance(const jobject context);

public:
    WorkManager(const jobject& core);

    jobject enqueueUniqueWork(const jstring uniqueWorkName, const jobject existingWorkPolicy, const jobject work);

    jobject enqueueUniquePeriodicWork(const jstring uniqueWorkName, const jobject existingPeriodicWorkPolicy, const jobject periodicWork);

private:
    WorkManager(){};

    WorkManager(const WorkManager&) = delete;
    WorkManager& operator=(const WorkManager&)=delete;
};


#endif //AND_WORKMANAGER_H
