package com.example.hive

import android.icu.text.SimpleDateFormat
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import androidx.core.content.ContextCompat
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.Volley
import com.github.mikephil.charting.animation.Easing
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.components.AxisBase
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
import java.time.format.DateTimeFormatter
import java.util.Locale

class TemperatureActivity : AppCompatActivity() {

    private lateinit var lineChart: LineChart
    private lateinit var textView:  TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_temperature)
        lineChart = findViewById(R.id.charttemp)
        textView = findViewById(R.id.textView)
        val url = "http://10.0.2.2:5000/data/temp-hum"


        val request = JsonObjectRequest(
            Request.Method.GET, url, null,
            { response->

                val dataArray = response.getJSONArray("data")
                Log.d("MainActivity", "${dataArray}")

                val dataList = ArrayList<DataTempHum>()
                for (i in 0 until dataArray.length()) {
                //for (i in 0 until 15) {
                    val resultObject = dataArray.getJSONObject(i)
                    val humidity = resultObject.getDouble("humidity")
                    val temperature = resultObject.getDouble("temperature")
                    val timestamp = resultObject.getString("timestamp")
                    textView.text=timestamp

                    val dataTempHum = DataTempHum(humidity, temperature, timestamp)
                    dataList.add(dataTempHum)
                }
                setUpLineChart()
                setDataToLineChart(dataList)

            },
            { error->
                 Log.e("MainActivity", "Error during API call: ${error.localizedMessage}")
                Log.e("MainActivity", "Status Code: ${error.networkResponse.statusCode}")
                Log.e("MainActivity", "Response Data: ${String(error.networkResponse.data)}")
            })
        Volley.newRequestQueue(this).add(request)




    }

    private fun setUpLineChart() {
        with(lineChart) {

            axisRight.isEnabled = false
           // animateX(1200, Easing.EaseInSine)

            description.isEnabled = false

            xAxis.position = XAxis.XAxisPosition.BOTTOM
            //xAxis.valueFormatter = MyAxisFormatter()
            xAxis.granularity = 1F
            xAxis.setDrawGridLines(false)
            xAxis.setDrawAxisLine(false)
            axisLeft.setDrawGridLines(false)
            extraRightOffset = 30f

            legend.isEnabled = true
            legend.orientation = Legend.LegendOrientation.VERTICAL
            legend.verticalAlignment = Legend.LegendVerticalAlignment.TOP
            legend.horizontalAlignment = Legend.LegendHorizontalAlignment.CENTER
            legend.form = Legend.LegendForm.LINE



        }
    }

    private fun setDataToLineChart(dataTempHumList: List<DataTempHum>) {

        val weekOneSales = LineDataSet(week1(dataTempHumList), "Temperature")
        weekOneSales.lineWidth = 3f
        weekOneSales.valueTextSize = 15f
        weekOneSales.mode = LineDataSet.Mode.CUBIC_BEZIER
       // weekOneSales.color = ContextCompat.getColor(this, R.color.red)
       // weekOneSales.valueTextColor = ContextCompat.getColor(this, R.color.red)
        weekOneSales.enableDashedLine(20F, 10F, 0F)

        val dataSet = ArrayList<ILineDataSet>()
        dataSet.add(weekOneSales)


        val lineData = LineData(dataSet)
        lineChart.data = lineData

        lineChart.invalidate()
    }

    private fun week1(dataTempHumList: List<DataTempHum>): ArrayList<Entry> {
        val sales = ArrayList<Entry>()

        for ((index, data) in dataTempHumList.withIndex()) {
            // Assuming you want to use the timestamp as x-axis and temperature as y-axis
            //val timestampMillis = java.text.SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z").parse(data.timestamp).time
            val temperature = data.temperature?.toFloat() ?: 0f




           // if (timestampMillis != null) {
                sales.add(Entry(index.toFloat(), temperature))
            //}
        }

        return sales
    }




}


