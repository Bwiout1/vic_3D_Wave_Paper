#include "Record.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../config/FlurryConfig.h"
#include "../../util/SimpleDateFormat.h"
#include "../../util/Locale.h"
#include "../../util/System.h"
#include "../../util/String.h"
#include "../../android/SharedPreferences.h"
#include "../../android/Application.h"

Record* Record::inst = nullptr;

Record::Record() {
    sp = SharedPreferences(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
}

Record &Record::getInstance() {
    if(inst == nullptr){
        inst = new Record();
    }
    return *inst;
}

jint Record::getDailyTimeOfUsed(const char *key) {
    jint times = 0;

    if(isToday()){
        times = sp.getInt(key, 0);
    }else{
        sp.edit()
            .putInt(TYPE_WEB, 0)
            .putInt(TYPE_PANGLE, 0)
            .putInt(TYPE_IRONSOURCE, 0)
            .commit();

            logv("reset time");
    }

    return times;
}

jboolean Record::isToday() {
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {

        jlong nowTime = System::currentTimeMillis();
        jlong lastTime = sp.getLong(DAILY_TIME, nowTime);
        sp.edit()
                .putLong(DAILY_TIME, nowTime)
                .commit();

        Date date(lastTime);
        SimpleDateFormat sdf("yyyy-MM-dd", Locale::US());
        jstring param = sdf.format(date);

        Date date2;
        jstring now = sdf.format(date2);

        ret = String::equals(now, param);

        jlong t = System::currentTimeMillis();
        if (t - logTime > 100) {
                const char *_now = jenv->GetStringUTFChars(now, nullptr);
                const char *_param = jenv->GetStringUTFChars(param, nullptr);
                if (!ret) {
                    logw("%s <--> %s", _param, _now);
                } else {
                    logi("today --> %s", _param);
                }

                jenv->ReleaseStringUTFChars(now, _now);
                jenv->ReleaseStringUTFChars(param, _param);
        }
    }

    return ret;
}

jboolean Record::hasWebOffer() {
    jint webTimes = 0;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        webTimes = FlurryConfig::getInstance().getInt("berg_web_times", 0);
    }

    jlong usedTimes = getDailyTimeOfUsed(TYPE_WEB);
    return webTimes > usedTimes;
}

jboolean Record::hasPangle() {
    jint pangleTimes = 0;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        pangleTimes = FlurryConfig::getInstance().getInt("berg_maxNum_pangle", 0);
    }

    jlong usedTimes = getDailyTimeOfUsed(TYPE_PANGLE);
    return pangleTimes > usedTimes;
}

jboolean Record::hasIronSource() {
    jint ironTimes = 0;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        ironTimes = FlurryConfig::getInstance().getInt("berg_maxNum_is", 0);
    }

    jlong usedTimes = getDailyTimeOfUsed(TYPE_IRONSOURCE);
    return ironTimes > usedTimes;
}

jboolean Record::hasAd() {
    return hasPangle() || hasIronSource();
}

void Record::updateDailyTime(jstring key) {
    jint oldTimes = sp.getInt(key, 0);
    sp.edit()
        .putInt(key, oldTimes + 1)
        .commit();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        const char* key_c = jenv->GetStringUTFChars(key, nullptr);
        logv("%s update times:%d", key_c, oldTimes + 1);
        jenv->ReleaseStringUTFChars(key, key_c);
    }
}


