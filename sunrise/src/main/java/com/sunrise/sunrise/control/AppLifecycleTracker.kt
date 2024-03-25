package com.sunrise.sunrise.control

import android.app.Activity
import android.app.ActivityManager
import android.app.Application
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.drawable.ColorDrawable
import android.os.Build
import android.os.Bundle
import android.view.ViewGroup
import com.sunrise.sunrise.PPPA
import com.sunrise.sunrise.SRAPP
import com.sunrise.sunrise.ads.AdsMaster
import com.sunrise.sunrise.ads.MtgAdapter
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class AppLifecycleTracker private constructor() {
    companion object {
        private val TAG = AppLifecycleTracker::class.java.simpleName

        val instance: AppLifecycleTracker by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            AppLifecycleTracker()
        }

        private const val ACT_TYPE_NORMAL = 0
        private const val ACT_TYPE_POPUP = 1
        private const val ACT_TYPE_ADN = 2
    }

    private var activityNum = 0
    fun isAppForeground(): Boolean {
        return activityNum > 0
    }

    fun start() {
        SRAPP.app.registerActivityLifecycleCallbacks(object :
            Application.ActivityLifecycleCallbacks {
            override fun onActivityCreated(activity: Activity, bundle: Bundle?) {
                CoroutineScope(Dispatchers.Main).launch {
                    AdsMaster.instance.initAds(activity)

                    val kind = classifyActivity(activity)

                    if (kind != ACT_TYPE_ADN) {
                        AdsMaster.instance.loadInters()
                    }

                    AdsMaster.instance.loadNative(false)

                    if (kind == ACT_TYPE_ADN || kind == ACT_TYPE_POPUP) {
                        shadow(activity)
                    }
                }
            }

            override fun onActivityStarted(activity: Activity) {
                if (activityNum < 0) activityNum = 1 else activityNum++

                if (classifyActivity(activity) == ACT_TYPE_NORMAL) {
                    CoroutineScope(Dispatchers.Main).launch {
                        catchAdView(activity)?.let {
                            AdsMaster.instance.addNative(activity, it)
                        }
                    }
                }
            }

            override fun onActivityResumed(activity: Activity) {
                MtgAdapter.instance.nativeOnResume(activity)
            }

            override fun onActivityPaused(activity: Activity) {
                MtgAdapter.instance.nativeOnPause(activity)
            }

            override fun onActivityStopped(activity: Activity) {
                if (activityNum < 1) activityNum = 0 else activityNum--
            }

            override fun onActivitySaveInstanceState(activity: Activity, bundle: Bundle) {}
            override fun onActivityDestroyed(activity: Activity) {
                if (classifyActivity(activity) != ACT_TYPE_ADN) {
                    catchAdView(activity)?.let {
                        AdsMaster.instance.clearNative(activity, it)
                    }
                }
            }
        })
    }

    //取出Ad相关页面和外展页
    private fun classifyActivity(activity: Activity): Int {
        var ret = ACT_TYPE_NORMAL

        if (activity is PPPA) {
            ret = ACT_TYPE_POPUP
            return ret
        }

        val fullName = activity.javaClass.name
        if (fullName.startsWith("com.bytedance")
            || fullName.startsWith("com.fyber")
            || fullName.startsWith("com.tapjoy")
            || fullName.startsWith("com.mbridge")
            || fullName.startsWith("com.unity3d")
            || fullName.startsWith("com.ironsource")
        ) {
            ret = ACT_TYPE_ADN
        }

        return ret
    }

    //找第一个带tag的view
    private fun catchAdView(activity: Activity): ViewGroup? {
        var adView = activity.window.decorView.findViewWithTag<ViewGroup>("big")
        adView?.let {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().v(TAG, "$activity catch native:big")
            }
            return it
        }
        adView = activity.window.decorView.findViewWithTag<ViewGroup>("small")
        adView?.let {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().v(TAG, "$activity catch native:small")
            }
            return it
        }
        return null
    }

    //ad page to hide icon & title in RecentList
    private fun shadow(activity: Activity) {
        try {
            val description =
                when {
                    Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP -> {
                        val bitmap = Bitmap.createBitmap(66, 66, Bitmap.Config.ARGB_8888)
                        val canvas = Canvas(bitmap)
                        ColorDrawable(0x00000000).apply {
                            setBounds(0, 0, 66, 66)
                            draw(canvas)
                        }

                        ActivityManager.TaskDescription(" ", bitmap)
                    }
                    else -> {
                        null
                    }
                }
            if (description != null) {
                activity.setTaskDescription(description)
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "activity shadow")
                }
            }
        } catch (e: Exception) {
            if (Switch.DEB_ENABLE)
                e.printStackTrace()
        }
    }
}