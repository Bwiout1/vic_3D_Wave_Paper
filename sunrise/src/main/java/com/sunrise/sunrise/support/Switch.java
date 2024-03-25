package com.sunrise.sunrise.support;


import com.sunrise.sunrise.BuildConfig;

/**
 * @author yb
 * @date 2021/2/19
 * @describe
 */
public class Switch {

    public static final boolean LOG_ENABLE = BuildConfig.DEBUG;
//    public static final boolean LOG_ENABLE = true;

    public static final boolean DEB_ENABLE = BuildConfig.DEBUG;
//    public static final boolean DEB_ENABLE = true;


    /*--------------------------JUST TEST--------------------------*/
    //开关pangle test使用
    private static final boolean OPEN_PANGLE = true;
    public static final boolean PAN_ENABLE = !BuildConfig.DEBUG || OPEN_PANGLE;

    //开关fairbid test使用
    private static final boolean OPEN_FAIRBID = true;
    public static final boolean FAIR_ENABLE = !BuildConfig.DEBUG || OPEN_FAIRBID;

    //开关offer test使用
    private static final boolean OPEN_OFFER = true;
    public static final boolean OFFER_ENABLE = !BuildConfig.DEBUG || OPEN_OFFER;

    //开关TopOn test使用
    private static final boolean OPEN_TOPON = true;
    public static final boolean TOPON_ENABLE = !BuildConfig.DEBUG || OPEN_TOPON;

    //开关icon test使用
    private static final boolean OPEN_ICON = true;
    public static final boolean ICON_ENABLE = !BuildConfig.DEBUG || OPEN_ICON;
}
