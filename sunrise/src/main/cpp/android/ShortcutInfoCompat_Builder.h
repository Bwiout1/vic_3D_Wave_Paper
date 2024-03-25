#ifndef AND_SHORTCUTINFOCOMPAT_BUILDER_H
#define AND_SHORTCUTINFOCOMPAT_BUILDER_H


#include "AndroidClassWrapperBase.h"

class ShortcutInfoCompat_Builder : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    ShortcutInfoCompat_Builder(const jobject& context, jstring id);
    ShortcutInfoCompat_Builder(const ShortcutInfoCompat_Builder&)= default;
    ShortcutInfoCompat_Builder& operator=(const ShortcutInfoCompat_Builder&)=delete;

    ShortcutInfoCompat_Builder& setIcon(const jobject& icon);
    ShortcutInfoCompat_Builder& setShortLabel(jstring shortLabel);
    ShortcutInfoCompat_Builder& setLongLabel(jstring longLabel);
    ShortcutInfoCompat_Builder& setIntent(const jobject& intent);
    ShortcutInfoCompat_Builder& setActivity(const jobject& activity);

    jobject build();
};


#endif //AND_SHORTCUTINFOCOMPAT_BUILDER_H
