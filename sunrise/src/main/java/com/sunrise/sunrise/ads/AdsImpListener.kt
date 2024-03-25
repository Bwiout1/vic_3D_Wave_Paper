package com.sunrise.sunrise.ads

interface AdsImpListener {
    fun onShow(network: String)
    fun onShowFail(network: String)
    fun onDismiss(network: String)
}