package com.sunrise.sunrise.control

import android.os.Build
import android.text.TextUtils
import androidx.core.content.edit
import com.android.installreferrer.api.InstallReferrerClient
import com.android.installreferrer.api.InstallReferrerStateListener
import com.sunrise.sunrise.SSDK
import com.sunrise.sunrise.SRAPP
import com.sunrise.sunrise.support.AesUtil
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Switch
import kotlinx.coroutines.*
import org.json.JSONObject
import java.net.URLDecoder

class TrafficType private constructor() {
    companion object {
        private val TAG = TrafficType::class.java.simpleName

        const val TRAFFIC_UNKNOWN = -1
        const val TRAFFIC_ORGANIC = 1
        const val TRAFFIC_PAID = 0

        val instance: TrafficType by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            TrafficType()
        }
    }


    val _trafficType = "trafficType"
    val _referrer = "referrer"

    fun detectType(callback: (() -> Unit)?){
        if (getType() != TRAFFIC_UNKNOWN){
            callback?.invoke()
            return
        }

        CoroutineScope(Dispatchers.Default).launch {
            val client = InstallReferrerClient.newBuilder(SRAPP.app).build()
            client.startConnection(object : InstallReferrerStateListener {
                override fun onInstallReferrerSetupFinished(responseCode: Int) {
                    if (Switch.LOG_ENABLE){
                        LogUtil.getInstance().d(TAG, "onInstallReferrerSetupFinished, responseCode:$responseCode")
                    }

                    try {
                        if (responseCode == InstallReferrerClient.InstallReferrerResponse.OK) {
                            val referrer = client.installReferrer.installReferrer
                            //todo: test only
//                            val referrer = "utm_source=apps.facebook.com&utm_campaign=fb4a&utm_content=%7B%22app%22%3A0%2C%22t%22%3A1668047650%2C%22source%22%3A%7B%22data%22%3A%229eb6e28f66fc05abdf3ccdb9182a7ac710c7c04387d6e912672cfe9faaf20c72aff4193a4b3e37681113040bcb91bbdbe50cd47d7feac123abded84dce4f25a60e69c71557c8408e0d7f16bf50ffb8876ef4ce332099189284c2e471be057ae5479b99f89f09bf63a65b707569f4a7aa1617e14d9e452630880612eade045f62ba1ccc7f66a6eb0ce2355be7bff22ae72f077d6a921a59b0093d26a6a1b2bb5cabefd41f73647d5abe352cb01c0bc7741209be0e3f619c60df4aa1c2963d6c940610854911fa5f511b7d4f8bf9d36517ed7ad7d0f79d81c56c211010a8ddbf1133fac0c6e1baefe34ec3a175f8ca4ee54184c19abb217e20e8471ddacabc570dc45e0c09a2ee08c04d683691e6bf921aa3edd99c5a19d2a6f8a2e2f9e63bf55d91a1a91eb9f6f2034ff65b41884e614fc2d6e25e11377d634649aba863b817abbb17d4d3df89ee31dc4fd9c3325dea571df86b7ee518f0be456bcfa8d49494319fe2fb1c60272259c2a680abeba21079f3605bbb88635e476284c9d7cf30ff965ceea007e0405902ae28719af8a23f3075d5ec67fb1527c210e5890de306961da0ec4ef9389d69b234f48cdcefd0294958bc0fed4f4e83e07d28c886d64ccacb4349%22%2C%22nonce%22%3A%22dc07a002fdaab4b24e3dde02%22%7D%7D"
                            if (Switch.LOG_ENABLE) {
                                LogUtil.getInstance().d(TAG, "referrer :$referrer")
                            }

                            SSDK.instance.sp.edit {
                                putString(_referrer, referrer)
                                putInt(_trafficType, parseGPReferrer(referrer))
                            }


                            SSDK.instance.analytics.logEvent("binstall",
                                Pair("ref", referrer),
                                Pair("dev", getDevMeta()),
                                Pair("isDebug", Switch.DEB_ENABLE.toString())
                            )

                            callback?.invoke()
                        }
                    } catch (e: Exception) {
                        if(Switch.DEB_ENABLE){
                            e.printStackTrace()
                        }

                        SSDK.instance.analytics.logError(null, "fetchGPReferrer", e)
                    }
                    client.endConnection()
                }

                override fun onInstallReferrerServiceDisconnected() {
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().d(TAG, "onInstallReferrerServiceDisconnected")
                    }
                }
            })
        }
    }


    fun getType(): Int {
        var type = SSDK.instance.sp.getInt(_trafficType, TRAFFIC_UNKNOWN)

        if (Switch.DEB_ENABLE){
            if (TRAFFIC_ORGANIC == type){
                type = TRAFFIC_PAID

                if(Switch.LOG_ENABLE){
                    LogUtil.getInstance().d(TAG, "for debug, change organic user to paid user")
                }
            }
        }

        return type
    }

    fun getDevMeta(): String {
        return String.format("%s|%s|%s", Build.MANUFACTURER, Build.MODEL, Build.VERSION.SDK_INT)
    }

    fun parseGPReferrer(referrer: String?) : Int{
        if (TextUtils.isEmpty(referrer)) {
            return TRAFFIC_ORGANIC
        }

        //check organic
        if(referrer!!.contains("org")
                && referrer.contains("rgani")
                && referrer.contains("anic")){
            return TRAFFIC_ORGANIC
        }

        //check filter
        if(SunConfig.RefConfig.userRefList != null) {
            for (str in SunConfig.RefConfig.userRefList!!) {
                if (referrer.contains(str)) {
                    if(Switch.LOG_ENABLE){
                        LogUtil.getInstance().v(TAG, "check filter success, keyword is: $str")
                    }
                    return TRAFFIC_PAID
                }
            }
        }

        //check mtg promotion
        if(!TextUtils.isEmpty(SunConfig.RefConfig.mtgRefCamp) && referrer.contains(SunConfig.RefConfig.mtgRefCamp)) {//appmetrica_tracking_id
            if(Switch.LOG_ENABLE){
                LogUtil.getInstance().v(TAG, "check mtg success, keyword is: ${SunConfig.RefConfig.mtgRefCamp}")
            }
            return TRAFFIC_PAID
        }

        //check fb promotion
        if (!TextUtils.isEmpty(SunConfig.RefConfig.fbRefKey) && !TextUtils.isEmpty(SunConfig.RefConfig.fbRefCamp)) {
            do {
                if (!referrer.contains("utm_content="))
                    break

                //gp返回的referrer已经执行过第一层URLDecode，referrer以 &k=v 形式存在，那么v中一定不会出现'&'
                val utmContent = referrer.split("utm_content=")[1].let {
                    if (it.contains("&")){
                        URLDecoder.decode(it.split("&")[0], "UTF-8")
                    } else {
                        URLDecoder.decode(it, "UTF-8")
                    }
                }

                try {
                    val jsonObject = JSONObject(utmContent).get("source") as JSONObject
                    val dataHex = jsonObject.getString("data")
                    val nonceHex = jsonObject.getString("nonce")

                    val keyHex = SunConfig.RefConfig.fbRefKey
                    if (keyHex == "")
                        break

                    val clearText = AesUtil.aes_gcmDecrypt(dataHex, keyHex, nonceHex)
                    if (Switch.LOG_ENABLE) {
                        LogUtil.getInstance().v(TAG, "keyHex is :$keyHex")
                        LogUtil.getInstance().v(TAG, "clearText is :$clearText")
                    }

                    if (clearText.contains(SunConfig.RefConfig.fbRefCamp)) {
                        if(Switch.LOG_ENABLE){
                            LogUtil.getInstance().v(TAG, "check fb success, keyword is: ${SunConfig.RefConfig.fbRefCamp}")
                        }
                        return TRAFFIC_PAID
                    }
                }catch (e:Exception){
                    if(Switch.LOG_ENABLE){
                        LogUtil.getInstance().e(TAG, "check fb promotion error")
                    }

                    SSDK.instance.analytics.logError(null, "decryptFbRef", e)
                }

                break
            } while (false)
        }

        return TRAFFIC_ORGANIC
    }
}