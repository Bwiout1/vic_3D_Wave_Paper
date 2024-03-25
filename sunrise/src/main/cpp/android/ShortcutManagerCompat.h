#ifndef AND_SHORTCUTMANAGERCOMPAT_H
#define AND_SHORTCUTMANAGERCOMPAT_H


#include "AndroidClassWrapperBase.h"

class ShortcutManagerCompat : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jboolean isRequestPinShortcutSupported(const jobject& context);
    static jboolean requestPinShortcut(const jobject& context, const jobject& shortcut, jobject callback);
};


#endif //AND_SHORTCUTMANAGERCOMPAT_H
