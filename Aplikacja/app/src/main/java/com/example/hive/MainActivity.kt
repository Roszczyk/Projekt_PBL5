package com.example.hive

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Button
import android.widget.ProgressBar
import android.widget.TextView
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

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

            // show the progress bar
            progressBar.visibility = View.VISIBLE


            ApiCall().getsensor(this) { payload ->
                var temp =payload.temperature.toString()
                var hum =payload.humidity.toString()

                //TO DO when new version of payload NO GPS
                ValOpen.text = payload.gps_lon.toString()
                ValLight.text = payload.gps_lat.toString()
                //to this line

                //Setting correct time stamp of update
                val currentTimestamp = System.currentTimeMillis()
                val dateFormat = SimpleDateFormat(" dd MMM yyyy, HH:mm", Locale("en", "PL"))
                ValUpdate.text =  dateFormat.format(Date(currentTimestamp))

                ValTemp.text = "$temp°C"
                ValHum.text = "$hum%"
                progressBar.visibility = View.GONE
            }

//
    }
}
