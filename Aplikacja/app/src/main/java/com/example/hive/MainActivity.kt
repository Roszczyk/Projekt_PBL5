package com.example.hive

import android.content.Intent
import android.os.AsyncTask
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.ImageView
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
    private lateinit var ValNoise:TextView
    private lateinit var ValAtivity: TextView
    private lateinit var BtnMap: LinearLayout
    private lateinit var BtnLight: LinearLayout
    private lateinit var BtnHeat: LinearLayout
    private lateinit var BtnHum: LinearLayout
    private lateinit var BtnTemp: LinearLayout
    private lateinit var progressBar: ProgressBar
    private lateinit var ImageLight: ImageView
    private lateinit var ImageHeat: ImageView
    private lateinit var ImageCover: ImageView
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        ValTemp = findViewById(R.id.temperature)
        ValHum = findViewById(R.id.humidity)
        ValOpen = findViewById(R.id.openstatus)
        ValLight = findViewById(R.id.lightstatus)
        ValNoise = findViewById(R.id.noise)
        ValAtivity = findViewById(R.id.activity)
        ValHeat = findViewById(R.id.heatingstatus)
        ValUpdate = findViewById(R.id.updated_at)
        //progressBar = findViewById(R.id.loader)
        BtnMap = findViewById(R.id.boxMap)
        BtnHum = findViewById(R.id.boxhum)
        BtnLight = findViewById(R.id.boxlight)
        BtnHeat = findViewById(R.id.boxheat)
        BtnTemp = findViewById(R.id.boxTemp)
        ImageLight = findViewById(R.id.imglight)
        ImageHeat = findViewById(R.id.imgheat)
        ImageCover = findViewById(R.id.imgcover)
      //  progressBar.visibility = View.VISIBLE



        val baseUrl = "http://10.0.2.2:5000/data/"



        ApiCall().getsensor(this) { payload ->
            var temp = payload.temperature.toString()
            var hum = payload.humidity.toString()
            var cover = payload.digital_in.toString()
            var heat = payload.heating.toString()
            var lig = payload.lights.toString()
            var noise = payload.noise.toString()
            var activity = payload.activity.toString()
            //TO DO when new version of payload NO GPS
            if (cover == "false") {
                ValOpen.text = "open"
                ImageCover.setImageResource(R.drawable.openclose)
            }
            else {
                ValOpen.text = "closed"
                ImageCover.setImageResource(R.drawable.close)
            }


            if (heat == "true") {
                ValHeat.text = "ON"
                ImageHeat.setImageResource(R.drawable.heatingon)
            }
            else{
                ValHeat.text = "OFF"
                ImageHeat.setImageResource(R.drawable.heating)
            }
            if (lig == "true") {
                ValLight.text = "ON"
                ImageLight.setImageResource(R.drawable.lighton)
            }
            else {
                ValLight.text = "OFF"
                ImageLight.setImageResource(R.drawable.light)
            }
            if (noise == "true") {
                ValNoise.text = "high"
            }
            else {
                ValNoise.text = "low"
            }
            if (activity == "true") {
                ValAtivity.text = "high"
            }
            else {
                ValAtivity.text = "low"
            }


            //to this line
            //Setting correct timestamp of update for last update
            val currentTimestamp = System.currentTimeMillis()
            val timeZone = TimeZone.getTimeZone("Europe/Warsaw")
            val dateFormat = SimpleDateFormat("dd MMM yyyy, HH:mm", Locale("pl", "PL"))

            dateFormat.timeZone = timeZone
            ValUpdate.text = dateFormat.format(Date(currentTimestamp))
            ValTemp.text = "$temp°C"
            ValHum.text = "$hum%"
            //progressBar.visibility = View.GONE

        }

        BtnMap.setOnClickListener() {
            val intent = Intent(this, MapActivity::class.java)
            startActivity(intent)

        }
        BtnTemp.setOnClickListener() {
            val intent = Intent(this, TemperatureActivity::class.java)
            startActivity(intent)

        }
        BtnHum.setOnClickListener() {
            val intent = Intent(this, HumidityActivity::class.java)
            startActivity(intent)

        }
        BtnLight.setOnClickListener() {


            ImageLight.setImageResource(R.drawable.lighton)

            ApiCall().getsensor(this) { payload ->
                var lig = payload.lights
                var str =""

                if (lig == true)
                    str = "${baseUrl}lights?value=false"
                else
                    str = "${baseUrl}lights?value=true"
                lig= !lig

                val jsonParams = JSONObject()

                val request = JsonObjectRequest(
                    Request.Method.POST, "${str}", jsonParams,
                    { response ->

                        if (lig == true) {
                            ValLight.text = "ON"
                            ImageLight.setImageResource(R.drawable.lighton)
                        }
                        else {
                            ValLight.text = "OFF"
                            ImageLight.setImageResource(R.drawable.light)
                        }

                    },
                    { error ->
                        Log.e("MainActivity", "Error during API call: ${error.localizedMessage}")
                    }
                )
                Volley.newRequestQueue(this).add(request)
            }
        }
        BtnHeat.setOnClickListener() {




            ApiCall().getsensor(this) { payload ->
                var heat = payload.heating
                var str =""

                if (heat == true)
                    str = "${baseUrl}heating?value=false"
                else
                    str = "${baseUrl}heating?value=true"

                heat= !heat!!

                val jsonParams = JSONObject()

                val request = JsonObjectRequest(
                    Request.Method.POST, "${str}", jsonParams,
                    { response ->

                        if (heat == true) {
                            ValHeat.text = "ON"
                            ImageHeat.setImageResource(R.drawable.heatingon)
                        }
                        else{
                            ValHeat.text = "OFF"
                            ImageHeat.setImageResource(R.drawable.heating)
                        }

                    },
                    { error ->
                        Log.e("MainActivity", "Error during API call: ${error.localizedMessage}")

                    }
                )
                Volley.newRequestQueue(this).add(request)
            }
        }
    }

}