package com.game.zxyz;

import android.app.Application;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.SystemClock;
import android.text.TextUtils;
import android.util.Log;

import com.game.crypt.cryptolib.CryptoUtil;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * @author yb
 * @date 2021/8/5
 * @describe 解密并加载dex及原Application
 */

//todo:修改类名与dota.core
public class ROXY extends Application {

    private static final String TAG = ROXY.class.getSimpleName();

    private boolean isBindReal;
    private Application delegate;
    private final String targetApp = com.x.y.StrBox.hostAppClsName;
    private final String sKey = com.x.y.StrBox.hostDexKey;
    private final String dexDirName = com.x.y.StrBox.hostDexDirName;
    private final String extName = com.x.y.StrBox.hostDexExtName;
    private int index = 0;

//    static {
//        try {
//            System.loadLibrary(BuildConfig.lName);
//        } catch (Throwable th) {
//            th.printStackTrace();
//        }
//    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
//        Log.e("xmas", "ROXY::attachBaseContext, time is :"+System.currentTimeMillis());
//        if (Switch.DEB_ENABLE) {
        //用于在解密过程中如果出现异常，可以进行捕获
        CrashHandler.getInstance().init(base);
//        }

        //getMetaData();
        dexDecrypt2();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        try {
//            Log.e("xmas", "ROXY::onCreate, time is :"+System.currentTimeMillis());
            bindRealApplicatin();
        } catch (Exception e) {
            if (Switch.LOG_ENABLE) {
                e.printStackTrace();
            }
        }
    }

    //Dex 解密2 从assets下解密
    private void dexDecrypt2() {
        long startTime = SystemClock.currentThreadTimeMillis();
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "dexDecrypt2");
        }

        //获取当前加密的apk文件
        File apkFile = new File(getApplicationInfo().sourceDir);
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "apkfile:" + apkFile.toString());
        }

        //创建解压目录dexDir
        File versionDir = getDir(extName, Context.MODE_PRIVATE);
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "versionDir:" + versionDir.toString());
        }
        File appDir = new File(versionDir, "app");
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "appDir:" + appDir.toString());
        }
        File dexDir = new File(appDir, "dir");
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "dir:" + dexDir.toString());
        }

        //dex文件list
        List<File> dexFiles = new ArrayList<>();
        if (!dexDir.exists() || dexDir.list().length == 0) {
            dexDir.mkdirs();
            if (Switch.LOG_ENABLE) {
                Log.d("Moon", "decrypt dex");
            }
            String tempDexDir = "assets/" + dexDirName;
            index = 1;
            //解压apk到appDir下
            Zip.getInstance().unZipTargetFile(apkFile, appDir, tempDexDir);
            File assetsDexDir = new File(appDir, "assets/" + dexDirName);
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "assets:" + assetsDexDir.toString());
            }
            //assets/dexDirName目录下
            File[] files = assetsDexDir.listFiles();

            for (File file : files) {
                String name = file.getName();

                //找出所有tmp文件
                if (name.endsWith(String.format(".%s", extName))) {
                    try {
                        File dexFile = new File(dexDir, rename());
                        if (Switch.LOG_ENABLE) {
                            LogUtil.getInstance().d(TAG, file + " decrypt to " + dexFile);
                        }
                        byte[] bytes = ProxyUtils.getInstance().getBytes(file);
                        byte[] decrypt = CryptoUtil.decAndRemovePadding(sKey, bytes);
                        FileOutputStream fos = new FileOutputStream(dexFile);
                        fos.write(decrypt);
                        fos.flush();
                        fos.close();
                        dexFiles.add(dexFile);
                    } catch (Exception e) {
                        if (Switch.LOG_ENABLE) {
                            e.printStackTrace();
                        }
                    }
                }
            }
            //删除assets下原文件
            deleteFile(assetsDexDir.getParentFile());
        } else {
            dexFiles.addAll(Arrays.asList(dexDir.listFiles()));
        }

        try {
            loadDex(dexFiles, versionDir);

            long endTime = SystemClock.currentThreadTimeMillis() - startTime;
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "解密完成! 共耗时：" + endTime + " ms");
            }
        } catch (Exception e) {
            if (Switch.LOG_ENABLE) {
                e.printStackTrace();
            }
        }
    }

    //解压出的dex重命名
    private String rename() {
        return "a" + ++index +".dex";
    }

    private void loadDex(List<File> dexFiles, File versionDir) throws Exception {
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "loadDex");
        }
        //1.先从 ClassLoader 中获取 pathList 的变量
        Field pathListField = ProxyUtils.getInstance().findField(getClassLoader(), "pathList");
        //1.1 得到 DexPathList 类
        Object pathList = pathListField.get(getClassLoader());
        //1.2 从 DexPathList 类中拿到 dexElements 变量
        Field dexElementsField = ProxyUtils.getInstance().findField(pathList, "dexElements");
        //1.3 拿到已加载的 dex 数组
        Object[] dexElements = (Object[]) dexElementsField.get(pathList);

        //2. 反射到初始化 dexElements 的方法，也就是得到加载 dex 到系统的方法
        Method makeDexElements = ProxyUtils.getInstance().findMethod(pathList, "makePathElements", List.class, File.class, List.class);
        //2.1 实例化一个 集合  makePathElements 需要用到
        ArrayList<IOException> suppressedExceptions = new ArrayList<>();
        //2.2 反射执行 makePathElements 函数，把已解码的 dex 加载到系统，不然是打不开 dex 的，会导致 crash
        Object[] addElements = (Object[]) makeDexElements.invoke(pathList, dexFiles, versionDir, suppressedExceptions);

        //3. 实例化一个新数组，用于将当前加载和已加载的 dex 合并成一个新的数组
        Object[] newElements = (Object[]) Array.newInstance(dexElements.getClass().getComponentType(), dexElements.length + addElements.length);
        //3.1 将系统中的已经加载的 dex 放入 newElements 中
        System.arraycopy(dexElements, 0, newElements, 0, dexElements.length);
        //3.2 将解密后已加载的 dex 放入新数组中
        System.arraycopy(addElements, 0, newElements, dexElements.length, addElements.length);

        //4. 将合并的新数组重新设置给 DexPathList的 dexElements
        dexElementsField.set(pathList, newElements);
    }

    private Application getTargetApplication() throws ClassNotFoundException, IllegalAccessException, InstantiationException {
        if (delegate == null) {
            //2. 拿到真实 APK APPlication 的 class
//            Class<?> delegateClass = Class.forName(targetApp);
            Class<?> delegateClass = Class.forName(targetApp, true, getClassLoader());
            //3. 反射实例化，其实 Android 中四大组件都是这样实例化的。
            delegate = (Application) delegateClass.newInstance();
        }
        return delegate;
    }

    private void bindRealApplicatin() throws Exception {
        if (!isBindReal) {

            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "bindRealApplicatin");
            }
            //1. 得到 attachBaseContext(context) 传入的上下文 ContextImpl
            Context baseContext = getBaseContext();
            //2. 拿到真实 APK APPlication 的 class
