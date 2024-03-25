#ifndef AND_DISPLAYMANAGER_H
#define AND_DISPLAYMANAGER_H


#include "AndroidClassWrapperBase.h"

class DisplayManager : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static const jint MATCH_CONTENT_FRAMERATE_ALWAYS = 2;
    static const jint MATCH_CONTENT_FRAMERATE_NEVER = 0;
    static const jint MATCH_CONTENT_FRAMERATE_SEAMLESSS_ONLY = 1;
    static const jint MATCH_CONTENT_FRAMERATE_UNKNOWN = -1;
    static const jint VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR = 16;
    static const jint VIRTUAL_DISPLAY_FLAG_OWN_CONTENT_ONLY = 8;
    static const jint VIRTUAL_DISPLAY_FLAG_PRESENTATION = 2;
    static const jint VIRTUAL_DISPLAY_FLAG_PUBLIC = 1;
    static const jint VIRTUAL_DISPLAY_FLAG_SECURE = 4;

public:
    DisplayManager(const jobject &core);

    DisplayManager(const DisplayManager &) = default;

    DisplayManager &operator=(const DisplayManager &) = delete;

    jobject createVirtualDisplay(const char *name, jint width, jint height, jint densityDpi,
                                 jobject surface, jint flags);

private:
    DisplayManager() {}
};


#endif //AND_DISPLAYMANAGER_H
