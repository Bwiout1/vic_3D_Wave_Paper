package com.sunrise.sunrise.control

import android.text.TextUtils
import androidx.core.content.edit
import com.game.zxyz.Efghijk
import com.flurry.android.FlurryConfig
import com.flurry.android.FlurryConfigListener
import com.sunrise.sunrise.SSDK
import com.sunrise.sunrise.SRAPP
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Switch
import org.json.JSONArray
import org.json.JSONException
import org.json.JSONObject

class SunConfig private constructor() {

    companion object {
        private val TAG = SunConfig::class.java.simpleName

        private const val CONFIG_KEY = "_flurry"

        val instance: SunConfig by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            SunConfig()
        }
    }

    private var configJson:JSONObject? = null

    private val _hasLocalCfg = "has-local-cfg"
    fun hasLocalConfig(): Boolean {
        return SSDK.instance.sp.getBoolean(_hasLocalCfg, false)
    }


    fun pull(callback: (() -> Unit)?) {
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "fetching flurry config...")
        }

        //为确保每次成功获取到配置时，callback都能成功调用，每次都注册不同的listener，在获取成功获取/失败时，需要FlurryConfig.getInstance().unregisterListener
        //test cases:
        //1.app安装， 首次启动， 网络正常，成功获取配置，检查各个回调接口的调用顺序:  onFetchSuccess()->onActivateComplete()
        //2.app安装， 本地已有配置，第二次/三次等启动，网络正常，成功获取配置，检查各个回调接口的调用顺序:  onActivateComplete()
        //3.app安装， 首次启动， 网络异常，不能从远程拉取配置：onFetchError(true) ->...->onFetchError(false)
        //4.app安装，首次启动，先无网络，在超时之前/之后，网络正常，检查各个回调接口的调用顺序:  onFetchError()->onFetchSuccess()->onActivateComplete()
        //5.本地已有配置,网络异常，不能从远程拉取配置，检查各个回调接口的调用顺序:  onActivateComplete()->onFetchNoChange()
        FlurryConfig.getInstance().registerListener(object : FlurryConfigListener {
            override fun onFetchSuccess() {

                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "onFetchSuccess")
                }

                FlurryConfig.getInstance().activateConfig()
            }

            override fun onFetchNoChange() {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "onFetchNoChange")
                }
            }

            override fun onFetchError(isRetrying: Boolean) {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().e(TAG, "onFetchError， isRetrying:$isRetrying")
                }

                if(!isRetrying){
                    FlurryConfig.getInstance().unregisterListener(this)
                }
            }

            override fun onActivateComplete(isCache: Boolean) {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "onActivateComplete， isCache:$isCache")
                }
                FlurryConfig.getInstance().unregisterListener(this)

                decryptConfig()

                parseRef()
                parsePangle()
                parseFairbid()
                parseMtg()
                parseOther()

                SSDK.instance.sp.edit {
                    putBoolean(_hasLocalCfg, true)
                }

                callback?.invoke()
            }

        }, SRAPP.handler)


        FlurryConfig.getInstance().fetchConfig()
    }

    private fun decryptConfig(){
        val cipher = FlurryConfig.getInstance().getString(CONFIG_KEY, "").trim()
//        val cipher = "a472bdbd6802b48bc3dcbc5e1edda820cf7732257e8cfa967deda84efc6392726bf1cec6ad58d699b57598a4fa1b5d6b9ad5d8a54f965c1e710a55dec967add8b46fdccfab4a84656428c33792f5c6ce97d1b92a777a5b1284586f19fc55169d3506f1328bec0d78a89e9488f54dae28a7f9f2cdfaa7fcf74d675504a1cd0555a1382edfba2cd054fcbb399a2c5221ac50d457045870f7797e90561806130ad32f0c48c76e74a8c69ca786ccdc6a958f0574917c2ce19ee80c736e997773c41eb7838a7d3fa10ac4237cbd8b4b84d54b944b349fcd8a718824a66fa9afc5e0636530bfa5592af6f2c11394ac0eb8d28faf9a322ad573b002b85df66f6d6ef61c620f7dcd952b6c9ae4ad3481f647c2217bf34e585793a2f0ac0dd2b976dc7be2d1c123ec478fba99fcebfff8a948d86ead79f090733ace302c1ef3f15ccc422e950220de7c89350063e7c2e479621637b3713d0682bd7fa20df2d945a7882ef82c6ebb3fb8194ef0b7926da79856c7e53a2b8169f07897abb6a1ab4cc5730c7cef75181db974ca7e624806ca491aebcdbda9651f0e13a00618ddb4d22ad596094c6df4098c81dcb295b9cbe355a13f968f0a2f63da483424a9e32a7b36788cc2397b2305a19171da1a474f90b567f029d23eb8b264b778d1fc5539394be150195a1b69228837194f9be865c31c5ee845c95a2cbbd62a0a5e7b054e2ba1df09d27c66786c2aceb571f71176d3bbf1ee98086076dedcd29dc873c2e8e4130d2ce24dee6a01287401b65b5587081e2db746f3fc517231ad3725412f16f04fac2a8af5a751ea5a5686b32272c385e5a94967b92e16c190ebdfee04c519593c27bb8fd95cfac8643fa693a8bb5298b64b472b3f8b183fe6fcdde2a40f489d0aef6af55da1c56fcb79a5ce13c2d7f03f92a89e6589e3f87149fcd5f08c64b23e15697e0dfffd7a47e72504eead7a1c8d465b42731d0aaede1e4fd6c59faa0c29e5a2f1544da67c1a6f25522ed08debbeadc94b26fbdcd8fc7b2efe449135a5ead0abc30be98640b485c182b8afc6f3e0b7426edc66be84975952a9fcbd74c2f89192bc562d693bc159e963ceb85e33355792df9bc062336f1fb902"
        if(TextUtils.isEmpty(cipher))
            return

        try{
            val data = String(
                Efghijk.i(
                    SRAPP.app.packageName,
                    Efghijk.h(cipher)
                )
            )

            if(Switch.LOG_ENABLE){
                LogUtil.getInstance().v(TAG, "clear flurry data:$data")
            }

            if(!TextUtils.isEmpty(data)){
                configJson = JSONObject(data)
            }
        }catch(e:Exception){
            SSDK.instance.analytics.logError(null, "decryptConfigErr", e)

            if(Switch.LOG_ENABLE){
                e.printStackTrace()
            }
        }
    }

    private fun parseRef(){
        var rawConfig = configJson?.optString("fb_ref_key", "") ?: ""
        if (!TextUtils.isEmpty(rawConfig)) {
            RefConfig.fbRefKey = rawConfig
        }else{
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().e(TAG, "parseRef error, refKey is null")
            }
        }

        rawConfig = configJson?.optString("fb_ref_camp", "") ?: ""
        if(!TextUtils.isEmpty(rawConfig)){
            RefConfig.fbRefCamp = rawConfig
        }else{
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().e(TAG, "parseRef error, refCampaignStr is null")
            }
        }

        rawConfig = configJson?.optString("mtg_ref_camp", "") ?: ""
        if(!TextUtils.isEmpty(rawConfig)){
            RefConfig.mtgRefCamp = rawConfig
        }else{
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().e(TAG, "parseRef error, mtgRefCamp is null")
            }
        }

        rawConfig = configJson?.optString("user_ref_list") ?: ""
        rawConfig.replace(" ", "").let {
            if(!TextUtils.isEmpty(it)){
                RefConfig.userRefList = if(it.contains(",")){
                    it.split(",")
                }else{
                    listOf(it)
                }
            }else{
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "parseRef, userRefList is null")
                }
            }
        }

        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().v(TAG, "parseRef")
        }
    }

    private fun parsePangle() {
        val rawConfig = configJson?.optString("ads_fallback_pangle", "") ?: ""
        if (!TextUtils.isEmpty(rawConfig)) {
            try {
                val jsonObject = JSONObject(rawConfig)
                PangleConfig.times = jsonObject.optInt("times", 0)
                PangleConfig.appId = jsonObject.optString("app", "").trim()
                PangleConfig.nativeId = jsonObject.optString("native", "").trim()
                PangleConfig.intersId = jsonObject.optString("int", "").trim()

            } catch (e: Exception) {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().e(TAG, e.toString())
                }
            }
        }
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "parsePangle")
        }
    }

    private fun parseFairbid() {
        val rawConfig = configJson?.optString("ads_fallback_fairbid", "") ?: ""
        if (!TextUtils.isEmpty(rawConfig)) {
            try {
                val jsonObject = JSONObject(rawConfig)
                FairbidConfig.times = jsonObject.optInt("times", 0)
                FairbidConfig.appId = jsonObject.optString("fairbid_appid", "").trim()
                FairbidConfig.intersId = jsonObject.optString("fairbid_intid", "").trim()

            } catch (e: JSONException) {
                if (Switch.LOG_ENABLE) {
                    e.printStackTrace()
                }
            }
        }
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "parseFairbid")
        }
    }

    private fun parseMtg() {
        val rawConfig = configJson?.optString("ads_fallback_fairbid", "") ?: ""
        if (!TextUtils.isEmpty(rawConfig)) {
            try {
                val jsonObject = JSONObject(rawConfig)

                MtgConfig.nativePlc = jsonObject.optString("mtg_naplc", "").trim()
                MtgConfig.nativeBannerUnitId = jsonObject.optString("mtg_naunit", "").trim()
                MtgConfig.nativeMediumUnitId = jsonObject.optString("mtg_naunit2", "").trim()

            } catch (e: JSONException) {
                if (Switch.LOG_ENABLE) {
                    e.printStackTrace()
                }
            }
        }
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "parseMtg")
        }
    }

    private fun parseOther(){
        OtherConfig.metricaEnable = configJson?.optBoolean("metrica_enable", false) ?: false
    }

    fun getEnable(): Boolean {
        val config = configJson?.optString("ads_enable", "false")
        return config?.lowercase()?.contains("t") ?: false
    }

    fun getNewUserActivateTimeSeconds(): Long {
        return configJson?.optLong("berg_newUserAdDelayT", 7200L) ?: 7200L
    }

    fun getDailyWebT(): Int {
        return configJson?.optInt("berg_web_times", 0) ?: 0
    }

    fun getWebOfferUrl(): String {
        val urls = configJson?.optString("berg_web_url", "") ?: ""
        if (!TextUtils.isEmpty(urls)) {
            try {
                val jsonArray = JSONArray(urls)
                val size = jsonArray.length()
                if (size > 0) {
                    val random = (0 until size).random()
                    return jsonArray.optString(random)
                }
            } catch (e: Exception) {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().e(TAG, e.toString())
                }
            }
        }
        return ""
    }

    object PangleConfig {
        var times = 0
        var appId = ""
        var nativeId = ""
        var intersId = ""
    }

    object FairbidConfig {
        var times = 0
        var appId = ""
        var intersId = ""
    }

    object MtgConfig {
        var nativePlc = ""
        var nativeBannerUnitId = ""
        var nativeMediumUnitId = ""
    }

    object RefConfig{
        var fbRefKey = ""
        var fbRefCamp = ""
        var mtgRefCamp = ""
        var userRefList:List<String>? = null
        // aaaeeedd,gggiiisg,kksslll,aakkLLL,ddiiggaa,iimmddaa
    }

    object OtherConfig{
        var metricaEnable = false
    }
}

/*
{
    "ads_enable": "true",
    "ads_fallback_pangle": "{\"times\":\"5\",\"app\":\"8025677\", \"native\":\"980088216\", \"int\":\"980088188\"}",
    "ads_fallback_other": "{\"times\":\"5\",\"mtg_appid\":\"173142\",\"mtg_appkey\":\"1e3d468be31c0a4e9e78df36144224f4\",\"mtg_intplc\":\"511549\",\"mtg_intunit\":\"1912136\",\"mtg_naplc\":\"606337\",\"mtg_naunit\":\"2087091\",\"mtg_naunit2\":\"2098342\",\"fairbid_appid\":\"133676\",\"fairbid_intid\":\"693035\"}",
    "berg_newUserAdDelayT": "10",
    "berg_web_times": "5",
    "berg_web_url": "[\"https://lightningscannerr.xyz/\"]",
    "fb_ref_key": "5ecaef04adb1095e42625265e8a726f46bf16c5a41bb8a2450dae033d13b5c7c",
    "fb_ref_camp": "campaign_name",
    "mtg_ref_camp": "244173993936965391",
    "user_ref_list": "",
    "metrica_enable": "false"
}
 */