//            Class<?> delegateClass = Class.forName(targetApp);

            //3. 反射实例化，其实 Android 中四大组件都是这样实例化的。
//            delegate = (Application) delegateClass.newInstance();
            Application delegate = getTargetApplication();
            //3.1 得到 Application attach() 方法 也就是最先初始化的
            Method attach = Application.class.getDeclaredMethod("attach", Context.class);
            attach.setAccessible(true);
            //3.2 将真实的 Application 和假的 Application 进行替换。想当于自己手动控制 真实的 Application 生命周期
            attach.invoke(delegate, baseContext);

            /*ContextImpl---->mOuterContext(app)   通过Application的attachBaseContext回调参数获取*/
            //4. 拿到 Context 的实现类
            Class<?> contextImplClass = Class.forName("android.app.ContextImpl");
            //4.1 获取 mOuterContext Context 属性
            Field mOuterContextField = contextImplClass.getDeclaredField("mOuterContext");
            mOuterContextField.setAccessible(true);
            //4.2 将真实的 Application 交于 Context 中。这个根据源码执行，实例化 Application 下一个就行调用 setOuterContext 函数，所以需要绑定 Context
            mOuterContextField.set(baseContext, delegate);

            /*ActivityThread--->mAllApplications(ArrayList)       ContextImpl的mMainThread属性*/
            //5. 拿到 ActivityThread 变量
            Field mMainThreadField = contextImplClass.getDeclaredField("mMainThread");
            mMainThreadField.setAccessible(true);
            //5.1 拿到 ActivityThread 对象
            Object mMainThread = mMainThreadField.get(baseContext);

            /*ActivityThread--->>mInitialApplication*/
            //6. 反射拿到 ActivityThread class
            Class<?> activityThreadClass = Class.forName("android.app.ActivityThread");
            //6.1 得到当前加载的 Application 类
            Field mInitialApplicationField = activityThreadClass.getDeclaredField("mInitialApplication");
            mInitialApplicationField.setAccessible(true);
            //6.2 将 ActivityThread 中的 Applicaiton 替换为 真实的 Application 可以用于接收相应的声明周期和一些调用等
            mInitialApplicationField.set(mMainThread, delegate);

            /*ActivityThread--->mAllApplications(ArrayList)       ContextImpl的mMainThread属性*/
            //7. 拿到 ActivityThread 中所有的 Application 集合对象，这里是多进程的场景
            Field mAllApplicationsField = activityThreadClass.getDeclaredField("mAllApplications");
            mAllApplicationsField.setAccessible(true);
            ArrayList<Application> mAllApplications = (ArrayList<Application>) mAllApplicationsField.get(mMainThread);
            //7.1 删除 ROXY
            mAllApplications.remove(this);
            //7.2 添加真实的 Application
            mAllApplications.add(delegate);

            /*LoadedApk------->mApplication                      ContextImpl的mPackageInfo属性*/
            //8. 从 ContextImpl 拿到 mPackageInfo 变量
            Field mPackageInfoField = contextImplClass.getDeclaredField("mPackageInfo");
            mPackageInfoField.setAccessible(true);
            //8.1 拿到 LoadedApk 对象
            Object mPackageInfo = mPackageInfoField.get(baseContext);

            //9 反射得到 LoadedApk 对象
            Class<?> loadedApkClass = Class.forName("android.app.LoadedApk");
            Field mApplicationField = loadedApkClass.getDeclaredField("mApplication");
            mApplicationField.setAccessible(true);
            //9.1 将 LoadedApk 中的 Application 替换为 真实的 Application
            mApplicationField.set(mPackageInfo, delegate);

            /*修改ApplicationInfo className   LooadedApk*/
            //10. 拿到 LoadApk 中的 mApplicationInfo 变量
            Field mApplicationInfoField = loadedApkClass.getDeclaredField("mApplicationInfo");
            mApplicationInfoField.setAccessible(true);
            //10.1 根据变量反射得到 ApplicationInfo 对象
            ApplicationInfo mApplicationInfo = (ApplicationInfo) mApplicationInfoField.get(mPackageInfo);
            //10.2 将我们真实的 APPlication ClassName 名称赋值于它
            mApplicationInfo.className = targetApp;

            //11. 执行 代理 Application onCreate 生命周期
            delegate.onCreate();

            //解码完成
            isBindReal = true;
            if (Switch.LOG_ENABLE) {
                LogUtil.getInstance().d(TAG, "解码完成");
            }
        }
    }

    private void deleteFile(File file) {
        if (!file.exists()) {
            return;
        }
        if (!file.isFile() && file.listFiles() != null) {
            for (File child : file.listFiles()) {
                deleteFile(child);
            }
        }
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "delete " + file);
        }
        file.delete();
    }

    @Override
    public String getPackageName() {
        //?让代码走入if中的第三段中
        if (!TextUtils.isEmpty(targetApp)) {
            return "";
        }
        return super.getPackageName();
    }

    /**
     * 这个函数是如果在 AndroidManifest.xml 中定义了 ContentProvider 那么就会执行此处 : installProvider，简介调用该函数
     *
     * @param packageName
     * @param flags
     * @return
     * @throws PackageManager.NameNotFoundException
     */
    @Override
    public Context createPackageContext(String packageName, int flags) throws PackageManager.NameNotFoundException {
//        Log.e("xmas", "ROXY::createPackageContext, time:"+System.currentTimeMillis());
        if (TextUtils.isEmpty(targetApp)) {
            return super.createPackageContext(packageName, flags);
        }
        try {
            bindRealApplicatin();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return delegate;
    }
}
