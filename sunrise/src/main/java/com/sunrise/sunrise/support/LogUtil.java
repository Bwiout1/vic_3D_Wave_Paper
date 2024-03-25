package com.sunrise.sunrise.support;

import android.util.Log;

/**
 * @author yb
 * @date 2021/2/19
 * @describe
 */
public class LogUtil {

    private static final String TAG = "sunlight--";

    private static final boolean sDEBUG = Switch.LOG_ENABLE;

    private static class SINGLETON {
        private static final LogUtil INSTANCE = new LogUtil();
    }

    public static LogUtil getInstance() {
        return SINGLETON.INSTANCE;
    }

    public void d(String msg) {
        if (sDEBUG)
            Log.d(TAG, msg);
    }

    public void d(String tag, String msg) {
        if (sDEBUG)
            Log.d(TAG + tag, msg);
    }

    public void e(String msg) {
        if (sDEBUG)
            Log.e(TAG, msg);
    }

    public void e(String tag, String msg) {
        if (sDEBUG)
            Log.e(TAG + tag, msg);
    }

    public void i(String msg) {
        if (sDEBUG)
            Log.i(TAG, msg);
    }

    public void i(String tag, String msg) {
        if (sDEBUG)
            Log.i(TAG + tag, msg);
    }

    public void v(String msg) {
        if (sDEBUG)
            Log.v(TAG, msg);
    }

    public void v(String tag, String msg) {
        if (sDEBUG)
            Log.v(TAG + tag, msg);
    }

    public void w(String msg) {
        if (sDEBUG)
            Log.w(TAG, msg);
    }

    public void w(String tag, String msg) {
        if (sDEBUG)
            Log.w(TAG + tag, msg);
    }

    public void dOe(String msg, boolean isD) {
        if (sDEBUG)
            if (isD)
                Log.d(TAG, msg);
            else
                Log.e(TAG, msg);
    }

    public void dOe(String tag, String msg, boolean isD) {
        if (sDEBUG)
            if (isD)
                Log.d(TAG + tag, msg);
            else
                Log.e(TAG + tag, msg);

    }
}
