package com.example.hive

import android.content.Context
import android.provider.ContactsContract.Data
import android.widget.Toast
import retrofit.*

class ApiCall {

    fun getsensor(context: Context, callback: (DataModel) -> Unit) {

        val retrofit: Retrofit = Retrofit.Builder().baseUrl("http://10.0.2.2:5000/sensors").addConverterFactory(
            GsonConverterFactory.create()).build()

        val service: ApiService = retrofit.create<ApiService>(ApiService::class.java)

        val call: Call<DataModel> = service.getsensors()

        call.enqueue(object : Callback<DataModel> {

            override fun onResponse(response: Response<DataModel>?, retrofit: Retrofit?) {
                // This method is called when the API response is received successfully.
                if(response!!.isSuccess){
                    val telemetry: DataModel = response.body() as DataModel
                    callback(telemetry)
                }
            }
            override fun onFailure(t: Throwable?) {
                // This method is called when the API request fails.
                Toast.makeText(context, "Request Fail", Toast.LENGTH_SHORT).show()

            }
        })
    }

}