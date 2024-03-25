package com.sunrise.sunrise;

import android.annotation.SuppressLint
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.drawable.BitmapDrawable
import android.os.Build
import android.os.Bundle
import android.text.TextUtils
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.view.WindowManager
import android.webkit.*
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.ProgressBar
import androidx.fragment.app.FragmentActivity
import com.sunrise.sunrise.ads.AdsMaster
import com.sunrise.sunrise.control.SunConfig
import com.sunrise.sunrise.control.TrafficType
import com.sunrise.sunrise.control.Work
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Record
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.util.concurrent.atomic.AtomicBoolean
import kotlin.random.Random

//todo:修改类名

open class PPPA : FragmentActivity() {
    companion object {
        private val TAG = PPPA::class.java.simpleName
    }

    private var webHolder:FrameLayout? = null
    private var xBtn:ImageView? = null
    private var adHolder:FrameLayout? = null

    private val checkIntersOnHide = AtomicBoolean(false)
    private var sysKeyListener: BroadcastReceiver? = null

    private val name = Record.TYPE_WEB

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initView()

        /**
         * 不管triggerType为何数据，总是上报事件
         */
        val triggerType = intent.getIntExtra(Work.TRIGGERTYPE, Work.TRIGGERTYPE_UNKWON)

        SSDK.instance.analytics.logEvent(
            "berg_popup_act",
            Pair("dev", TrafficType.instance.getDevMeta()),
            Pair(
                "type", when (triggerType) {
                    Work.TRIGGERTYPE_WEB -> "web"
                    Work.TRIGGERTYPE_INTERS -> "inters"
                    Work.TRIGGERTYPE_BOTH -> "both"
                    else -> "unknown-${triggerType}"
                }
            )
        )
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "onCreate, triggerType=${triggerType}, 1:web, 2:ads")
        }

        /**
         * 当triggerType为非法类型时，直接关闭
         */
        if ( ! (triggerType==Work.TRIGGERTYPE_WEB || triggerType==Work.TRIGGERTYPE_INTERS || triggerType==Work.TRIGGERTYPE_BOTH)){
            finish()
            return
        }

        sysKeyListener = object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                val action = intent.action
                if (action != null && Intent.ACTION_CLOSE_SYSTEM_DIALOGS == action) {
                    val reason = intent.getStringExtra("reason")
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().d(TAG, "System key process, $reason")
                    }

                    //在10上如果隐藏了虚拟按键，无论是按home还是recent都会为发送recentapps通知，即无homekey通知；未隐藏的则正常发送通知
                    if (reason == "homekey") {
                    } else if (reason == "recentapps") {
                    }

                    if (checkIntersOnHide.compareAndSet(true, false)){
                        AdsMaster.instance.showPopupInters(this@PPPA, {finish()})
                    }
                }
            }
        }
        //注册home/recent键监听
        registerReceiver(sysKeyListener, IntentFilter(Intent.ACTION_CLOSE_SYSTEM_DIALOGS))



        checkIntersOnHide.set(triggerType==Work.TRIGGERTYPE_INTERS || triggerType==Work.TRIGGERTYPE_BOTH)
        when (triggerType) {
            Work.TRIGGERTYPE_WEB, Work.TRIGGERTYPE_BOTH -> {
                showWebFirst()
            }

            Work.TRIGGERTYPE_INTERS -> {
                //                如果当前没有可用全屏广告，可以尝试展示native广告， 当全屏广告ready时，全屏广告自动弹出
    //                此种情况：a.不显示close按钮
    //                        b.native广告可能曝光时间不够，
                var hasNative = false
                if (!AdsMaster.instance.hasInters()){
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().d(TAG, "onCreate, no inters ads ready, try to render native ads")
                    }

                    hasNative = true
                    adHolder?.let{
                        AdsMaster.instance.addNative(this, it)
                    }
                }

                if (checkIntersOnHide.compareAndSet(true, false)) {
                    AdsMaster.instance.showPopupInters(this, {
                        CoroutineScope(Dispatchers.Main).launch {
                            if (hasNative) {
                                if (Switch.LOG_ENABLE) {
                                    LogUtil.getInstance().d(
                                        TAG,
                                        "onCreate, inters ads dismissed, delay 500ms to increase native ads impression duration"
                                    )
                                }
                                delay(500)//全屏广告结束后，延迟500ms再关闭页面，增加native广告曝光成功率
                            }

                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().d(TAG, "onCreate, start to close page")
                            }
                            finish()
                        }
                    })
                }
            }
            else -> {
                finish()
            }
        }
    }

    private fun initView() {
        val layout = FrameLayout(this)

        val progBar = ProgressBar(this, null, android.R.attr.progressBarStyleLarge)
        progBar.isIndeterminate = true
        progBar.layoutParams = FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.CENTER
        )
        layout.addView(progBar)

        webHolder = FrameLayout(this)
        webHolder?.layoutParams = FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
        )
        layout.addView(webHolder)

        xBtn = ImageView(this)
        xBtn?.layoutParams = FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.TOP or Gravity.END
        ).apply {
            topMargin = 30
            marginEnd = 30
        }
        val bitmap = Bitmap.createBitmap(80, 80, Bitmap.Config.ARGB_8888)
        val canvas = Canvas(bitmap)
        val paint = Paint()
        paint.apply {
            color = 0x88000000.toInt()
            style = Paint.Style.STROKE
            strokeWidth = 5.0F
            isAntiAlias = true
        }
        canvas.drawCircle(40.0F, 40.0F, 37.0F, paint)
        val paths = floatArrayOf(
            25.0F, 25.0F, 55.0F, 55.0F,
            55.0F, 25.0F, 25.0F, 55.0F
        )
        canvas.drawLines(paths, paint)
        val drawable = BitmapDrawable(resources, bitmap)
        xBtn?.background = drawable
        xBtn?.visibility = View.INVISIBLE
        layout.addView(xBtn)


        adHolder = FrameLayout(this)
        adHolder?.layoutParams = FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.BOTTOM
        )
        adHolder?.tag = "small"
        layout.addView(adHolder)

        setContentView(layout)

        window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN or WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
            window.decorView.systemUiVisibility =
                View.SYSTEM_UI_FLAG_FULLSCREEN or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        } else {
            /*nothing to do*/
            //window.decorView.windowInsetsController
        }
    }

    private fun showWebFirst(){
        adHolder?.let{
            AdsMaster.instance.addNative(this, it, {//adShowCallback
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().d(TAG, "showWebFirst, native ads imp callback, start to show native ads")
                }

                CoroutineScope(Dispatchers.Main).launch{
                    /**
                     * native广告展示后，选择进入全屏广告的时机时，有两个方案：
                     * 1.使能xBtn，用户点击xBtn后展示全屏广告；
                     * 2.native曝光一定时间后后自动展示全屏广告
                     * 本实现采用第一种方案
                     */
                    if (checkIntersOnHide.compareAndSet(true, false)) {
                        xBtn?.visibility = View.VISIBLE
                        xBtn?.setOnClickListener {
                            AdsMaster.instance.showPopupInters(this@PPPA, {
                                CoroutineScope(Dispatchers.Main).launch {
                                    if (Switch.LOG_ENABLE) {
                                        LogUtil.getInstance().d(TAG,"showWebFirst, inters ads dismissed, delay 800ms to increase native ads impression duration")
                                    }
                                    delay(800)//全屏广告结束后，延迟800ms再关闭页面，增加native广告曝光成功率
                                    if (Switch.LOG_ENABLE) {
                                        LogUtil.getInstance().d(TAG,"showWebFirst, inters ads dismissed, start to close page")
                                    }
                                    finish()
                                }
                            })
                        }

                    } else { //如果不需要展示全屏广告，当native广告成功曝光>3s后，自动关闭页面
                        CoroutineScope(Dispatchers.Main).launch {
                            val postpone = 3000+Random.nextLong(2000)
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().d(TAG, "showWebFirst, no inters ads, delay ${postpone}ms to close page")
                            }
                            delay(postpone)

                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().d(TAG, "showWebFirst, no inters ads, start to close page")
                            }
                            finish()
                        }
                    }

                }
            }, {//adEndCallback:a.native广告加载失败，b.用户点击native广告的dislike按钮，
                if (checkIntersOnHide.compareAndSet(true, false)) {
                    AdsMaster.instance.showPopupInters(this, {
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().d(TAG, "showWebFirst, no native ads, inters ads dismissed, start to close page")
                        }
                        finish()
                    })
                } else {
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().d(TAG, "showWebFirst, no native ads, no inters ads, start to close page")
                    }
                    finish()
                }
            })
        }

        loadWeb()
    }


    private fun loadWeb() {
        val url = SunConfig.instance.getWebOfferUrl()
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().v(TAG, "loadWeb，url:$url")
        }

        if (TextUtils.isEmpty(url)){
            return
        }

        val webView = createWeb()
        webView.webViewClient = object : WebViewClient() {
            override fun shouldOverrideUrlLoading(
                view: WebView?, request: WebResourceRequest?
            ): Boolean {
                return super.shouldOverrideUrlLoading(view, request)
            }

            override fun onPageFinished(view: WebView?, url: String?) {
                super.onPageFinished(view, url)
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "web page finished")
                }
            }
        }
        var cusView: View? = null
        webView.webChromeClient = object : WebChromeClient() {
            override fun onShowCustomView(view: View?, callback: CustomViewCallback?) {
                super.onShowCustomView(view, callback)
                cusView = view
                webHolder?.addView(cusView)
                webView.visibility = View.GONE
                xBtn?.visibility = View.GONE
            }

            override fun onHideCustomView() {
                super.onHideCustomView()
                cusView?.let {
                    webHolder?.removeView(it)
                }
                webView.visibility = View.VISIBLE
                xBtn?.visibility = View.VISIBLE
            }
        }
        webView.loadUrl(url)
        webHolder?.addView(webView)

        Record.instance.updateUsedTimes(name)
    }

    @SuppressLint("SetJavaScriptEnabled")
    private fun createWeb(): WebView {
        val webView = WebView(this)
        webView.setBackgroundColor(0x00000000)
        val settings = webView.settings
        settings.javaScriptEnabled = true
        settings.useWideViewPort = true
        settings.loadWithOverviewMode = true
        settings.setSupportZoom(true)
        settings.builtInZoomControls = true
        settings.mediaPlaybackRequiresUserGesture = false
        settings.displayZoomControls = false
        settings.domStorageEnabled = true
        settings.setGeolocationEnabled(true)
        settings.javaScriptCanOpenWindowsAutomatically = false
        settings.setNeedInitialFocus(true)
        settings.saveFormData = false
        settings.setSupportMultipleWindows(false)
        settings.layoutAlgorithm = WebSettings.LayoutAlgorithm.NORMAL
//        val cacheDirPath = filesDir.absolutePath + "cache/"
//        settings.setAppCachePath(cacheDirPath)
//        settings.setAppCacheEnabled(true)
        settings.databaseEnabled = true
        settings.cacheMode = WebSettings.LOAD_DEFAULT
        return webView
    }


    override fun onBackPressed() {
        if (checkIntersOnHide.compareAndSet(true, false)){
            AdsMaster.instance.showPopupInters(this@PPPA, {finish()})
        } else {
            super.onBackPressed()
        }
    }


    override fun onDestroy() {
        adHolder?.removeAllViews()
        sysKeyListener?.let {
            unregisterReceiver(sysKeyListener)
        }

        super.onDestroy()

        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().v(TAG, "onDestroy")
        }
    }
}
