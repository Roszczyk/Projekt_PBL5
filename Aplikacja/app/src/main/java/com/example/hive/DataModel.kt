package com.example.hive

import android.health.connect.datatypes.units.Temperature
import java.math.BigDecimal
import java.text.DecimalFormat

// Model class for our Jokes
data class DataModel(
    var gps_lat: Double?,
    var gps_lon: Double?,
    var humidity: Double?,
    var temperature: Double?,
    var timestamp: String?,
): java.io.Serializable

