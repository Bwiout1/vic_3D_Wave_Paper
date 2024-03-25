#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include "Guard.h"
#include "DebuggerUtil.h"
#include "ApkSignatureUtil.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/LocalFrame.h"
#include "../util/Logger.h"

namespace Guard{

    /**
     * 把该函数放入可预测的循环周期内运行，每次运行之间的间隔应该固定。
     * @param limitSec
     * @return 0:运行周期符合预期， -1:运行间隔多大
     */
    int _check_cycle_interval(unsigned int limitSec){
        int ret = 0;

#ifdef NDEBUG
        static unsigned long lastUnixTs = 0;

        timeval tv;
        gettimeofday(&tv, NULL);
        if(lastUnixTs==0){
            lastUnixTs = tv.tv_sec;
        } else {
            long now = tv.tv_sec;
            ret = (lastUnixTs+limitSec>now) ? 0 : -1;
            lastUnixTs = now;
        }
#endif
        return ret;
    }

    void* _guard_thread_func(void* args){
        JNIEnv* env = PthreadJNIEnvHelper::getJNIEnv();
        if(env!= nullptr){
            srand(time(0));
            do{
                LocalFrame frame(env);
                if(_check_cycle_interval(200)) {
                    LOGF("cycle interval is not met");
                    break;
                }

                if(DebuggerUtil::check_debugger(env)) {
                    LOGF("Debugger is detected");
                    break;
                }

                if(ApkSignatureUtil::check_apk_signature(env)) {
                    LOGF("Signature validation failed.");
                    break;
                }

                sleep(rand()%100+20);
            } while (1);
        }

#ifndef NDEBUG
        LOGW("pid=%d, kill self", getpid());
        sleep(1);
#endif
        kill(getpid(), SIGKILL);

        return 0;
    }


    void startGuardThread(){
        pthread_t pthread;

        int err = pthread_create(&pthread, nullptr, _guard_thread_func, nullptr);
        if(err!=0){
            LOGE("create thread fail: %s\n", strerror(err));
        }
    }

















//    int check_proc_self_status_tracepid(){
//        int tracepid = 0;
//#ifdef NDEBUG
//
//        FILE *fp = fopen("/proc/self/status", "r");
//    if(fp== nullptr)
//        return tracepid;
//
//    char linestr[1024];
//    while (!feof(fp)){
//        fgets(linestr, (sizeof linestr)/sizeof(linestr[0]), fp);
//        if(strncmp(linestr, "TracerPid:", 10) == 0){
//            tracepid = atoi(&linestr[10]);
//
//            LOGW("pid=%d, detected trace pid=%d", getpid(), tracepid);
//            break;
//        }
//    }
//
//    fclose(fp);
//#endif
//
//        return tracepid;
//    }
}