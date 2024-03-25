package com.takeoff.celebs.qwer;

import static android.view.View.GONE;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioGroup;

import com.takeoff.celebs.qwer.Fragments.Fragment_2d;
import com.takeoff.celebs.qwer.Fragments.Fragment_3d;
import com.takeoff.celebs.qwer.Fragments.Fragment_blue;
import com.takeoff.celebs.qwer.Fragments.Fragment_fire;
import com.takeoff.celebs.qwer.Fragments.Fragment_gray;
import com.takeoff.celebs.qwer.Fragments.Fragment_green;
import com.takeoff.celebs.qwer.Fragments.Fragment_love;
import com.takeoff.celebs.qwer.Fragments.Fragment_orange;
import com.takeoff.celebs.qwer.Fragments.Fragment_parallax_3d;
import com.takeoff.celebs.qwer.Fragments.Fragment_pink;
import com.takeoff.celebs.qwer.Fragments.Fragment_purple;
import com.takeoff.celebs.qwer.Fragments.Fragment_red;
import com.takeoff.celebs.qwer.Fragments.Fragment_yellow;
import com.takeoff.celebs.qwer.R;

public class Fication extends FragmentActivity {

    public static int count=0;
    public static int count1=0;

    public  int random_num;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fication);

        ImageView img_search_logo = findViewById(R.id.search_logo);
        LinearLayout search_detail = findViewById(R.id.search_detail);
        ImageView img_color_logo = findViewById(R.id.color_picker_logo);
        LinearLayout color_detail = findViewById(R.id.color_picker_detail);

        ImageView back = findViewById(R.id.fication_back);
        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });

        RadioGroup radioGroup = findViewById(R.id.set_fragments);
        radioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId==R.id.txt_3d){
                    loadFragment(new Fragment_3d());
                } else if (checkedId==R.id.txt_parallax) {
                    loadFragment(new Fragment_parallax_3d());
                } else if (checkedId==R.id.txt_fire) {
                    loadFragment(new Fragment_fire());
                } else if (checkedId==R.id.txt_2d) {
                    loadFragment(new Fragment_2d());
                } else if (checkedId==R.id.txt_love) {
                    loadFragment(new Fragment_love());
                }
            }
        });

        RadioGroup radioGroup1 = findViewById(R.id.color_group);
        radioGroup1.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId==R.id.btn_gray){
                    loadFragment(new Fragment_gray());
                } else if (checkedId==R.id.btn_pink) {
                    loadFragment(new Fragment_pink());
                } else if (checkedId==R.id.btn_red) {
                    loadFragment(new Fragment_red());
                } else if (checkedId==R.id.btn_orange) {
                    loadFragment(new Fragment_orange());
                } else if (checkedId==R.id.btn_yellow) {
                    loadFragment(new Fragment_yellow());
                } else if (checkedId==R.id.btn_green) {
                    loadFragment(new Fragment_green());
                } else if (checkedId==R.id.btn_blue) {
                    loadFragment(new Fragment_blue());
                } else if (checkedId==R.id.btn_purple) {
                    loadFragment(new Fragment_purple());
                }

            }
        });


        img_search_logo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                count++;
                search_detail.setVisibility(View.VISIBLE);
                img_search_logo.setVisibility(GONE);
                for(int i=0;i<500;i++){
                    if (count%2==0){
                        search_detail.setVisibility(GONE);
                        img_search_logo.setVisibility(View.VISIBLE);
                    } else  {
                        search_detail.setVisibility(View.VISIBLE);
                        img_search_logo.setVisibility(GONE);
                    }
                }


            }
        });

        img_color_logo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                count1++;
                color_detail.setVisibility(View.VISIBLE);
                img_color_logo.setVisibility(GONE);
                if(count1%2==0){
                    color_detail.setVisibility(GONE);
                    img_color_logo.setVisibility(View.VISIBLE);
                }else {
                    color_detail.setVisibility(View.VISIBLE);
                    img_color_logo.setVisibility(GONE);
                }
            }
        });

        loadFragment(new Fragment_3d());


    }


    private void loadFragment(Fragment fragment) {
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.replace(R.id.fragment_container_view_tag, fragment);
        fragmentTransaction.commit();
    }


}

