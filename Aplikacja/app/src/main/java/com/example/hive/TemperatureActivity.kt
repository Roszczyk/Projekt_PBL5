package com.example.hive

import android.annotation.SuppressLint
import android.content.Intent
import android.graphics.Color
import android.icu.text.SimpleDateFormat
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.core.content.ContextCompat
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.Volley
import com.github.mikephil.charting.animation.Easing
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.components.AxisBase
import com.github.mikephil.charting.components.Description
import com.github.mikephil.charting.components.Legend
import com.github.mikephil.charting.components.XAxis
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import org.json.JSONArray
import java.math.BigDecimal
import java.math.RoundingMode
import java.time.format.DateTimeFormatter
import java.util.Locale

class TemperatureActivity : AppCompatActivity() {

    private lateinit var lineChart: LineChart
    private lateinit var imageBack: ImageView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_temperature)
        lineChart = findViewById(R.id.charttemp)
        imageBack = findViewById(R.id.btnBack)

        val url = "http://10.0.2.2:5000/data/temp-hum"


        val request = JsonObjectRequest(
            Request.Method.GET, url, null,
            { response->

                var dataArray = response.getJSONArray("data")
                if(dataArray.toString()=="[]"){
                    Toast.makeText(applicationContext,"Sorry, could not download the data",Toast.LENGTH_SHORT).show()
                    val intent2 = Intent(this, MainActivity::class.java)
                    startActivity(intent2)
                }

                var evenCount =0
                val dataList = ArrayList<DataTempHum>()
                val timeList =ArrayList<String>()
                for (i in (dataArray.length()-5) until dataArray.length()) {
                //for (i in 0 until 15) {
                    val resultObject = dataArray.getJSONObject(i)
                    val humidity = resultObject.getDouble("humidity")
                    val temperature = resultObject.getDouble("temperature")
                    val timestamp = resultObject.getString("timestamp")
                    val dataTempHum = DataTempHum(humidity, temperature, timestamp)
                    dataList.add(dataTempHum)

                    timeList.add(timestamp)
                }

                setUpLineChart(timeList)
                setDataToLineChart(dataList)


            },
            { error->
                 Log.e("MainActivity", "Error during API call: ${error.localizedMessage}")
                Log.e("MainActivity", "Status Code: ${error.networkResponse.statusCode}")
                Log.e("MainActivity", "Response Data: ${String(error.networkResponse.data)}")
            })
        Volley.newRequestQueue(this).add(request)



        imageBack.setOnClickListener() {
            val intent2 = Intent(this, MainActivity::class.java)
            startActivity(intent2)
        }



    }

    private fun setUpLineChart(timeList: List<String>) {
        with(lineChart) {



            axisRight.isEnabled = false
            description.isEnabled = false

            xAxis.position = XAxis.XAxisPosition.BOTTOM
            xAxis.granularity = 1F
            xAxis.setDrawGridLines(false)
            xAxis.setDrawAxisLine(false)
            xAxis.valueFormatter=IndexAxisValueFormatter(timeList)



            axisLeft.setDrawGridLines(false)
            extraRightOffset = 30f

            legend.isEnabled = false
            legend.orientation = Legend.LegendOrientation.VERTICAL
            legend.verticalAlignment = Legend.LegendVerticalAlignment.TOP
            legend.horizontalAlignment = Legend.LegendHorizontalAlignment.CENTER
            legend.form = Legend.LegendForm.LINE





        }
    }

    private fun setDataToLineChart(dataTempHumList: List<DataTempHum>) {


        val lineTemp = LineDataSet(temp(dataTempHumList), "Temperature")
        lineTemp.lineWidth = 3f
        lineTemp.valueTextSize = 15f
        lineTemp.mode = LineDataSet.Mode.CUBIC_BEZIER
        lineTemp.setColor(Color.DKGRAY)
        lineTemp.setDrawCircles(false)
        lineTemp.fillColor=Color.BLUE


        val dataSet = ArrayList<ILineDataSet>()
        dataSet.add(lineTemp)
        val lineData = LineData(dataSet)
        lineChart.data = lineData
        lineChart.invalidate()



    }


    private fun temp(dataTempHumList: List<DataTempHum>): ArrayList<Entry> {
        val sales = ArrayList<Entry>()

        for ((index, data) in dataTempHumList.withIndex()) {
            // Assuming you want to use the timestamp as x-axis and temperature as y-axis
            //val timestampMillis = java.text.SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z").parse(data.timestamp).time
            val temperature = data.temperature?.toFloat() ?: 0f
           // val timestamp = data.timestamp?.toFloat()?:0f

           // xAxisValueList.add("03:25:26"


           // if (timestampMillis != null) {
                sales.add(Entry(index.toFloat(), temperature))
            //}
        }

        return sales
    }




}


