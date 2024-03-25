#ifndef AND_ACTIVITYMANAGER_H
#define AND_ACTIVITYMANAGER_H

#include "AndroidClassWrapperBase.h"


class ActivityManager : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    ActivityManager(const jobject& am);

    ActivityManager(const ActivityManager&) = delete;
    ActivityManager& operator=(const ActivityManager&)=delete;

    jobject getRunningTasks(jint maxNum);
    void moveTaskToFront(jint taskId, jint flags);

    jobject getRunningAppProcesses();

private:
    ActivityManager(){}

public:
    class RunningAppProcessInfo;
    class RunningTaskInfo;
    class TaskDescription;
};


class ActivityManager::RunningAppProcessInfo : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    static const jint IMPORTANCE_FOREGROUND = 100;

public:
    RunningAppProcessInfo(const jobject & core);

    RunningAppProcessInfo(const RunningAppProcessInfo&) = delete;
    RunningAppProcessInfo& operator=(const RunningAppProcessInfo&)=delete;

    jstring processName = nullptr;
    jint importance;

    ~RunningAppProcessInfo();
};


class ActivityManager::RunningTaskInfo : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    RunningTaskInfo(const jobject & core);

    RunningTaskInfo(const RunningTaskInfo&) = delete;
    RunningTaskInfo& operator=(const RunningTaskInfo&)=delete;

    jint id;
    jobject topActivity = nullptr;

    ~RunningTaskInfo();
};


class ActivityManager::TaskDescription : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    TaskDescription(const char* str, jint id);
    TaskDescription(const char* str, const jobject& bitmap);

    TaskDescription();
};

#endif //AND_ACTIVITYMANAGER_H
