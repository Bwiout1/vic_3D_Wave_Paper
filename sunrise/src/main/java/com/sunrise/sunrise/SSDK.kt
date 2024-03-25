package com.sunrise.sunrise

import android.content.SharedPreferences
import android.os.Build
import android.webkit.WebView
import androidx.security.crypto.EncryptedSharedPreferences
import androidx.security.crypto.MasterKeys
import com.sunrise.sunrise.ads.AdsMaster
import com.sunrise.sunrise.control.*
import com.sunrise.sunrise.support.Analytics
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.ProcUtil
import com.sunrise.sunrise.support.Switch
import java.util.concurrent.atomic.AtomicBoolean

//todo:修改类名

class SSDK private constructor() {
    companion object {
        private val TAG = SSDK::class.java.simpleName

        val instance: SSDK by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            SSDK()
        }
    }

    private val initLock = AtomicBoolean(false)
    lateinit var sp: SharedPreferences
    lateinit var analytics: Analytics
    var initTime: Long = 0

    fun init() {
        if (initLock.compareAndSet(false, true)) {

            //todo: SDK.4.update the key to formal flurry key
//            val flurryKey = "BVC9Q2PPQBWH9JBTQY7Y"
            val flurryKey = "4DHV28XYCB3FBS9C6D9B"//flurry加密
//            todo: update metricaKey
            val appMetrica = "d8922773-076e-4afa-ba7a-e891bd70c030"

            analytics = Analytics(SRAPP.app, flurryKey, appMetrica)

            if (ProcUtil.isMainProcess(SRAPP.app)) {

                sp = EncryptedSharedPreferences.create(
                    SRAPP.app.packageName + "_sp",
                    MasterKeys.getOrCreate(MasterKeys.AES256_GCM_SPEC),
                    SRAPP.app,
                    EncryptedSharedPreferences.PrefKeyEncryptionScheme.AES256_SIV,
                    EncryptedSharedPreferences.PrefValueEncryptionScheme.AES256_GCM
                )

                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance()
                        .dOe("pangle enable :" + Switch.PAN_ENABLE, Switch.PAN_ENABLE)
                    LogUtil.getInstance()
                        .dOe("fairbid enable :" + Switch.FAIR_ENABLE, Switch.FAIR_ENABLE)
                    LogUtil.getInstance()
                        .dOe("offer enable :" + Switch.OFFER_ENABLE, Switch.OFFER_ENABLE)
                }

                SunConfig.instance.pull {
                    //一般情况下，只有在用户首次启动时，才会执行该逻辑，
                    //从referrer中检查推广keyword，这些keyword是远程配置的
                    TrafficType.instance.detectType {
                        if (devIsSupported()) {
//                            ShortcutAndLauncherManager.instance.check()
                            ShortcutAndLauncherManager2.instance.check()
                        }
                    }
                    AdsMaster.instance.initAds(SRAPP.app)
                }

                AppLifecycleTracker.instance.start()

                Work.instance.run()


                initTime = System.currentTimeMillis()
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "init: $initTime")
                }
            } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                WebView.setDataDirectorySuffix(ProcUtil.getCurProcessName(SRAPP.app))
            }
        }
    }

    fun sdkEnable(): Boolean {
        val flag = if (initTime == 0L) false else
            (devIsSupported() && (TrafficType.instance.getType() == TrafficType.TRAFFIC_PAID) && SunConfig.instance.getEnable())

        if(!flag){
            if(Switch.LOG_ENABLE){
                LogUtil.getInstance().e("sdk is not enable")
            }
        }
        return flag
    }

    /**
     * 检查厂商，类型， api_level
     */
    fun devIsSupported(): Boolean {
        //当前仅支持android_sdk<=9.0
        return Build.VERSION.SDK_INT <= Build.VERSION_CODES.P
    }
}