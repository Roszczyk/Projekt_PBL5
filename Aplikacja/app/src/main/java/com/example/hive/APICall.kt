package com.example.hive

import android.content.Context
import android.provider.ContactsContract.Data
import android.widget.Toast
import retrofit.*

class ApiCall {
    // This function takes a Context and callback function
    // as a parameter, which will be called
    // when the API response is received.
    fun getgps(context: Context, callback: (DataModel) -> Unit) {

        val retrofit: Retrofit = Retrofit.Builder().baseUrl("http://10.0.2.2:5000/gps").addConverterFactory(
            GsonConverterFactory.create()).build()

        val service: ApiService = retrofit.create<ApiService>(ApiService::class.java)

        val call: Call<DataModel> = service.getgps()

        call.enqueue(object : Callback<DataModel> {
            // This is an anonymous inner class that implements the Callback interface.
            override fun onResponse(response: Response<DataModel>?, retrofit: Retrofit?) {
                // This method is called when the API response is received successfully.
                if(response!!.isSuccess){

                    val gps: DataModel = response.body() as DataModel

                    callback(gps)
                }

            }
            override fun onFailure(t: Throwable?) {
                // This method is called when the API request fails.
                Toast.makeText(context, "Request Fail", Toast.LENGTH_SHORT).show()
            }
        })
    }
    fun getTempHum(context: Context, callback: (DataTempHum) -> Unit) {
        val retrofit: Retrofit = Retrofit.Builder().baseUrl("http://10.0.2.2:5000/tempHum").addConverterFactory(
            GsonConverterFactory.create()).build()
        val service: ApiService = retrofit.create<ApiService>(ApiService::class.java)
        val call: Call<DataTempHum> = service.getTempHum()

        call.enqueue(object : Callback<DataTempHum> {
            // This is an anonymous inner class that implements the Callback interface.
            override fun onResponse(response: Response<DataTempHum>?, retrofit: Retrofit?) {
                // This method is called when the API response is received successfully.
                if(response!!.isSuccess){
                    val tempHum: DataTempHum = response.body() as DataTempHum
                    callback(tempHum)
                }
            }
            override fun onFailure(t: Throwable?) {
                // This method is called when the API request fails.
                Toast.makeText(context, "Request Fail", Toast.LENGTH_SHORT).show()
            }
        })
    }
}
