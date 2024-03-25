#include <__threading_support>
#include <unistd.h>
#include "StartStrategyList.h"
#include "StartStrategyResetIntentImpl4MIUI.h"
#include "StartStrategyResetIntentImpl4VIVO.h"
#include "StartStrategyFullScreenIntentImpl.h"
#include "StartStrategyJobServiceImpl.h"
#include "StartStrategyAlarmImpl.h"
#include "StartStrategyNormal.h"
#include "StartStrategyBringForegroundImpl.h"
#include "StartStrategySafeImpl.h"
#include "StartStrategyVirtualDisplayImpl.h"
#include "../../android/AndroidJni.h"

//static int fds[2];
//
//void* _st_cleanup_thread_func(void* args){
//    StartStrategyList *p = nullptr;
//    long ret = -1;
//
//    while (ret = read(fds[0], &p, sizeof(p))){
//        if(ret == -1){
//            LOGE("read error");
//        }else if (p != nullptr) {
//            LOGV("cleanup_thread, delete StartStrategyList object:%p", p);
//
//            delete p;
//            p = nullptr;
//        }
//    }
//}

jboolean StartStrategyList::startActivity2(jobject context, jobject intent) {
    LOGV("startActivity2");
    StartStrategyList* list = new StartStrategyList(nullptr);
    list->startPrev(context, intent);
    return 0;//delete
}

//jboolean JNICALL
//StartStrategyList::startActivity(JNIEnv *env, jobject javaObj, jobject context, jobject intent) {
//    LOGV("startActivity");
//    StartStrategyList* list = new StartStrategyList(javaObj);
////    LOGE("list0.address is %p", list);
//    list->startPrev(context, intent);
//    return 0;//delete
//}


void JNICALL StartStrategyList::notify(JNIEnv *env, jclass clazz, jobject intent) {
    Intent intentWrapper(intent);
    jlong ptr = intentWrapper.getLongExtra(st_list, 0);
    if(ptr == 0)
        return;

    if(ptr != ((StartStrategyList*)ptr)->magic) {
        LOGE("It's not a StartStrategyList object, please check it. ptr is %ld, magic is %ld", ptr, ((StartStrategyList*)ptr)->magic);
        return;
    }

    ((StartStrategyList*)ptr)->success = true;
    LOGV("notify success, StartStrategyList:%p", ptr);
}

bool StartStrategyList::inited = false;
const char* const StartStrategyList::st_list = "ST_LIST_PTR";

void StartStrategyList::init() {
    LOGV("StartStrategyList::init");

    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;
        }

//        long ret = pipe(fds);
//        if(ret == -1){
//            LOGE("pipe error");
//        }
//
//        pthread_t pthread;
//        int err = pthread_create(&pthread, nullptr, _st_cleanup_thread_func, nullptr);
//        if(err!=0){
//            LOGE("create thread fail: %s\n", strerror(err));
//        }
    }
}

StartStrategyList::StartStrategyList(jobject core){
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr) {
        mCore = jenv ->NewGlobalRef(core);
    }
    magic = (jlong)this;

    nextRun = new Runnable((RunnableCallback)StartStrategyList::startIterate, this);
}

void StartStrategyList::startPrev(jobject context, jobject intent) {
    LOGV("StartStrategyList::startPrev");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv== nullptr)
        return;

    auto iter = strategyList.begin();
    while (iter != strategyList.end()){
        delete *iter;
        iter = strategyList.erase(iter);
    }
    if(mContext!= nullptr){
        jenv ->DeleteGlobalRef(mContext);
        mContext = nullptr;
    }
    if(mIntent!= nullptr){
        delete mIntent;
        mIntent = nullptr;
    }


    mContext = jenv->NewGlobalRef(context);
    mIntent = new Intent(intent);
    mIntent->putExtra(st_list, (jlong) this);//key(ST_LIST_PTR) = this;
//    LOGV("putExtra{key:ST_LIST_PTR, value:%ld}", (jlong)this);

    strategyList.push_back(new StartStrategyResetIntentImpl4MIUI());//
    strategyList.push_back(new StartStrategyResetIntentImpl4VIVO());//部分vivo需手动授予后台弹出界面权限
    strategyList.push_back(new StartStrategyVirtualDisplayImpl());//可跳转
    strategyList.push_back(new StartStrategyAlarmImpl());//可跳转,拉不到前台
    strategyList.push_back(new StartStrategySafeImpl());//可跳转,但可能拉不到前台
    strategyList.push_back(new StartStrategyFullScreenIntentImpl());//可跳转,但可能拉不到前台
    strategyList.push_back(new StartStrategyJobServiceImpl());//无法跳转
    strategyList.push_back(new StartStrategyNormal());
    strategyList.push_back(new StartStrategyBringForegroundImpl());//有时可跳转


    step = strategyList.begin();
    startIterate(this);
}

void StartStrategyList::startIterate(StartStrategyList *list) {
    LOGV("startIterate, StartStrategyList:%p", list);

    if(list->success || (list->step == list->strategyList.end())){

        LOGV("Iterate finish, StartStrategyList:%p", list);

        DeferredGC::recycle(list);

//        long ret = write(fds[1], &list, sizeof(list));
//        if(ret == -1){
//            LOGE("write error");
//        }

        return;
    }


    IStartStrategy* strategy = *(list->step);
    list->step++;
    if(strategy== nullptr)
        return;

    list->mIntent ->putExtra("action", strategy->getName());
    strategy ->start(list->mContext, *(list->mIntent));

    AndroidJni::getUIHandler().postDelayed(*(list->nextRun), strategy->delayTimeMills());
}



StartStrategyList::~StartStrategyList(){
    LOGV("StartStrategyList::~StartStrategyList, %p", this);

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    auto iter = strategyList.begin();
    while (iter != strategyList.end()){
        IStartStrategy* strategy= *iter;
        delete strategy;
        iter = strategyList.erase(iter);
    }
    if(mContext!= nullptr){
        jenv ->DeleteGlobalRef(mContext);
        mContext = nullptr;
    }
    if(mIntent!= nullptr){
        delete mIntent;
        mIntent = nullptr;
    }

    if(nextRun != nullptr){
        delete nextRun;
        nextRun = nullptr;
    }
}