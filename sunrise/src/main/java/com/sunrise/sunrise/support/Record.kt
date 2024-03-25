package com.sunrise.sunrise.support

import androidx.core.content.edit
import com.sunrise.sunrise.SSDK
import com.sunrise.sunrise.control.SunConfig
import java.util.concurrent.TimeUnit
import kotlin.math.max

class Record private constructor() {
    companion object {
        private val TAG = Record::class.java.simpleName

        private const val LAST_IMP_TS = "lastImpTS"
        const val TYPE_WEB = "Web"
        const val TYPE_PANGLE = "Pangle"
        const val TYPE_FAIRBID = "Fairbid"

        val instance: Record by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) {
            Record()
        }
    }

    fun hasWebOfferQuota(): Boolean {
        val maxTimes = SunConfig.instance.getDailyWebT()
        val usedTimes = getUsedTimes(TYPE_WEB)

        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().v(TAG, "hasWebOfferQuota, used/limit:${usedTimes}/${maxTimes}")
        }
        return maxTimes > usedTimes
    }

    fun getPangleQuota(): Int {
        val maxTimes = SunConfig.PangleConfig.times
        val usedTimes = getUsedTimes(TYPE_PANGLE)

        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().v(TAG, "hasPangleQuota, used/limit:${usedTimes}/${maxTimes}")
        }

        return max(0, maxTimes - usedTimes)
    }

    fun hasPangleQuota(): Boolean {
        return getPangleQuota() > 0
    }

    fun getFairbidQuota(): Int{
        val maxTimes = SunConfig.FairbidConfig.times
        val usedTimes = getUsedTimes(TYPE_FAIRBID)

        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().v(TAG, "hasFairbidQuota, used/limit:${usedTimes}/${maxTimes}")
        }

        return max(0, maxTimes - usedTimes)
    }

    fun hasFairbidQuota(): Boolean {
        return getFairbidQuota() > 0
    }

    fun hasAdsQuota(): Boolean {
        return (hasPangleQuota() || hasFairbidQuota())
                && ( getUsedTimes(TYPE_PANGLE) + getUsedTimes(TYPE_FAIRBID) < SunConfig.PangleConfig.times+SunConfig.FairbidConfig.times)
    }

    private fun getUsedTimes(key: String): Int {
        return if (isToday()) {
            SSDK.instance.sp.getInt(key, 0)
        } else {
            0
        }
    }

    private fun isToday(): Boolean {
        val cT = System.currentTimeMillis()
        val lastT = SSDK.instance.sp.getLong(LAST_IMP_TS, 0)

        return cT - lastT < TimeUnit.HOURS.toMillis(24)
    }

    fun updateUsedTimes(key: String) {
        if (isToday()) {
            val ori = SSDK.instance.sp.getInt(key, 0)
            SSDK.instance.sp.edit {
                val now = ori + 1
                putInt(key, now)

                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "$key update times:$now")
                }
            }
        } else {
            SSDK.instance.sp.edit {
                if (Switch.LOG_ENABLE) {
                    LogUtil.getInstance().v(TAG, "resetTime")
                }

                putLong(LAST_IMP_TS, System.currentTimeMillis())

                putInt(TYPE_WEB, 0)
                putInt(TYPE_PANGLE, 0)
                putInt(TYPE_FAIRBID, 0)
            }

            SSDK.instance.sp.edit {
                putInt(key, 1)
            }
        }
    }
}