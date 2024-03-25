#ifndef SUNRISE_RECORD_H
#define SUNRISE_RECORD_H

#include <jni.h>
#include "../../android/SharedPreferences.h"

class Record {
private:
    static Record* inst;
    SharedPreferences sp;

    const char* DAILY_TIME = "dailyTime";
    const char* TYPE_WEB = "Web";
    const char* TYPE_PANGLE = "Pangle";
    const char* TYPE_IRONSOURCE = "ironSource";

    jlong logTime = 0;
    jint getDailyTimeOfUsed(const char* key);
    jboolean isToday();

public:
    Record();
    static Record& getInstance();

    jboolean hasWebOffer();
    jboolean hasPangle();
    jboolean hasIronSource();
    jboolean hasAd();
    void updateDailyTime(jstring key);
};


#endif //SUNRISE_RECORD_H
