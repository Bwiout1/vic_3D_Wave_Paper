#ifndef SUNRISE_PANGLE_H
#define SUNRISE_PANGLE_H

#include "AdUnit.h"
#include "../../util/ConcurrentLinkedQueue.h"
#include "openadsdk/TTAdNative.h"
#include <atomic>

class AdInteractionListener;

class Pangle : public AdUnit{
private:
    static Pangle* inst;

    static void* p_loadNative(void* args);
    void fillNative2(ViewGroup& container, AdInteractionListener* listener);
    void loadNextNative();
//    static void* p_loadNextNative(void* args);

    static void* p_loadInters(void* args);
public:
    Pangle();
    static Pangle& getInstance();
    void initNext();

    std::atomic<bool> aNative;
    std::atomic<bool> aInters;
    ConcurrentLinkedQueue* mCacheNative = nullptr;
    ConcurrentLinkedQueue* mCacheInters = nullptr;

    TTAdNative* ttAdNative = nullptr;

    jboolean isLoading();

    jstring name();
    void loadNative();
    void fillNative(ViewGroup& container);
    jboolean hasNative();
    void clearNative(ViewGroup& container);
    void fillCleanNative(ViewGroup& container);


    void loadInters();
    void showInters(const jobject& activity, IntersShowListener& callback);
    void loadNextInters();
    jboolean hasInters();
};


#endif //SUNRISE_PANGLE_H
