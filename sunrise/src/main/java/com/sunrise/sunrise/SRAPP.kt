package com.sunrise.sunrise

import android.app.Application
import android.os.Handler
import android.os.Looper
import com.android.resguard.AppExtResManager

//todo:修改类名与上两级路径(sunrise.sunrise)

open class SRAPP :Application(){
    override fun onCreate() {
        if (AppExtResManager.hasSplitRes()) {
            AppExtResManager.init(this)
            AppExtResManager.getInstance().apply {
                decryptRes()
                loadRes()
            }
        }
        super.onCreate()
        app = this

        SSDK.instance.init()
    }

    companion object {
        val handler = Handler(Looper.getMainLooper())
        lateinit var app:Application
    }
}