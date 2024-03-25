package com.sunrise.sunrise.control

import android.content.Context
import androidx.work.Worker
import androidx.work.WorkerParameters
import com.sunrise.sunrise.support.LogUtil
import com.sunrise.sunrise.support.Switch

class FlurryWorker(appContext: Context, workerParameters: WorkerParameters) :
    Worker(appContext, workerParameters) {

    companion object {
        private val TAG = FlurryWorker::class.java.simpleName
    }

    override fun doWork(): Result {
        if (Switch.LOG_ENABLE) {
            LogUtil.getInstance().d(TAG, "check flurry")
        }

        Work.instance.doConfigWork()

        return Result.success()
    }
}