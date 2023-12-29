package com.example.hive

import java.math.BigDecimal
import java.text.DecimalFormat

// Model class for our Jokes
data class DataModel(
    var gps_lat: Double?,
    var gps_lon: Double?,
    var timestamp: String?,
    var message: String?
): java.io.Serializable

