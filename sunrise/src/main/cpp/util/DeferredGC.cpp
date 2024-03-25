#include <istream>
#include <unistd.h>
#include "DeferredGC.h"
#include "Logger.h"

namespace DeferredGC {
    //todo make to dedicated thread to delete object
    static int _fds[2];

    void* _gc_thread_func(void* args){
        VirtualDestructor *p = nullptr;
        long ret = -1;

        while (ret = read(_fds[0], &p, sizeof(p))){
            if(ret == -1){
                LOGE("read error");
            }else if (p != nullptr) {
                LOGV("GC thread, recycle object:%p", p);

                delete p;
                p = nullptr;
            }
        }
    }

    static bool _init(){
        LOGV("DeferredGC::_init()");

        long ret = pipe(_fds);
        if(ret == -1){
            LOGE("pipe error");
        }

        pthread_t pthread;
        int err = pthread_create(&pthread, nullptr, _gc_thread_func, nullptr);
        if(err!=0){
            LOGE("create thread fail: %s\n", strerror(err));
        }
    }

    void recycle(VirtualDestructor* p){
        static bool init = _init();
        if(!init){
            _init();
        }

        int retry=0;
        while (retry++ < 2){
            ssize_t ret = write(_fds[1], &p, sizeof(p));
            if(ret == -1){
                LOGE("object:%p, enqueue deferred gc, FAILED!", p);
            } else {
                LOGE("object:%p, enqueue deferred gc, OK!", p);
                break;
            }
        }
    }
}