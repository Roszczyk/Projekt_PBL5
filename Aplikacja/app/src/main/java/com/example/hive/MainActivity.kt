package com.example.hive

import android.os.AsyncTask
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.ProgressBar
import android.widget.RelativeLayout
import android.widget.TextView
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import java.util.TimeZone

class MainActivity : AppCompatActivity() {

    private lateinit var ValTemp: TextView
    private lateinit var ValHum: TextView
    private lateinit var ValOpen: TextView
    private lateinit var ValLight: TextView
    private lateinit var ValUpdate: TextView
    private lateinit var progressBar: ProgressBar

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        ValTemp = findViewById(R.id.temperature)
        ValHum = findViewById(R.id.humidity)
        ValOpen = findViewById(R.id.openstatus)
        ValLight= findViewById(R.id.lightstatus)
        ValUpdate= findViewById(R.id.updated_at)
        progressBar = findViewById(R.id.loader)
        progressBar.visibility = View.VISIBLE


        ApiCall().getsensor(this) {payload ->
            var temp =payload.temperature.toString()
            var hum =payload.humidity.toString()

                //TO DO when new version of payload NO GPS
            ValOpen.text = payload.gps_lon.toString()
            ValLight.text = payload.gps_lat.toString()
                //to this line
            //Setting correct timestamp of update for last update
            val currentTimestamp = System.currentTimeMillis()
            val timeZone = TimeZone.getTimeZone("Europe/Warsaw")
            val dateFormat = SimpleDateFormat("dd MMM yyyy, HH:mm", Locale("pl", "PL"))
            
            dateFormat.timeZone = timeZone
            ValUpdate.text = dateFormat.format(Date(currentTimestamp))
            ValTemp.text = "$temp°C"
            ValHum.text = "$hum%"
            progressBar.visibility = View.GONE

        }


    }
}
