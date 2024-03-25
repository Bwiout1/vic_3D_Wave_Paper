package com.takeoff.celebs.qwer;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.resource.bitmap.RoundedCorners;
import com.takeoff.celebs.qwer.R;

import java.util.ArrayList;
import java.util.List;

public class PhotoAdapter extends RecyclerView.Adapter<PhotoAdapter.Myholder> {

    List<PhotoBean> data = new ArrayList<>();

    private Context context;

    public onItemViewClickListener listener;

    public void setData(List<PhotoBean> data) {
        this.data = data;
    }

    public PhotoAdapter(Context context) {
        this.context = context;
    }

    public void setOnItemClickListener(onItemViewClickListener listener) {
        this.listener = listener;
    }


    @NonNull
    @Override
    public Myholder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new Myholder(LayoutInflater.from(context).inflate(R.layout.photo_text_list, null));
    }

    @Override
    public void onBindViewHolder(@NonNull Myholder holder, int position) {
        Glide.with(context)
                .load(data.get(position).getUrl())
                .transform(new RoundedCorners(30))
                .into(holder.img_show);
        holder.txt_num.setText(data.get(position).getName());
        holder.img_set_like.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                holder.img_set_like.setImageResource(R.drawable.wallpaper_like_sel);
                for (int i=1;i<500;i++){
                    if(i%2==0){
                        holder.img_set_like.setImageResource(R.drawable.wallpaper_like);
                    }else{
                        holder.img_set_like.setImageResource(R.drawable.wallpaper_like_sel);
                    }
                }

            }
        });
        holder.constraintLayout.setOnClickListener(new View.OnClickListener() {//绑定列表项点击事件
            @Override
            public void onClick(View v) {
                listener.onItemClick(position);//点击跳转
            }
        });

    }

    public class Myholder extends RecyclerView.ViewHolder{

        ImageView img_show;
        ImageView img_set_like;
        TextView  txt_num;
        ConstraintLayout constraintLayout;


        public Myholder(@NonNull View itemView) {

            super(itemView);

            img_show = itemView.findViewById(R.id.img_show_photo);
            img_set_like = itemView.findViewById(R.id.img_set_like);
            txt_num = itemView.findViewById(R.id.txt_like_num);
            constraintLayout = itemView.findViewById(R.id.photo_list);

            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (listener != null) {
                        int position = getAdapterPosition();
                        if (position != RecyclerView.NO_POSITION) {
                            listener.onItemClick(position);
                        }
                    }
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        return null != data ? data.size() : 0;
    }

    public interface onItemViewClickListener {
        void onItemClick(int position);
    }

}
