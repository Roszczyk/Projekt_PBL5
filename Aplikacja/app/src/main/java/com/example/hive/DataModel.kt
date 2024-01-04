package com.example.hive

import android.health.connect.datatypes.units.Temperature
import java.math.BigDecimal
import java.text.DecimalFormat

// Model class for our Jokes
data class DataModel(
    var activity: Boolean,
    var digital_in: Boolean?,
    var gps_lat: Double,
    var gps_lon: Double,
    var heating: Boolean?,
    var humidity: Double?,
    var lights: Boolean,
    var noise: Boolean,
    var temperature: Double?,
): java.io.Serializable

