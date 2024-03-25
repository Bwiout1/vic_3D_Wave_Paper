#ifndef POPUP_STARTSTRATEGYLIST_H
#define POPUP_STARTSTRATEGYLIST_H

#include <vector>
#include <jni.h>
#include "IStartStrategy.h"
#include "../../util/Runnable.h"

class StartStrategyList : public AndroidClassWrapperBase {
public:
//    static jboolean JNICALL  startActivity(JNIEnv *env, jobject thisZ, jobject context, jobject intent);
    static void JNICALL notify(JNIEnv *env, jclass clazz, jobject intent);

    static jboolean startActivity2(jobject context, jobject intent);

private:
    static bool inited;
    static const char* const st_list;
    static void init();

public:
    StartStrategyList(jobject core);
    StartStrategyList(const StartStrategyList&)= default;
    StartStrategyList& operator=(const StartStrategyList&)=delete;

    void startPrev(jobject context, jobject intent);
    static void startIterate(StartStrategyList* strategyList);

    ~StartStrategyList();
private:
    StartStrategyList(){}

    jobject mContext= nullptr;
    Intent* mIntent= nullptr;
    std::vector<IStartStrategy*> strategyList;
    std::vector<IStartStrategy*>::iterator step;
    jlong magic;
    bool success= false;

    Runnable* nextRun;
};


#endif //POPUP_STARTSTRATEGYLIST_H
