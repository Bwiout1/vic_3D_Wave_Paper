package com.takeoff.celebs.qwer.Fragments;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.sunrise.sunrise.ads.AdsMaster;
import com.takeoff.celebs.qwer.Color_Source;
import com.takeoff.celebs.qwer.PhotoAdapter;
import com.takeoff.celebs.qwer.PhotoBean;
import com.takeoff.celebs.qwer.R;
import com.takeoff.celebs.qwer.Selected_Photo;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Fragment_green extends Fragment  {

    private RecyclerView recyclerView;
    private PhotoAdapter photoAdapter;
    private List<PhotoBean> datalist;

    private Color_Source color_source;

    @SuppressLint("MissingInflatedId")
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view = inflater.inflate(R.layout.fragment11_green, container, false);

        recyclerView = view.findViewById(R.id.green_recycle);

        GridLayoutManager layoutManager = new GridLayoutManager(this.getActivity(), 2);


        color_source = new Color_Source();
        photoAdapter = new PhotoAdapter(this.getActivity());
        recyclerView.setLayoutManager(layoutManager);
        datalist = new ArrayList<>();
        photoAdapter.setData(datalist);
        for(int i=0;i<color_source.Green_url.length;i++){
            PhotoBean item = new PhotoBean();
            int num = Get_like_num();
            item.setUrl(color_source.Green_url[i]);
            item.setName(num +"K");
            datalist.add(item);
        }
        recyclerView.setAdapter(photoAdapter);

        photoAdapter.setOnItemClickListener(new PhotoAdapter.onItemViewClickListener() {
            @Override
            public void onItemClick(int position) {
                PhotoBean item = datalist.get(position);
                Intent intent = new Intent(getActivity(), Selected_Photo.class);
                intent.putExtra("posion",position);
                intent.putExtra("url",item.getUrl());
                intent.putExtra("kinds","green");
                AdsMaster.Companion.getInstance().showInters(getActivity(), () -> {
                    startActivity(intent);
                });
            }
        });

        return view;

    }
    public int Get_like_num(){
        Random random = new Random();
        int ran_num = random.nextInt(30)+1;
        return ran_num;
    }
}
