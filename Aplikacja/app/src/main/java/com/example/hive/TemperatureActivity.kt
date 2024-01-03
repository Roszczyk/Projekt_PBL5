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
        val url = "http://10.0.2.2:5000/data/tempHum"



        val request = JsonObjectRequest(
            Request.Method.GET, url, null,
            { response->
                val responseStr = response.toString()
                Log.d("MainActivity", "Response: $responseStr")
                val itemArray = JSONArray(response.toString())
                Log.d("MainActivity", "${itemArray}")

                val dataList = ArrayList<DataTempHum>()
                for(i in 0 until itemArray.length() ) {
                    val resultObject = itemArray.getJSONObject(i)
                    val humidity = resultObject.getString("humidity")
                    val temperature = resultObject.getString("temperature")
                    val timestamp = resultObject.getString("timestamp")

                    val dataTempHum = DataTempHum(humidity, temperature, timestamp)
                    dataList.add(dataTempHum)

                }

            },
            { error->
                Log.e("MainActivity", "${error.message}")
            })
        Volley.newRequestQueue(this).add(request)



//        val jsonString = """
//        [
//          {
//            "humidity": 34.4,
//            "temperature": 26.9,
//            "timestamp": "Tue, 02 Jan 2024 16:21:19 GMT"
//          },
//          {
//            "humidity": 34.1,
//            "temperature": 26.7,
//            "timestamp": "Tue, 02 Jan 2024 16:21:29 GMT"
//          },
//          {
//            "humidity": 34.1,
//            "temperature": 27.0,
//            "timestamp": "Tue, 02 Jan 2024 16:21:39 GMT"
//          },
//          {
//            "humidity": 34.5,
//            "temperature": 26.7,
//            "timestamp": "Tue, 02 Jan 2024 16:21:49 GMT"
//          }
//        ]
//    """


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


