package com.example.hive

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Button
import android.widget.ProgressBar
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    private lateinit var btn: Button
    private lateinit var tele: TextView
    private lateinit var progressBar: ProgressBar

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btn = findViewById(R.id.btn)

        tele = findViewById(R.id.tele)

        progressBar = findViewById(R.id.idLoadingPB)


        btn.setOnClickListener {
            // show the progress bar
            progressBar.visibility = View.VISIBLE

//            ApiCall().getTempHum(this) { tempHum ->
//                tele.text = tempHum.humidity.toString()
//                progressBar.visibility = View.GONE
//            }

            ApiCall().getgps(this) { gps ->
                tele.text = gps.gps_lon.toString()
                progressBar.visibility = View.GONE
            }
        }
//        ApiCall().getgps(this) { gps ->
//            tele.text = gps.gps_lon.toString()
//            if(gps.message != null)
//                tele.text = "no data"
//            // hide the progress bar
//            progressBar.visibility = View.GONE
//        }
    }
}
