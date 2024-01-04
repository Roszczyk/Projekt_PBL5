package com.example.hive

import java.math.BigDecimal

data class DataTempHum(
    var humidity: Double?,
    var temperature: BigDecimal?,
    var timestamp: String?,
): java.io.Serializable