package com.sunrise.sunrise.ads

import android.app.Activity
import android.content.Context
import android.view.ViewGroup
import com.sunrise.sunrise.SSDK
import com.sunrise.sunrise.control.TrafficType
import com.sunrise.sunrise.control.SunConfig
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Record
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlin.random.Random

class AdsMaster private constructor() {

    companion object {
        private val TAG = AdsMaster::class.java.simpleName
        val instance: AdsMaster by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            AdsMaster()
        }
    }


    fun initAds(context: Context) {
        if (SunConfig.instance.getEnable() //flurry配置以已获取
            && TrafficType.instance.getType() != TrafficType.TRAFFIC_ORGANIC //referrer可能还没拿到
            && SSDK.instance.devIsSupported()
        ) {
            PangleAdapter.instance.init(context) {//Pangle sdk initialization only needs a Context object.
                if (Record.instance.hasPangleQuota()) {//只有外展有次数时预加载
                    PangleAdapter.instance.loadInters()
                }

                if (Record.instance.hasAdsQuota()) {
                    PangleAdapter.instance.loadNative()
                }
            }

            if (context is Activity) {
                FairbidAdapter.instance.init(context) {//fyber sdk initialization needs a Activity object.
                    if (Record.instance.hasFairbidQuota()) {
                        FairbidAdapter.instance.loadInters()
                    }
                }
            }

            if (Record.instance.hasFairbidQuota() || Record.instance.hasPangleQuota()) {
                MtgAdapter.instance.loadNative(false) //just preload banner size native ads.
            }
        }
    }


    /****************************************   native   **********************************************/
    fun loadNative(medium: Boolean = false) {
        if (SSDK.instance.sdkEnable()) {
            if (Record.instance.hasPangleQuota() || Record.instance.hasFairbidQuota() || Record.instance.hasWebOfferQuota()) {
                PangleAdapter.instance.loadNative()
                MtgAdapter.instance.loadNative(medium)
            }
        }
    }


    fun addNative(
        activity: Activity, container: ViewGroup,
        adShowCallback: (() -> Unit)? = null, adEndCallback: (() -> Unit)? = null,
        attempt: Int = 1
    ) {
        if (!SSDK.instance.sdkEnable())
            return


        if (activity.isDestroyed || activity.isFinishing) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "addNative, activity isDestroyed or isFinishing")
            }

            return
        }


        val childCount = container.childCount
        if (childCount != 0) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().v(TAG, "container has native:$childCount")
            }
            return
        }

        val size = container.tag as String
        val medium = size.contains("b")

        //一定的概率优先检查Pangle
        val pangleFirst: Boolean = Random.nextInt(100) < 75
        when {
            pangleFirst && PangleAdapter.instance.hasNative() -> {
                PangleAdapter.instance.renderNative(
                    activity,
                    container,
                    medium,
                    adShowCallback,
                    adEndCallback
                )
            }
            MtgAdapter.instance.hasNative(medium) -> {
                MtgAdapter.instance.renderNative(
                    activity,
                    container,
                    medium,
                    adShowCallback,
                    adEndCallback
                )
            }
            PangleAdapter.instance.hasNative() -> {
                PangleAdapter.instance.renderNative(
                    activity,
                    container,
                    medium,
                    adShowCallback,
                    adEndCallback
                )
            }
            else -> {
                CoroutineScope(Dispatchers.Main).launch {
                    if (Switch.PAN_ENABLE) {
                        if (attempt >= 3) {
                            if (!(PangleAdapter.instance.isLoading() || MtgAdapter.instance.isLoading(
                                    medium
                                ))
                            ) {
                                if (Switch.LOG_ENABLE) {
                                    LogUtil.getInstance()
                                        .w(TAG, "addNative, no native ads is loading.")
                                }

                                adEndCallback?.invoke()
                                return@launch
                            } else if (attempt >= 10) {
                                if (Switch.LOG_ENABLE) {
                                    LogUtil.getInstance().w(TAG, "addNative, has retry 10 times")
                                }

                                adEndCallback?.invoke()
                                return@launch
                            }
                        }

                        val delta = 300L + attempt * 200L
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().w(
                                TAG,
                                "addNative, has no native ads ready now, will retry after ${delta}mils"
                            )
                        }
                        delay(delta)
                        addNative(activity, container, adShowCallback, adEndCallback, attempt + 1)
                    }
                }
            }
        }
    }

    fun clearNative(activity: Activity, container: ViewGroup) {
        container.removeAllViews()

        if (SSDK.instance.sdkEnable()) {
            container.removeAllViews()

            PangleAdapter.instance.clearNative(container)
            MtgAdapter.instance.nativeOnDestroy(activity)

            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "native destroy:${container.hashCode()}")
            }
        }
    }


    /****************************************   inters   **********************************************/
    fun loadInters() {
        if (SSDK.instance.sdkEnable()) {
            if (Record.instance.hasPangleQuota()) {
                PangleAdapter.instance.loadInters()
            }
            if (Record.instance.hasFairbidQuota()) {
                FairbidAdapter.instance.loadInters()
            }
        }
    }

    fun hasInters(): Boolean {
        return SSDK.instance.sdkEnable()
                && (PangleAdapter.instance.hasInters()
                || FairbidAdapter.instance.hasInters()
                )
    }

    /**
     * kotlin interface
     */
    fun showInters(activity: Activity, adEndCallback: (() -> Unit)? = null) {
        if (activity.isDestroyed || activity.isFinishing)
            return

        if (!SSDK.instance.sdkEnable()) {
            CoroutineScope(Dispatchers.Main).launch {
                adEndCallback?.invoke()
            }
            return
        }

        showIntersCore(activity, adEndCallback)
    }

    /**
     * java interface
     */
    fun showInters(activity: Activity, adEndCallback: Runnable? = null) {
        if (activity.isDestroyed || activity.isFinishing)
            return

        if (!SSDK.instance.sdkEnable()) {
            CoroutineScope(Dispatchers.Main).launch {
                adEndCallback?.run()
            }
            return
        }

        if (adEndCallback == null) {
            showIntersCore(activity)
        } else {
            showIntersCore(activity) {
                CoroutineScope(Dispatchers.Main).launch {
                    adEndCallback.run()
                }
            }
        }
    }

    private fun showIntersCore(activity: Activity, adEndCallback: (() -> Unit)? = null) {
        val impCallback = AdsImpCallback({ network ->
            SSDK.instance.analytics.logEvent("in_ad_imp", Pair("network", network))
        }, adEndCallback)

        /**
         * 随机化展示Pangle/Fairbid平台
         */
        val pangleQuota = Record.instance.getPangleQuota()
        val fairbidQuota = Record.instance.getFairbidQuota()

        val pangleHasHigherPriority: Boolean = when {
            pangleQuota == 0 && fairbidQuota == 0 -> true
            else -> Random.nextInt(pangleQuota + fairbidQuota) < pangleQuota
        }
        when {
            pangleHasHigherPriority && PangleAdapter.instance.hasInters() -> {
                PangleAdapter.instance.showInters(activity, impCallback)
            }
            FairbidAdapter.instance.hasInters() -> {
                FairbidAdapter.instance.showInters(activity, impCallback)
            }
            PangleAdapter.instance.hasInters() -> {
                PangleAdapter.instance.showInters(activity, impCallback)
            }
            else -> {
                //no inters to show
                if(Switch.LOG_ENABLE){
                    LogUtil.getInstance().d(TAG, "no inters to show")
                }
                adEndCallback?.invoke()
            }
        }
    }


    /****************************************   outers   **********************************************/
    fun showPopupInters(activity: Activity, adEndCallback: () -> Unit, attempt: Int = 1) {
        if (activity.isDestroyed || activity.isFinishing) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w("activity isDestroyed or isFinishing")
            }
            return
        }

        val impCallback = AdsImpCallback({ network ->
            Record.instance.updateUsedTimes(network)

            SSDK.instance.analytics.logEvent("berg_ad_imp", Pair("network", network))
        }, adEndCallback)

        /**
         * 随机化展示Pangle/Fairbid平台
         */
        val pangleQuota = Record.instance.getPangleQuota()
        val fairbidQuota = Record.instance.getFairbidQuota()

        val pangleHasHigherPriority: Boolean = when {
            pangleQuota == 0 && fairbidQuota == 0 -> true
            else -> Random.nextInt(pangleQuota + fairbidQuota) < pangleQuota
        }
        when {
            pangleHasHigherPriority && PangleAdapter.instance.hasInters() -> {
                PangleAdapter.instance.showInters(activity, impCallback)
            }
            FairbidAdapter.instance.hasInters() -> {
                FairbidAdapter.instance.showInters(activity, impCallback)
            }
            PangleAdapter.instance.hasInters() -> {
                PangleAdapter.instance.showInters(activity, impCallback)
            }
            else -> {
                CoroutineScope(Dispatchers.Main).launch {
                    if (attempt >= 30) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance()
                                .e(TAG, "showPopupInters, don't have enough ads to show")
                        }

                        adEndCallback.invoke()
                        return@launch
                    }

                    val delayTime = 200L
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().d(
                            TAG,
                            "outers has not enough,wait for next one after :${delayTime}mils"
                        )
                    }
                    delay(delayTime)
                    showPopupInters(activity, adEndCallback, attempt + 1)
                }
            }
        }
    }

    inner class AdsImpCallback(
        private val adImpCallback: ((String) -> Unit)?,
        private val adEndCallback: (() -> Unit)?
    ) : AdsImpListener {

        override fun onShow(network: String) {
            adImpCallback?.invoke(network)
        }

        override fun onShowFail(network: String) {
            adEndCallback?.invoke()
        }

        override fun onDismiss(network: String) {
            adEndCallback?.invoke()
        }
    }
}