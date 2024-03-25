package com.takeoff.celebs.qwer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import com.takeoff.celebs.qwer.R;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Go_Gallery extends FragmentActivity {

    List<PhotoBean> data = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_go_gallery);

        ImageView back = findViewById(R.id.gallery_back);
        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });

        RecyclerView gall_re = findViewById(R.id.gallery_recyclerview);
        PhotoAdapter photoAdapter = new PhotoAdapter(this);
        PhotoSource photoSource = new PhotoSource();
        gall_re.setLayoutManager(new GridLayoutManager(this,2));
        for (int i=0 ;i<photoSource.gallery.length;i++){
            PhotoBean item = new PhotoBean();
            int num = Get_like_num();
            item.setUrl(photoSource.gallery[i]);
            item.setName(num +"K");
            data.add(item);

        }
        photoAdapter.setData(data);
        gall_re.setAdapter(photoAdapter);
        photoAdapter.setOnItemClickListener(new PhotoAdapter.onItemViewClickListener() {
            @Override
            public void onItemClick(int position) {
                PhotoBean item = data.get(position);
                Intent intent = new Intent(Go_Gallery.this,Selected_Photo.class);
                intent.putExtra("posion",position);
                intent.putExtra("url",item.getUrl());
                intent.putExtra("kinds","gallery");
                startActivity(intent);
            }
        });



    }
    public int Get_like_num(){
        Random random = new Random();
        int ran_num = random.nextInt(30)+1;
        return ran_num;
    }
}