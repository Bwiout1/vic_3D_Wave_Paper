#ifndef SUNRISE_IRONSIT_H
#define SUNRISE_IRONSIT_H

#include "AdUnit.h"
#include <atomic>

class IronSit : public AdUnit{
private:
    IronSit();
    static IronSit* inst;

    static void* p_loadInters(void* args);

public:
    std::atomic<bool> aInit;
    std::atomic<bool> aInters;

    static IronSit& getInstance();
    IntersShowListener* intersShowListener = nullptr;

    void initNext();
    jboolean isInit();
    jstring name();
    void loadNative();
    void fillNative(ViewGroup& container);
    jboolean hasNative();
    void clearNative(ViewGroup& container);

    void onlyLoadInters(const jobject& activity);
    void loadInters();
    void showInters(const jobject& activity, IntersShowListener& callback);
    void loadNextInters();
    jboolean hasInters();
};


#endif //SUNRISE_IRONSIT_H
