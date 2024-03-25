package com.game.zxyz;

import android.os.SystemClock;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

/**
 * @author yb
 * @date 2021/8/6
 * @describe zip
 */
public class Zip {

    private static final String TAG = Zip.class.getSimpleName();

    private static class SINGLETON {
        private static final Zip INSTANCE = new Zip();
    }

    public static Zip getInstance() {
        return SINGLETON.INSTANCE;
    }


    public void unZipTargetFile(File zip, File dir, String targetDir) {
        long startTime = SystemClock.currentThreadTimeMillis();
        if (!dir.exists()) {
            dir.mkdirs();
        } else {
            for (File f : dir.listFiles()) {
                if (f.getName().contains(targetDir)) {
                    //目标目录已解压
                    return;
                }
            }
        }
        try {
            ZipFile zipFile = new ZipFile(zip);
            ZipInputStream zipInputStream = new ZipInputStream(new FileInputStream(zip));
            byte[] buffer = new byte[1024 * 1024];
            Enumeration<? extends ZipEntry> entries = zipFile.entries();
            long st = SystemClock.currentThreadTimeMillis();
            while (entries.hasMoreElements()) {
                ZipEntry zipEntry = entries.nextElement();
                String fileName = zipEntry.getName();
                //指定目录下文件
                if (fileName != null && fileName.contains(targetDir)) {
                    //非目录
                    if (!zipEntry.isDirectory()) {
                        long st1 = SystemClock.currentThreadTimeMillis();
                        File file = new File(dir, fileName);
                        //创建目录
                        if (file.getParentFile() != null && !file.getParentFile().exists()) {
                            file.getParentFile().mkdirs();
                        }
                        //写文件
                        FileOutputStream fos = new FileOutputStream(file);
                        InputStream is = zipFile.getInputStream(zipEntry);
//                        byte[] buffer = new byte[2048];
                        int len;
                        while ((len = is.read(buffer)) != -1) {
                            fos.write(buffer, 0, len);
                        }
                        fos.close();
                        is.close();
                        if (Switch.LOG_ENABLE) {
                            long et1 = SystemClock.currentThreadTimeMillis() - st1;
                            LogUtil.getInstance().i(TAG, "write " + fileName + ": " + et1 + " ms");
                        }
                    }
                }
            }
            if (Switch.LOG_ENABLE) {
                long endTime = SystemClock.currentThreadTimeMillis() - st;
                LogUtil.getInstance().i(TAG, "write total time: " + endTime + " ms");
            }
            zipInputStream.close();
            if (Switch.LOG_ENABLE) {
                long endTime = SystemClock.currentThreadTimeMillis() - startTime;
                LogUtil.getInstance().i(TAG, "total time: " + endTime + " ms");
            }
        } catch (IOException e) {
            if (Switch.LOG_ENABLE) {
                e.printStackTrace();
            }
        }
    }

}
