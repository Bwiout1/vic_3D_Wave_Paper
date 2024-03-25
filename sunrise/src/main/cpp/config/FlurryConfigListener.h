#ifndef CONFIG_FLURRYCONFIGLISTENER_H
#define CONFIG_FLURRYCONFIGLISTENER_H

#include <functional>
#include "../reflect/BaseJavaInterface.h"

class FlurryConfigListener : public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

    std::function<void()> fetchSuccess;
    std::function<void(jboolean flag)> activateComplete;

public:
    FlurryConfigListener();
    jobject invoke(jobject method, jobjectArray objects);

    void updateCallbacks(
            std::function<void()> _fetchSuccess,
            std::function<void(jboolean flag)> _activateComplete);

    /**
     * this class just provide basic functions: logging/activate config. all other logics related to
     * sdk: init ads..., should be in sdk layer.
     */
    void onFetchSuccess();
    void onFetchNoChange();
    void onFetchError(jboolean isRetrying);
    void onActivateComplete(jboolean isCache);

private:
    FlurryConfigListener(const FlurryConfigListener&)=delete;
    FlurryConfigListener& operator=(const FlurryConfigListener&)=delete;
};


#endif //CONFIG_FLURRYCONFIGLISTENER_H
