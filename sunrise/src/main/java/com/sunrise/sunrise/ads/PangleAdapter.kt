package com.sunrise.sunrise.ads

import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.graphics.Typeface
import android.graphics.drawable.ShapeDrawable
import android.graphics.drawable.shapes.OvalShape
import android.text.TextUtils
import android.view.View
import android.view.ViewGroup
import android.widget.*
import com.bumptech.glide.Glide
import com.bytedance.sdk.openadsdk.api.init.PAGConfig
import com.bytedance.sdk.openadsdk.api.init.PAGSdk
import com.bytedance.sdk.openadsdk.api.interstitial.PAGInterstitialAd
import com.bytedance.sdk.openadsdk.api.interstitial.PAGInterstitialAdInteractionListener
import com.bytedance.sdk.openadsdk.api.interstitial.PAGInterstitialAdLoadListener
import com.bytedance.sdk.openadsdk.api.interstitial.PAGInterstitialRequest
import com.bytedance.sdk.openadsdk.api.nativeAd.PAGNativeAd
import com.bytedance.sdk.openadsdk.api.nativeAd.PAGNativeAdInteractionListener
import com.bytedance.sdk.openadsdk.api.nativeAd.PAGNativeAdLoadListener
import com.bytedance.sdk.openadsdk.api.nativeAd.PAGNativeRequest
import com.sunrise.sunrise.control.SunConfig
import com.sunrise.sunrise.support.DensityUtil
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Record
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.util.concurrent.ConcurrentLinkedQueue
import java.util.concurrent.atomic.AtomicBoolean

