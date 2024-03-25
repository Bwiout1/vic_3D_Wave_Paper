package com.sunrise.sunrise.support;

import android.app.ActivityManager;
import android.app.Application;
import android.content.Context;
import android.os.Build;
import android.text.TextUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.lang.reflect.Method;
import java.util.List;

public class ProcUtil {
    public static boolean isMainProcess(Context context) {
        String pkgName = context.getPackageName();
        if (TextUtils.isEmpty(pkgName))
            return false;

        return pkgName.equals(getCurProcessName(context));
    }

    public static String getCurProcessName(Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            String processName = Application.getProcessName();
            if (!TextUtils.isEmpty(processName)) {
                return processName;
            }
        }

        try {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(new File("/proc/" + android.os.Process.myPid() + "/cmdline")));
            String trim = bufferedReader.readLine().trim();
            bufferedReader.close();
            if (!TextUtils.isEmpty(trim)) {
                return trim;
            }
        } catch (Exception e) {
//            LogFacade.w(LOG_TAG, e);
        }

        int myPid = android.os.Process.myPid();
        List<ActivityManager.RunningAppProcessInfo> runningAppProcesses = ((ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE)).getRunningAppProcesses();
        if (runningAppProcesses == null) {
            return getCurrentProcessNameByActivityThread();
        }

        for (ActivityManager.RunningAppProcessInfo runningAppProcessInfo : runningAppProcesses) {
            if (runningAppProcessInfo.pid == myPid) {
                return runningAppProcessInfo.processName;
            }
        }
        return null;


    }

    public static String getCurrentProcessNameByActivityThread() {
        try {
            Method declaredMethod = Class.forName("android.app.ActivityThread", false, Application.class.getClassLoader()).getDeclaredMethod("currentProcessName", new Class[0]);
            declaredMethod.setAccessible(true);
            Object invoke = declaredMethod.invoke(null, new Object[0]);
            if (invoke instanceof String) {
                return (String) invoke;
            }
            return null;
        } catch (Throwable unused) {
//            LogFacade.w(LOG_TAG, unused);
            return null;
        }
    }
}
