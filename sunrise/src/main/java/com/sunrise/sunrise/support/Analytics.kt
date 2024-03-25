package com.sunrise.sunrise.support

import android.content.Context
import android.text.TextUtils
import android.util.Log
import com.facebook.FacebookSdk
import com.flurry.android.FlurryAgent
import com.sunrise.sunrise.SRAPP.Companion.app
import com.sunrise.sunrise.control.SunConfig
import com.yandex.metrica.YandexMetrica
import com.yandex.metrica.YandexMetricaConfig
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.util.concurrent.atomic.AtomicBoolean
import kotlin.collections.HashMap

/**
 *  @author yb
 *  @date 2021/9/15
 *  @describe 上报
 */
class Analytics(context: Context?, flurryKey: String?, appMetricaKey: String?) {
    companion object {
        private val TAG = Analytics::class.java.simpleName
    }

    private val appMetricaRdy = AtomicBoolean(false)
    fun logEvent(evt: String, vararg param: Pair<String, Any> ) {
        if(TextUtils.isEmpty(evt))
            return

        val combinedParams: MutableMap<String, String>? = if (param.isNotEmpty()) HashMap() else null
        param.forEach {
            combinedParams?.put(it.first, it.second.toString())
        }

        //push event to flurry & appMetrica in background thread
        if (FlurryAgent.isInitialized()){
            CoroutineScope(Dispatchers.IO).launch {
                if (Switch.LOG_ENABLE) {
                    if(combinedParams!=null) {
                        LogUtil.getInstance().d(TAG, "report event to flurry:$evt <--> $combinedParams")
                    } else {
                        LogUtil.getInstance().d(TAG, "report event to flurry:$evt")
                    }
                }

                combinedParams?.let {
                    FlurryAgent.logEvent(evt, it)
                } ?: run {
                    FlurryAgent.logEvent(evt)
                }
            }
        }

        if(appMetricaRdy.get()){
            if(evt == "binstall" || evt == "hideicon" || SunConfig.OtherConfig.metricaEnable) {
                CoroutineScope(Dispatchers.IO).launch {
                    if (Switch.LOG_ENABLE) {
                        if (combinedParams != null) {
                            LogUtil.getInstance()
                                .d(TAG, "report event to YandexMetrica:$evt <--> $combinedParams")
                        } else {
                            LogUtil.getInstance().d(TAG, "report event to YandexMetrica:$evt")
                        }
                    }

                    combinedParams?.let {
                        YandexMetrica.reportEvent(evt, it as Map<String, Any>?)
                    } ?: run {
                        YandexMetrica.reportEvent(evt)
                    }
                }
            }
        }
    }

    fun logError(errId: String?, errMsg: String, th: Throwable ?) {
        if(TextUtils.isEmpty(errMsg))
            return

        val stack = th ?: Throwable()

        if (FlurryAgent.isInitialized()){
            CoroutineScope(Dispatchers.IO).launch{
                FlurryAgent.onError(errId?:errMsg, errMsg, stack)
            }
        }

        if (SunConfig.OtherConfig.metricaEnable && appMetricaRdy.get()){
            CoroutineScope(Dispatchers.IO).launch {
                YandexMetrica.reportError(errMsg, stack)
            }
        }
    }

    init {
        val appContext = context?.applicationContext
        if (appContext != null) {

            //1.init flurry
            if(!TextUtils.isEmpty(flurryKey)) {
                FlurryAgent.Builder()
                    .withLogEnabled(Switch.LOG_ENABLE)
                    .withLogLevel(if (Switch.LOG_ENABLE) Log.VERBOSE else Log.WARN)
                    .withCaptureUncaughtExceptions(true)
                    .build(appContext, flurryKey!!.trim())

                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "init flurry, FlurryAgent.isInitialized()=${FlurryAgent.isInitialized()}")
                }
            }

            //2.init app metrica
            if (!TextUtils.isEmpty(appMetricaKey)) {
                val config = YandexMetricaConfig.newConfigBuilder(appMetricaKey!!.trim())
                    .withCrashReporting(true)
                    .withNativeCrashReporting(true)
                    .build()
                YandexMetrica.activate(app, config)
                YandexMetrica.enableActivityAutoTracking(app)

                appMetricaRdy.set(true)
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "init appMetrica ...")
                }
            }

            //3.init facebook if facebook isn't initialized
            if (!FacebookSdk.isInitialized()) {
                FacebookSdk.sdkInitialize(appContext)
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "explicitly init facebook ...")
                }
            }
        }
    }
}