class PangleAdapter private constructor() {
    companion object {
        private val TAG = PangleAdapter::class.java.simpleName

        val instance: PangleAdapter by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            PangleAdapter()
        }
    }

    private val nativeCache = ConcurrentLinkedQueue<PAGNativeAd>()
    private val intersCache = ConcurrentLinkedQueue<PAGInterstitialAd>()

    private val nativeLoading = AtomicBoolean(false)
    private val intersLoading = AtomicBoolean(false)

    private val initing = AtomicBoolean(false)
    private val name = Record.TYPE_PANGLE


    /**
     * 初始化Pangle SDK
     */
    fun init(context: Context, callback: (() -> Unit)?) {
        if (!Switch.PAN_ENABLE)
            return

        if (PAGSdk.isInitSuccess())
            return

        val appId = SunConfig.PangleConfig.appId
        if (TextUtils.isEmpty(appId)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "init pangle sdk, appId is empty")
            }
            return
        }

        val appContext = context.applicationContext
        if (appContext == null) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "init, application context is null")
            }
            return
        }

        if (initing.compareAndSet(false, true)) {
            CoroutineScope(Dispatchers.Main).launch {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "init, doing initialization...")
                }

                PAGSdk.init(
                    appContext,
                    PAGConfig.Builder()
                        .appId(appId)
                        .useTextureView(hasWakeLockPermission(context))
                        .debugLog(Switch.DEB_ENABLE)
                        .supportMultiProcess(false)
                        .setChildDirected(0) //Set the configuration of COPPA, 0:adult, 1:child
                        .setGDPRConsent(1) //Set the configuration of GDPR, 0:User doesn't grant consent, 1: User has granted the consent
                        .setDoNotSell(0) //Set the configuration of CCPA, 0: "sale" of personal information is permitted, 1: user has opted out of "sale" of personal information
                        .build(),
                    object : PAGSdk.PAGInitCallback {
                        override fun success() {
                            initing.set(false)

                            //检查状态
                            if (PAGSdk.isInitSuccess()) {
                                if (Switch.LOG_ENABLE) {
                                    LogUtil.getInstance().d(TAG, "init, success")
                                }
                                callback?.invoke()
                            }
                        }

                        override fun fail(code: Int, msg: String?) {
                            initing.set(false)

                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance()
                                    .e(TAG, "init, fail, code=${code}, msg=${msg}")
                            }
                        }
                    })
            }
        }
    }


    /*-----------------------------native-----------------------------*/
    fun hasNative(): Boolean {
        return !nativeCache.isEmpty()
    }

    fun loadNative(attempt: Int = 1) {
        if (!Switch.PAN_ENABLE) {
            return
        }

        if (!PAGSdk.isInitSuccess()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().e(TAG, "loadNative, Pangle sdk isn't initialized!")
            }
            return
        }

        if (hasNative()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "loadNative, has native ads cached")
            }
            return
        }

        val nativeId = SunConfig.PangleConfig.nativeId
        if (TextUtils.isEmpty(nativeId)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "loadNative, native unit id is empty")
            }
            return
        }

        if (nativeLoading.compareAndSet(false, true)) {
            CoroutineScope(Dispatchers.IO).launch {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "loadNative, loading")
                }

                PAGNativeAd.loadAd(
                    nativeId,
                    PAGNativeRequest(),
                    object : PAGNativeAdLoadListener {
                        override fun onError(code: Int, message: String?) {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().e(TAG, "loadNative, onError:$code - $message")
                            }
                            nativeLoading.set(false)

                            if (attempt < 2) {//允许10s后再重试一次
                                CoroutineScope(Dispatchers.Default).launch {
                                    delay(10 * 1000)//10 seconds
                                    loadNative(attempt + 1)
                                }
                            }
                        }

                        override fun onAdLoaded(pagNativeAd: PAGNativeAd) {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().d(TAG, "loadNative, onAdLoaded")
                            }
                            nativeCache.add(pagNativeAd)
                            nativeLoading.set(false)
                        }
                    })
            }
        }

    }

    fun renderNative(activity: Activity, container: ViewGroup, medium: Boolean,
                     adShowCallback: (() -> Unit)? = null, adEndCallback: (() -> Unit)? = null) {
        CoroutineScope(Dispatchers.Main).launch {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "renderNative, container:${container.hashCode()}")
            }

            val pagNativeAd: PAGNativeAd? = nativeCache.poll()//从缓存中取出

            val adData = pagNativeAd?.nativeAdData
            adData?.let {
                val context = container.context ?: return@launch

                val nativeAdView = RelativeLayout(context).apply {
                    layoutParams = RelativeLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT
                    )
                    setBackgroundColor(resources.getColor(android.R.color.background_light))
                }

                //icon
                val icon = ImageView(context).apply {
                    layoutParams = RelativeLayout.LayoutParams(
                        DensityUtil.dp2px(50.0F), DensityUtil.dp2px(50.0F)
                    ).apply {
                        addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE)
                        addRule(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.TRUE)
                        marginStart = 10
                    }
                    id = View.generateViewId()
                }
                nativeAdView.addView(icon)
                if (it.icon != null && it.icon.imageUrl != null) {
                    if (!activity.isDestroyed && !activity.isFinishing) {
                        Glide.with(activity).load(it.icon.imageUrl).into(icon)
                    }
                }

                //title
                val title = TextView(context).apply {
                    layoutParams = RelativeLayout.LayoutParams(
                        ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
                    ).apply {
                        addRule(RelativeLayout.END_OF, icon.id)
                        addRule(RelativeLayout.ALIGN_TOP, icon.id)
                        marginStart = 5
                    }
                    typeface = Typeface.defaultFromStyle(Typeface.BOLD)
                    setTextColor(resources.getColor(android.R.color.darker_gray))
                }
                nativeAdView.addView(title)
                title.text = it.title

                //dislike
                val dislike = ImageView(context).apply{
                    layoutParams = RelativeLayout.LayoutParams(
                        DensityUtil.dp2px(10.0F), DensityUtil.dp2px(10.0F)
                    ).apply{
                        addRule(RelativeLayout.END_OF, icon.id)
                        addRule(RelativeLayout.ALIGN_BOTTOM, icon.id)
                    }
                    background = ShapeDrawable(OvalShape()).apply {
                        paint.color = 0x4DC5C4C4
                    }
                    setPadding(DensityUtil.dp2px(10.0F), 0, 0, 0)
                    id = View.generateViewId()
                }
                nativeAdView.addView(dislike)

                //buttonText
                val btn = Button(context).apply {
                    layoutParams = RelativeLayout.LayoutParams(
                        DensityUtil.dp2px(120.0F), DensityUtil.dp2px(50.0F)
                    ).apply {
                        addRule(RelativeLayout.ALIGN_TOP, icon.id)
                        addRule(RelativeLayout.ALIGN_PARENT_END, RelativeLayout.TRUE)
                        marginEnd = 5
                    }
                    text = "Learn More"
                    typeface = Typeface.defaultFromStyle(Typeface.BOLD)
                }
                nativeAdView.addView(btn)
                btn.text = if (TextUtils.isEmpty(it.buttonText)) "..." else it.buttonText


                //logo
                val logo = FrameLayout(context).apply {
                    layoutParams = RelativeLayout.LayoutParams(
                        DensityUtil.dp2px(20.0F), DensityUtil.dp2px(10.0F)
                    ).apply {
                        addRule(RelativeLayout.END_OF, dislike.id)
                        addRule(RelativeLayout.ALIGN_BOTTOM, icon.id)
                    }
                }
                nativeAdView.addView(logo)
                if (it.adLogoView != null) {
                    logo.addView(it.adLogoView)
                }

                //nativeMain
                val nativeMain = FrameLayout(context).apply{
                    layoutParams = RelativeLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT, DensityUtil.dp2px(150.0F)
                    ).apply {
                        addRule(RelativeLayout.ALIGN_PARENT_START, RelativeLayout.TRUE)
                        addRule(RelativeLayout.BELOW, icon.id)
                    }
                    id = View.generateViewId()
                }
                nativeAdView.addView(nativeMain)
                if (medium) {
                    nativeMain.visibility = View.VISIBLE
                    val mediaView: View = it.mediaView
                    if (mediaView.parent == null) {
                        nativeMain.removeAllViews()
                        nativeMain.addView(mediaView)
                    }
                } else {
                    nativeMain.visibility = View.GONE
                }

                //description
                val description = TextView(context).apply {
                    layoutParams = RelativeLayout.LayoutParams(
                        ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
                    ).apply {
                        addRule(RelativeLayout.ALIGN_PARENT_START, RelativeLayout.TRUE)
                        addRule(RelativeLayout.BELOW, nativeMain.id)
                        marginStart = DensityUtil.dp2px(9.0F)
                        topMargin = DensityUtil.dp2px(5.0F)
                    }
                    setTextColor(resources.getColor(android.R.color.darker_gray))
                }
                nativeAdView.addView(description)
                description.text = it.description


                /** The views that can be clicked  */
                val clickViewList: MutableList<View> = ArrayList()
                clickViewList.add(nativeAdView)

                /** The views that can trigger the creative action (like download app)  */
                val creativeViewList: MutableList<View> = ArrayList()
                creativeViewList.add(btn)

                pagNativeAd.registerViewForInteraction(
                    nativeAdView as ViewGroup,
                    clickViewList,
                    creativeViewList,
                    dislike,
                    object : PAGNativeAdInteractionListener {
                        override fun onAdShowed() {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance()
                                    .d(TAG, "renderNative, onAdShowed:${container.hashCode()}")
                            }

                            adShowCallback?.let {
                                CoroutineScope(Dispatchers.Main).launch {
                                    adShowCallback.invoke()
                                }
                            }
                        }

                        override fun onAdClicked() {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance()
                                    .d(TAG, "renderNative, onAdClicked:${container.hashCode()}")
                            }
                        }

                        override fun onAdDismissed() {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance()
                                    .w(TAG, "renderNative, onAdDismissed:${container.hashCode()}")
                            }
                            container.removeAllViews()

                            adEndCallback?.let {
                                CoroutineScope(Dispatchers.Main).launch {
                                    adEndCallback.invoke()
                                }
                            }
                        }
                    })

                container.removeAllViews()
                container.addView(nativeAdView)

                //广告被使用后，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
                AdsMaster.instance.loadNative(medium)
            }
        }
    }


    fun clearNative(container: ViewGroup?) {
        /*nothing to do*/
    }

    fun isLoading(): Boolean {
        return nativeLoading.get()
    }


    /*-----------------------------interstitial-----------------------------*/
    fun hasInters(): Boolean {
        return !intersCache.isEmpty() && Switch.PAN_ENABLE
    }

    fun loadInters(attempt: Int = 1) {
        if (!Switch.PAN_ENABLE) {
            return
        }

        if (!PAGSdk.isInitSuccess()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().e(TAG, "loadInters, Pangle sdk isn't initialized!")
            }
            return
        }

        if (hasInters()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "loadInters, has inters cached")
            }
            return
        }

        val intersId = SunConfig.PangleConfig.intersId
        if (TextUtils.isEmpty(intersId)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "loadInters, ad unit id is empty")
            }
            return
        }

        if (intersLoading.compareAndSet(false, true)) {
            CoroutineScope(Dispatchers.Main).launch {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "loadInters, loading...")
                }

                PAGInterstitialAd.loadAd(
                    intersId,
                    PAGInterstitialRequest(),
                    object : PAGInterstitialAdLoadListener {
                        override fun onError(code: Int, message: String?) {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().e(TAG, "loadInters, onError:$code - $message")
                            }
                            intersLoading.set(false)

                            if (attempt < 2) {//允许10s后再重试一次
                                CoroutineScope(Dispatchers.Default).launch {
                                    delay(10 * 1000)//10 seconds
                                    loadInters(attempt + 1)
                                }
                            }
                        }

                        override fun onAdLoaded(interstitialAd: PAGInterstitialAd?) {
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().d(TAG, "loadInters, onAdLoaded")
                            }
                            if (interstitialAd != null) {
                                intersCache.add(interstitialAd)
                            }

                            intersLoading.set(false)
                        }
                    })
            }
        }
    }

    fun showInters(activity: Activity?, callback: AdsImpListener?) {
        CoroutineScope(Dispatchers.Main).launch {
            val pagInterAd = intersCache.poll()
            if (pagInterAd != null) {
                pagInterAd.setAdInteractionListener(object : PAGInterstitialAdInteractionListener {
                    override fun onAdShowed() {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().d(TAG, "showInters, onAdShowed")
                        }

                        CoroutineScope(Dispatchers.Main).launch {
                            callback?.onShow(name)
                        }
                    }

                    override fun onAdClicked() {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().d(TAG, "showInters, onAdClicked")
                        }
                    }

                    override fun onAdDismissed() {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().d(TAG, "showInters, onAdDismissed")
                        }

                        CoroutineScope(Dispatchers.Main).launch {
                            callback?.onDismiss(name)
                        }

                        //广告关闭时，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
                        AdsMaster.instance.loadInters()
                    }
                })

                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "showInters, show interstitial ads...")
                }
                pagInterAd.show(activity)
            } else {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().e(TAG, "showInters, doesn't have inters ads cached")
                }

                CoroutineScope(Dispatchers.Main).launch {
                    callback?.onShowFail(name)
                }

                //无广告时，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
                AdsMaster.instance.loadInters()
            }
        }
    }


    private fun hasWakeLockPermission(context: Context): Boolean {
        try {
            val pm = context.packageManager
            return PackageManager.PERMISSION_GRANTED == pm.checkPermission(
                "android.permission.WAKE_LOCK",
                context.packageName
            )
        } catch (e: java.lang.Exception) {
            if (Switch.LOG_ENABLE) {
                e.printStackTrace()
            }
        }
        return false
    }
}