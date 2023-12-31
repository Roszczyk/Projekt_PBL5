package com.example.hive

import retrofit.Call
import retrofit.http.GET

// This interface defines an API
// service for getting random jokes.
interface ApiService {

    @GET("data/sensors")
    fun getsensors(): Call<DataModel>

    @GET("data/tempHum")
    fun getTempHum(): Call<DataTempHum>
}
