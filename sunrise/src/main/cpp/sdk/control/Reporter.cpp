#include "Reporter.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../util/String.h"
#include "../../analytics/FlurryAgent.h"
#include "../../analytics/YandexMetrica.h"
#include <pthread.h>

//bool Reporter::inited = false;
//jclass Reporter::clsZ = nullptr;

struct Box{
    const char* name;
    HashMap& hashMap;
};
//void Reporter::init() {
//    if(!inited){
//        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
//        if(jenv!= nullptr) {
//
//        }
//    }
//}

void Reporter::report(const char *name, HashMap &hashMap) {

    struct Box box = {name, hashMap};

    pthread_t tid;
    pthread_create(&tid, nullptr, p_report, &box);
}

void *Reporter::p_report(void* args) {
    Box* box;
    box = (Box*)args;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring name = jenv->NewStringUTF(box->name);
        if(String::length(name) != 0){
            if(box->hashMap.isNull){
                FlurryAgent::logEvent(name);
                YandexMetrica::reportEvent(name);
            }else{
                FlurryAgent::logEvent(name, box->hashMap);
                YandexMetrica::reportEvent(name, box->hashMap);
            }
        }

        jenv->DeleteLocalRef(name);
    }else{
        loge("report error");
    }

    if(box != nullptr){
        delete box;
        box = nullptr;
    }
    return nullptr;
}
