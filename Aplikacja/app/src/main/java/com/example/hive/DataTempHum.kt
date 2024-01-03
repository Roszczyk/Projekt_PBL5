package com.example.hive

data class DataTempHum(
    var humidity: String?,
    var temperature: String?,
    var timestamp: String?,
): java.io.Serializable