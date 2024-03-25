package com.takeoff.celebs.qwer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.takeoff.celebs.qwer.R;

public class ExitActivity extends FragmentActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_exit);

        TextView txt_yes = findViewById(R.id.txt_yes);
        TextView txt_no = findViewById(R.id.txt_no);

        txt_yes.setOnClickListener(this::onClick);
        txt_no.setOnClickListener(this::onClick);
    }

    @Override
    public void onClick(View v) {
        if(v.getId()==R.id.txt_yes){
            System.exit(0);
        } else if (v.getId()==R.id.txt_no) {
            onBackPressed();

        }
    }
}