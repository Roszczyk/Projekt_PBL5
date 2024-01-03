package com.example.hive

data class DataTempHum(
    var humidity: Double?,
    var temperature: Double?,
    var timestamp: String?,
): java.io.Serializable