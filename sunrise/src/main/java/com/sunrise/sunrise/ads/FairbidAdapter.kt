package com.sunrise.sunrise.ads

import android.app.Activity
import android.text.TextUtils
import com.fyber.FairBid
import com.fyber.fairbid.ads.ImpressionData
import com.fyber.fairbid.ads.Interstitial
import com.fyber.fairbid.ads.interstitial.InterstitialListener
import com.fyber.fairbid.ads.mediation.MediatedNetwork
import com.fyber.fairbid.ads.mediation.MediationStartedListener
import com.fyber.fairbid.user.UserInfo
import com.fyber.marketplace.fairbid.bridge.MarketplaceBridge
import com.sunrise.sunrise.control.SunConfig
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Record
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicReference

class FairbidAdapter private constructor() {
    companion object {
        private val TAG = FairbidAdapter::class.java.simpleName

        val instance: FairbidAdapter by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            FairbidAdapter()
        }
    }

    private val initing = AtomicBoolean(false) //正在初始化


    private val intLoading = AtomicBoolean(false)

    private val impCallbackRef = AtomicReference<AdsImpListener?>()

    private val name = Record.TYPE_FAIRBID

    fun init(activity: Activity, callback: (() -> Unit)?) {
        if (!Switch.FAIR_ENABLE) {
            return
        }


        if (FairBid.hasStarted())
            return


        val appId = SunConfig.FairbidConfig.appId
        if (TextUtils.isEmpty(appId)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().e(TAG, "init sdk, appId is null")
            }

            return
        }


        if (initing.compareAndSet(false, true)) {
            CoroutineScope(Dispatchers.Main).launch {
                UserInfo.setGdprConsent(true, activity.applicationContext)
                UserInfo.setLgpdConsent(true, activity.applicationContext)

                val bid = FairBid.configureForAppId(appId)
                if (Switch.LOG_ENABLE) {
                    bid.enableLogs()
                }

//                if (Switch.DEB_ENABLE) {
                    bid.withMediationStartedListener(object : MediationStartedListener {
                        override fun onNetworkFailedToStart(
                            network: MediatedNetwork,
                            errorMessage: String
                        ) {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().w(
                                    TAG,
                                    "init, onNetworkFailedToStart, adn ${network.name}/${network.version}, init err:$errorMessage"
                                )
                            }
                        }

                        override fun onNetworkStarted(network: MediatedNetwork) {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().v(
                                    TAG,
                                    "init, onNetworkStarted, adn ${network.name}/${network.version}"
                                )
                            }
//mtg is split from fairbid as an independent network.
                        if (network.name.contains("Mintegral", true)
                            || network.name.contains("MAL", true)
                            || network.name.contains("mtg",true)){
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().v(TAG, "Mintegral is inited")
                            }
                            MtgAdapter.instance.mtgInited = true
                        }
                        }
                    })
//                }
                bid.start(activity)

                initing.set(false)

                if (Switch.LOG_ENABLE) {
                    /**
                     * 必须在app/build.gradle中引入对应版本的fairbid plugin，如：
                     * id 'com.fyber.fairbid-sdk-plugin' version '3.30.1'
                     */
                    LogUtil.getInstance().v(TAG, "init, FairBid.hasStarted=${FairBid.hasStarted()}, marketplace ver=${MarketplaceBridge.getVersion()}")
                }
                while (!FairBid.hasStarted()) {//不确定fairbid sdk什么时候初始化完成
                    delay(1000)
                }

                Interstitial.setInterstitialListener(object : InterstitialListener {
                    override fun onRequestStart(placementId: String) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().v(
                                TAG,
                                "Interstitial, onRequestStart, plcId:${placementId}"
                            )
                        }
                    }

                    override fun onUnavailable(placementId: String) {
                        intLoading.set(false)

                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().w(
                                TAG,
                                "Interstitial, onUnavailable, plcId:${placementId}"
                            )
                        }
                    }

                    override fun onAvailable(placementId: String) {
                        intLoading.set(false)

                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().v(
                                TAG,
                                "Interstitial, onAvailable, plcId:${placementId}"
                            )
                        }
                    }

                    override fun onShow(
                        placementId: String,
                        impressionData: ImpressionData
                    ) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance()
                                .v(TAG, "Interstitial, onShow, plcId:${placementId}")
                        }

                        CoroutineScope(Dispatchers.Main).launch {
                            impCallbackRef.get()?.onShow(name)
                        }
                    }

                    override fun onClick(placementId: String) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance()
                                .v(TAG, "Interstitial, onClick, plcId:${placementId}")
                        }
                    }

                    override fun onShowFailure(
                        placementId: String,
                        impressionData: ImpressionData
                    ) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().e(
                                TAG,
                                "Interstitial, onShowFailure, plcId:${placementId}"
                            )
                        }

                        CoroutineScope(Dispatchers.Main).launch {
                            impCallbackRef.get()?.onDismiss(name)
                            impCallbackRef.set(null)
                        }

                        //广告展示失败，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
                        AdsMaster.instance.loadInters()
                    }

                    override fun onHide(placementId: String) {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance()
                                .v(TAG, "Interstitial, onHide, plcId:${placementId}")
                        }

                        CoroutineScope(Dispatchers.Main).launch {
                            impCallbackRef.get()?.onDismiss(name)
                            impCallbackRef.set(null)
                        }

                        //广告关闭时，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
                        AdsMaster.instance.loadInters()
                    }
                })

                callback?.invoke()
            }
        }
    }

    /**
     * 假设：
     * 1。开启了自动加载
     * 2。全屏广告ID唯一
     */

    /****************************************   Inters   **********************************************/
    fun loadInters() {
        if (!Switch.FAIR_ENABLE) {
            return
        }

        if (!FairBid.hasStarted()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "loadInters, fairbid sdk has not started yet")
            }
            return
        }

        if (hasInters()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance()
                    .v(TAG, "loadInters, fairbid sdk hold a interstitial ads already")
            }
            return
        }

        val id = SunConfig.FairbidConfig.intersId
        if (TextUtils.isEmpty(id)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance()
                    .w(TAG, "loadInters, fairbid interstitial id configured in console is empty")
            }
            return
        }

        if (intLoading.compareAndSet(false, true)) {
            CoroutineScope(Dispatchers.Main).launch {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "loadInters, manually load interstitial, plcId=${id}")
                }

                Interstitial.request(id)
            }
        }
    }

    fun hasInters(): Boolean {
        val id = SunConfig.FairbidConfig.intersId
        return !TextUtils.isEmpty(id) && Interstitial.isAvailable(id) && Switch.FAIR_ENABLE
    }

    fun showInters(activity: Activity, callback: AdsImpListener?) {
        if (hasInters()) {
            CoroutineScope(Dispatchers.Main).launch {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "showInters, showing")
                }

                impCallbackRef.set(callback)

                Interstitial.show(SunConfig.FairbidConfig.intersId, activity)
            }

        } else {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "showInters, have no interstitial ads available")
            }
            CoroutineScope(Dispatchers.Main).launch {
                callback?.onShowFail(name)
            }

            //无广告时，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
            AdsMaster.instance.loadInters()
        }
    }

}