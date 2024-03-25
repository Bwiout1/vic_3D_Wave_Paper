package com.takeoff.celebs.qwer;

import android.app.Application;
import android.content.Context;

import com.sunrise.sunrise.SRAPP;

public class AppBase extends SRAPP {
    public static Application sApp;

    public void attachBaseContext(Context context){
        super.attachBaseContext(context);
        //todo:保活
//        SunSpot.pre();
    }

    public void onCreate() {
        super.onCreate();
        sApp = this;
    }

}
