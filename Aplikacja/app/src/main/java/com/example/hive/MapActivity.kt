package com.example.hive

import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.MarkerOptions

class MapActivity : AppCompatActivity(), OnMapReadyCallback {

    private lateinit var mMap: GoogleMap
    private lateinit var HomeBtn: Button


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.map_activity)
        HomeBtn= findViewById(R.id.homeBtn)
        HomeBtn.text = "<        Home"
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        val mapFragment = supportFragmentManager
            .findFragmentById(R.id.map) as SupportMapFragment

        mapFragment.getMapAsync(this)
        HomeBtn.setOnClickListener() {
            val intent2 = Intent(this, MainActivity::class.java)
            startActivity(intent2)
        }

    }


    override fun onMapReady(googleMap: GoogleMap) {
        mMap = googleMap
        ApiCall().getsensor(this) { gps ->
            var lat = gps.gps_lat
            var lon = gps.gps_lon
            val loca = LatLng(lat,lon)
            mMap.addMarker(
                MarkerOptions()
                    .position(loca)
                    .title("Hive Location"))
            mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(loca, 13f))
           // mMap.setMinZoomPreference(14.0f)
        }

        // Add a marker in Sydney and move the camera

    }
}