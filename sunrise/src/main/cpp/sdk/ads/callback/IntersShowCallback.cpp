#include "IntersShowCallback.h"
#include "../../../util/HashMap.h"
#include "../../control/Reporter.h"

IntersShowCallback::IntersShowCallback(jobject const &_next) {
    if(_next != nullptr){
        next = _next;
    }
}

void IntersShowCallback::onShow(jstring pre) {
    HashMap hashMap = HashMap();
    hashMap.put("network", pre);
    Reporter::report("in_ad_imp", hashMap);
}

void IntersShowCallback::onShowFail(jstring pre) {
    if(next != nullptr){
        //todo:next.invoke
    }
}

void IntersShowCallback::onDismiss(jstring pre) {
    if(next != nullptr){
        //todo:next.invoke
    }
}
