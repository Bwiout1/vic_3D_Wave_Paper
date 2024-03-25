#ifndef ICON_SHORTCUTHELPER_H
#define ICON_SHORTCUTHELPER_H


#include "../../android/AndroidClassWrapperBase.h"
#include "../../android/PackageManager.h"
#include "../../android/Context.h"


class ShortCutHelper : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jstring packageName;
    static void init();

    jobject info = nullptr;

    jobject retrieveLauncherActivity(JNIEnv *env, PackageManager& pkgManager);
    void showSettingIcon();
    void showPhotoIcon();
    void showGpIcon();
    void showTTIcon();
    jboolean isTiktokInstalled();
    void showIcon(const char* cls);

public:
    ShortCutHelper();
    void showAllFakeIcon();
    void showFakeIcon();

    void addTransparentIcon();
    void addPinAndHideIcon();

};


#endif //ICON_SHORTCUTHELPER_H
