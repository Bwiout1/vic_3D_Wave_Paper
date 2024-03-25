package com.sunrise.sunrise.control

import android.app.Application
import android.app.KeyguardManager
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.PowerManager
import android.text.TextUtils
import androidx.core.content.edit
import androidx.work.*
import com.sunrise.sunrise.SSDK
import com.sunrise.sunrise.SRA
import com.sunrise.sunrise.SRAPP
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Record
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.util.concurrent.TimeUnit
import kotlin.random.Random

class Work private constructor() {
    companion object {
        private val TAG = Work::class.java.simpleName

        const val TRIGGERTYPE: String = "trigger"
        const val TRIGGERTYPE_UNKWON: Int = 0
        const val TRIGGERTYPE_WEB: Int = 1
        const val TRIGGERTYPE_INTERS: Int = 2
        const val TRIGGERTYPE_BOTH: Int = 3

        val instance: Work by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            Work()
        }
    }


    fun run() {
        WorkManager.initialize(SRAPP.app, Configuration.Builder().build())
        scheduleOneshotAd(Random.nextLong(120, 200))
        initAdPeriodic()
        initConfigPeriodic()

        SRAPP.app.registerReceiver(object : BroadcastReceiver() {
            override fun onReceive(context: Context?, intent: Intent?) {
                doAdsWork()
            }
        }, IntentFilter(Intent.ACTION_SCREEN_ON))
    }

    private fun scheduleOneshotAd(delay: Long) {
        val one = OneTimeWorkRequestBuilder<AdWorker>()
            .setInitialDelay(if (Switch.DEB_ENABLE) 40L else delay, TimeUnit.SECONDS)
            .build()
        WorkManager.getInstance(SRAPP.app).enqueueUniqueWork(
            SRAPP.app.packageName + "-one",
            ExistingWorkPolicy.APPEND_OR_REPLACE, one
        )
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "put one time AdWork into queue")
        }
    }

    private fun initAdPeriodic() {
        val periodic = PeriodicWorkRequestBuilder<AdWorker>(
            if (Switch.DEB_ENABLE) 15 else 20, TimeUnit.MINUTES, 5, TimeUnit.MINUTES
        )
            .build()
        WorkManager.getInstance(SRAPP.app)
            .enqueueUniquePeriodicWork(
                SRAPP.app.packageName + "-all",
                ExistingPeriodicWorkPolicy.KEEP, periodic
            )
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "put Periodic AdWork into queue")
        }
    }

    private fun initConfigPeriodic() {
        val periodic = PeriodicWorkRequestBuilder<FlurryWorker>(
            if (Switch.DEB_ENABLE) 15 else 480, TimeUnit.MINUTES, 5, TimeUnit.MINUTES
        )
            .build()
        WorkManager.getInstance(SRAPP.app)
            .enqueueUniquePeriodicWork(
                SRAPP.app.packageName + "-config",
                ExistingPeriodicWorkPolicy.KEEP, periodic
            )
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "put Periodic FlurryWork into queue")
        }
    }

    fun doConfigWork() {
        SunConfig.instance.pull{
            val referrer = SSDK.instance.sp.getString(TrafficType.instance._referrer, "")
            if(!TextUtils.isEmpty(referrer)) {
                SSDK.instance.sp.edit {
                    putInt(
                        TrafficType.instance._trafficType,
                        TrafficType.instance.parseGPReferrer(referrer)
                    )
                }
            }
        }
    }

    fun doAdsWork() {
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "knock knock ...")
        }

        CoroutineScope(Dispatchers.Main).launch {
            if (!SSDK.instance.sdkEnable()) {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().w("SDK is not enable")
                }
                return@launch
            }

            if(!Switch.DEB_ENABLE){
                if (getRetentionTimeMills() < TimeUnit.SECONDS.toMillis(SunConfig.instance.getNewUserActivateTimeSeconds())) {
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().w("install time is not enough")
                    }
                    return@launch
                }
            }

            val hasAdsQuota = Record.instance.hasAdsQuota()
            val hasWebOfferQuota = Record.instance.hasWebOfferQuota();
            val type = when {
                hasAdsQuota && hasWebOfferQuota -> {
                    TRIGGERTYPE_BOTH
                }
                Record.instance.hasAdsQuota() -> {
                    TRIGGERTYPE_INTERS
                }
                Record.instance.hasWebOfferQuota() -> {
                    TRIGGERTYPE_WEB
                }
                else -> {
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().w(TAG, "outers quota is not enough")
                    }
                    return@launch
                }
            }

            if (isScreenOn(SRAPP.app)) {
                scheduleOneshotAd(Random.nextLong(120, 150))
            } else {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().w(TAG, "screen off")
                }

                scheduleOneshotAd(Random.nextLong(240, 300))

                return@launch
            }

            if (AppLifecycleTracker.instance.isAppForeground()) {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().w(TAG, "App is in foreground, don't show popup ad")
                }
                return@launch
            }

            delay(3000)//延迟3s，给广告缓存留下足够时间
            startPopup(SRAPP.app, type)
        }
    }

    private fun startPopup(app: Application, type: Int) {
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "trying to start Outers Activity, type=${if(1==type) "Web" else "PopupAds"}")
        }

        val intent = Intent(app, SRA::class.java)
        intent.putExtra(TRIGGERTYPE, type)
        intent.addFlags(
            Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TOP
                    or Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS
        )
        app.startActivity(intent)
    }

    private fun isScreenOn(context: Context): Boolean {
        val pm = context.getSystemService(Context.POWER_SERVICE) as PowerManager
        val isOpen = pm.isInteractive
        val km = context.getSystemService(Context.KEYGUARD_SERVICE) as KeyguardManager
        val flag = km.isKeyguardLocked
        return isOpen && !flag
    }

    private fun getRetentionTimeMills(): Long {
        try {
            val pkg = SRAPP.app.packageManager
            val pki = pkg.getPackageInfo(SRAPP.app.packageName, 0)
            return System.currentTimeMillis() - pki.firstInstallTime
        } catch (e: Exception) {
            if (Switch.LOG_ENABLE)
                e.printStackTrace()
        }
        return 0L
    }
}