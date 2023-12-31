package com.example.hive

import android.content.Intent
import android.os.AsyncTask
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.LinearLayout
import android.widget.ProgressBar
import android.widget.RelativeLayout
import android.widget.TextView
import com.android.volley.toolbox.JsonObjectRequest
import org.json.JSONObject
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import java.util.TimeZone
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.Volley
import android.widget.Toast

class MainActivity : AppCompatActivity() {

    private lateinit var ValTemp: TextView
    private lateinit var ValHum: TextView
    private lateinit var ValOpen: TextView
    private lateinit var ValLight: TextView
    private lateinit var ValHeat:TextView
    private lateinit var ValUpdate: TextView
    private lateinit var BtnMap: LinearLayout
    private lateinit var BtnLight: LinearLayout
    private lateinit var progressBar: ProgressBar

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        ValTemp = findViewById(R.id.temperature)
        ValHum = findViewById(R.id.humidity)
        ValOpen = findViewById(R.id.openstatus)
        ValLight = findViewById(R.id.lightstatus)
        ValHeat = findViewById(R.id.heatingstatus)
        ValUpdate = findViewById(R.id.updated_at)
        progressBar = findViewById(R.id.loader)
        BtnMap = findViewById(R.id.boxMap)
        BtnLight = findViewById(R.id.boxlight)
        progressBar.visibility = View.VISIBLE

        val baseUrl = "http://10.0.2.2:5000/data"


        ApiCall().getsensor(this) { payload ->
            var temp = payload.temperature.toString()
            var hum = payload.humidity.toString()
            var cover = payload.digital_in.toString()
            var heat = payload.heating.toString()
            var lig = payload.lights.toString()
            //TO DO when new version of payload NO GPS
            if (cover == "true")
                ValOpen.text = "open"
            else
                ValOpen.text = "closed"

            if (heat == "true")
                ValHeat.text = "on"
            else
                ValHeat.text = "off"

            if (lig == "true")
                ValLight.text = "on"
            else
                ValLight.text = "off"


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

        BtnMap.setOnClickListener() {
            val intent = Intent(this, MapActivity::class.java)
            startActivity(intent)
        }
        BtnLight.setOnClickListener() {
            val jsonParams = JSONObject()
            jsonParams.put("heating", true)
            ValTemp.text = "ssssp°C"

            val request = JsonObjectRequest(
                Request.Method.POST,"http://10.0.2.2:5000/data/heating?value=true", jsonParams,
                { response ->
                    println("DODANO TRENERA")
                    ValTemp.text = "git°C"

                },
                { error ->
                    // Obsługa błędu
                    ValTemp.text = "error°C"
                    Log.e("MainActivity", "Error during API call: ${error.localizedMessage}")



                }
            )
            Volley.newRequestQueue(this).add(request)
        }

    }

}