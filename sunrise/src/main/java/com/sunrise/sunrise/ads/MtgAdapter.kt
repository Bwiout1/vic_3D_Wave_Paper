package com.sunrise.sunrise.ads

import android.app.Activity
import android.content.res.Resources
import android.text.TextUtils
import android.view.ViewGroup
import com.mbridge.msdk.MBridgeConstans
import com.mbridge.msdk.MBridgeSDK
import com.mbridge.msdk.out.*
import com.sunrise.sunrise.SRAPP
import com.sunrise.sunrise.control.SunConfig
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.lang.ref.WeakReference
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicReference


class MtgAdapter private constructor() {
    companion object {
        val instance: MtgAdapter by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            MtgAdapter()
        }

        private val TAG = MtgAdapter::class.java.simpleName
    }

    var mtgInited = false
    private val nativeBannerLoading = AtomicBoolean(false)
    private val nativeMediumLoading = AtomicBoolean(false)
    private val handlerSmallRef = AtomicReference<MBNativeAdvancedHandler?>()
    private val handlerBigRef = AtomicReference<MBNativeAdvancedHandler?>()
    private val actAdsMap = HashMap<Activity, MBNativeAdvancedHandler>()
    private val width: Int = Resources.getSystem().displayMetrics.widthPixels
    private val bannerHeight: Int = (SRAPP.app.resources.displayMetrics.density*80).toInt()
    private val mediumHeight: Int = (SRAPP.app.resources.displayMetrics.density*250).toInt()

    private fun sdkIsCompleted(): Boolean {
        if (!mtgInited)
            return false

        return if (MBridgeSDK.PLUGIN_LOAD_STATUS.COMPLETED == MBridgeSDKFactory.getMBridgeSDK().status) {
            true
        } else {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "mtgSDK is not completed")
            }
            false
        }
    }

    fun hasNative(medium: Boolean = false): Boolean {
        val handler: MBNativeAdvancedHandler? =
            if (medium) handlerBigRef.get() else handlerSmallRef.get()

        return (handler != null) && handler.isReady
    }

    fun isLoading(medium: Boolean = false): Boolean {
        return if (medium) nativeMediumLoading.get() else nativeBannerLoading.get()
    }

    fun loadNative(medium: Boolean = false, attempt: Int = 1) {
        if (!sdkIsCompleted()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "loadNative, MTG sdk isn't initialized!")
            }
            return
        }

        if (hasNative(medium)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().v(TAG, "loadNative, has native ads cached for size:${if (medium) "medium" else "banner"}")
            }

            return
        }

        val plc = SunConfig.MtgConfig.nativePlc
        val unitId =
            if (medium) SunConfig.MtgConfig.nativeMediumUnitId else SunConfig.MtgConfig.nativeBannerUnitId
        if (TextUtils.isEmpty(plc) || TextUtils.isEmpty(unitId)) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "loadNative, id is empty, plc=${plc}, unitId=${unitId}")
            }

            return
        }


        val loadingFlag: AtomicBoolean = if (medium) {
            nativeMediumLoading
        } else {
            nativeBannerLoading
        }

        if (loadingFlag.compareAndSet(false, true)) {
            val handler = MBNativeAdvancedHandler(null, plc, unitId)
            handler.setCloseButtonState(MBMultiStateEnum.negative)
            handler.setPlayMuteState(MBridgeConstans.REWARD_VIDEO_PLAY_MUTE)
            handler.autoLoopPlay(AutoPlayMode.PLAY_WHEN_NETWORK_IS_AVAILABLE)
            handler.setAdListener(object : NativeAdvancedAdListener {
                override fun onLoadFailed(p0: MBridgeIds?, msg: String?) {
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().e(TAG, "loadNative, onLoadFailed, size=${if (medium) "medium" else "banner"}, msg=${msg}")
                    }
                    loadingFlag.set(false)

                    if (attempt < 2) {//允许10s后再重试一次
                        CoroutineScope(Dispatchers.Main).launch {
                            delay(10 * 1000)//10 seconds
                            loadNative(medium, attempt + 1)
                        }
                    }
                }

                override fun onLoadSuccessed(p0: MBridgeIds?) {
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().v(TAG, "loadNative, onLoadSuccessed, size=${if (medium) "medium" else "banner"}")
                    }
                    loadingFlag.set(false)
                }

                //followings are unnecessary for loading
                override fun onLogImpression(p0: MBridgeIds?) {}

                override fun onClick(p0: MBridgeIds?) {}

                override fun onLeaveApp(p0: MBridgeIds?) {}

                override fun showFullScreen(p0: MBridgeIds?) {}

                override fun closeFullScreen(p0: MBridgeIds?) {}

                override fun onClose(p0: MBridgeIds?) {}
            })


            if (medium) {
                handler.setNativeViewSize(width, mediumHeight)
            } else {
                handler.setNativeViewSize(width, bannerHeight)
            }

            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().v(TAG, "loadNative, start to load native, plc=${plc}, unitId=${unitId}, size=${if (medium) "medium" else "banner"}")
            }
            handler.load()

            val handlerRef = if (medium) {
                handlerBigRef
            } else {
                handlerSmallRef
            }
            handlerRef.getAndSet(handler)?.release()
        }
    }

    fun renderNative(
        activity: Activity,
        container: ViewGroup,
        medium: Boolean,
        adShowCallback: (() -> Unit)? = null,
        adEndCallback: (() -> Unit)? = null
    ) {
        if (!sdkIsCompleted()) {
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().w(TAG, "renderNative, MTG sdk isn't initialized!")
            }

            return
        }


        val handler = if (medium) {
            handlerBigRef.getAndSet(null)
        } else {
            handlerSmallRef.getAndSet(null)
        }

        do {
            if (handler==null)
                break

            if (!handler.isReady){
                handler.release()
                break
            }
            actAdsMap[activity] = handler

            handler.setAdListener(object : NativeAdvancedAdListener {
                //these two interfaces are unnecessary for impression
                override fun onLoadFailed(p0: MBridgeIds?, p1: String?) {}
                override fun onLoadSuccessed(p0: MBridgeIds?) {}


                override fun onLogImpression(p0: MBridgeIds?) {
                    if(Switch.LOG_ENABLE) {
                        LogUtil.getInstance().v(TAG, "renderNative, onLogImpression")
                    }
//                    adShowCallback?.invoke()
                }

                override fun onClick(p0: MBridgeIds?) {
                    if(Switch.LOG_ENABLE) {
                        LogUtil.getInstance().v(TAG, "renderNative, onClick")
                    }
                }

                override fun onLeaveApp(p0: MBridgeIds?) {}

                override fun showFullScreen(p0: MBridgeIds?) {}

                override fun closeFullScreen(p0: MBridgeIds?) {}

                override fun onClose(p0: MBridgeIds?) {
                    if(Switch.LOG_ENABLE) {
                        LogUtil.getInstance().v(TAG, "renderNative, onClose")
                    }

                    adEndCallback?.invoke()
                }
            })

            val adNativeView = handler.adViewGroup
            if (adNativeView != null && adNativeView.parent == null){
                // When you construct the MBNativeAdvancedHandler object and the incoming activity is not the activity of the current show, please call before displaying the ad
                MBridgeSDKFactory.getMBridgeSDK()
                    .updateDialogWeakActivity(WeakReference(activity))

                container.addView(adNativeView)
                handler.onResume()
                container.postInvalidate()
                adShowCallback?.invoke()
            }

        } while (false)


        //广告被使用后，加载下一次广告，至于加载的判断逻辑(是否加载/加载哪些平台)由上层逻辑判断
        AdsMaster.instance.loadNative(medium)
    }





//lifecycle
//    每个MBNativeAdvancedHandler对应一个广告，当该广告展示后，它只与附着的activity相关
    fun nativeOnResume(activity: Activity) {
        actAdsMap[activity]?.onResume()
    }

    fun nativeOnPause(activity: Activity) {
        actAdsMap[activity]?.onPause()
    }

    fun nativeOnDestroy(activity: Activity) {
        actAdsMap.remove(activity)?.release()
    }
}