#ifndef AND_ALARMMANAGER_H
#define AND_ALARMMANAGER_H


#include "AndroidClassWrapperBase.h"

class AlarmManager : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    AlarmManager(const jobject & core);

    AlarmManager(const AlarmManager&) = delete;
    AlarmManager& operator=(const AlarmManager&)=delete;

    void set(jint type, jlong triggerAtMillis, const jobject & operation);
    void setExact(jint type, jlong triggerAtMillis, const jobject & operation);
private:
    AlarmManager(){}
};


#endif //AND_ALARMMANAGER_H
