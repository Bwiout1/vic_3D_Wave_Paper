package com.takeoff.celebs.qwer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.takeoff.celebs.qwer.R;
import com.sunrise.sunrise.ads.AdsMaster;

public class HomeActivity extends FragmentActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        TextView txt_gallery = findViewById(R.id.txt_gallery);
        TextView txt_fication = findViewById(R.id.txt_fication);

        txt_gallery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HomeActivity.this,Go_Gallery.class);
                AdsMaster.Companion.getInstance().showInters(HomeActivity.this, () -> {
                startActivity(intent);
                });
            }
        });

        txt_fication.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HomeActivity.this,Fication.class);
                AdsMaster.Companion.getInstance().showInters(HomeActivity.this, () -> {
                    startActivity(intent);
                });

            }
        });
    }
    public void onBackPressed() {
        Intent intent = new Intent(HomeActivity.this,ExitActivity.class);
        AdsMaster.Companion.getInstance().showInters(HomeActivity.this, () -> {
            startActivity(intent);
        });
    }



}