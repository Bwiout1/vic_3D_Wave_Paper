#include "OutersShowCallback.h"
#include "../../../util/HashMap.h"
#include "../../control/Reporter.h"
#include "../../control/ControlRoom.h"
#include "../../control/Record.h"
#include "../../../android/SharedPreferences.h"
#include "../../../android/Application.h"

OutersShowCallback::OutersShowCallback(std::function<void()> _next) {
    next = nullptr;
    next = std::move(_next);
}

void OutersShowCallback::onShow(jstring pre) {
    HashMap hashMap = HashMap();
    hashMap.put("network", pre);

    SharedPreferences sp(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
    const char* KEY_TIME = "init";
    jlong time = sp.getLong(KEY_TIME, 0);
    jlong initTime = ControlRoom::getInstance().getInitTime();
    if(time > 0 && time != initTime){
        hashMap.put("restart", "true");

        sp.edit()
            .putLong(KEY_TIME, initTime)
            .commit();
    }else{
        if(time == 0){
            hashMap.put("restart", "false");

            sp.edit()
                .putLong(KEY_TIME, initTime)
                .commit();
        }
    }
    Reporter::report("berg_ad_imp", hashMap);
    Record::getInstance().updateDailyTime(pre);
}

void OutersShowCallback::onShowFail(jstring pre) {
    if(next != nullptr){
        next();
    }
}

void OutersShowCallback::onDismiss(jstring pre) {
    if(next != nullptr){
        next();
    }
}
