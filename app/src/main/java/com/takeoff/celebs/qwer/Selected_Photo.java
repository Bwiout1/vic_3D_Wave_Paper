package com.takeoff.celebs.qwer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentActivity;
import androidx.viewpager.widget.PagerAdapter;
import androidx.viewpager.widget.ViewPager;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.takeoff.celebs.qwer.R;

public class Selected_Photo extends FragmentActivity {
    public  static int pos;
    public static String url;
    public  static String [] source_url;
    public  static String kinds;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_selected_photo);


        Intent intent = getIntent();
        pos = intent.getIntExtra("posion",0);
        url = intent.getStringExtra("url");
        kinds = intent.getStringExtra("kinds");
        Set_Source_url();

        ViewPager viewPager = findViewById(R.id.viewPager);
        viewPager.setAdapter(new MyPagerAdapter());
        viewPager.setCurrentItem(pos);
        viewPager.setPageTransformer(false, new ViewPager.PageTransformer() {
            private static final float MIN_SCALE = 0.6f;
            @Override
            public void transformPage(@NonNull View page, float position) {
                int pageWidth = page.getWidth();
                int pageHeight = page.getHeight();

                if (position < -1) { // 页面位于屏幕左侧

                } else if (position <= 1) { // 页面在屏幕左侧和右侧之间
                    // 缩放页面大小
                    float scaleFactor = Math.max(MIN_SCALE, 1 - Math.abs(position));
                    float vertMargin = pageHeight * (1 - scaleFactor) / 2;
                    float horzMargin = pageWidth * (1 - scaleFactor) / 2;
                    if (position < 0) {
                        page.setTranslationX(horzMargin - vertMargin / 2);
                    } else {
                        page.setTranslationX(-horzMargin + vertMargin / 2);
                    }

                    page.setScaleX(scaleFactor);
                    page.setScaleY(scaleFactor);

                    // 调整透明度
                    // 选中的图片变大
                    if (position == 0) {
                        page.setScaleX(1f);
                        page.setScaleY(1f);
                    }
                } else { // 页面位于屏幕右侧

                }
            }
        });

        TextView txt_apply = findViewById(R.id.apply_wallpaper);
        ImageView img_set_like = findViewById(R.id.select_wallpaper_like);
        ImageView img_share = findViewById(R.id.photo_share);
        ImageView back = findViewById(R.id.selected_back);

        img_set_like.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                img_set_like.setImageResource(R.drawable.wallpaper_like_sel);
                for (int i =1;i<500;i++){
                    if (i%2==0){
                        img_set_like.setImageResource(R.drawable.wallpaper_like);
                    }else {
                        img_set_like.setImageResource(R.drawable.wallpaper_like_sel);
                    }
                }
            }
        });

        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();
            }
        });

        txt_apply.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Download_set.donwloadImg(getApplicationContext(),url);
            }
        });

        viewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {}

            @Override
            public void onPageSelected(int position) {
                //选中的页面
                pos = position;
                url = source_url[pos];
            }

            @Override
            public void onPageScrollStateChanged(int state) {}
        });


    }
    class MyPagerAdapter extends PagerAdapter {

        @Override
        public int getCount() {
            return source_url.length;
        }

        @NonNull
        @Override
        public Object instantiateItem(@NonNull ViewGroup container, int position) {
            ImageView imageView = new ImageView(Selected_Photo.this);


            Glide.with(Selected_Photo.this)
                    .load(source_url[position])
                    .into(imageView);

            imageView.setScaleType(ImageView.ScaleType.FIT_XY);
            container.addView(imageView);


            return imageView;
        }

        @Override
        public void destroyItem(@NonNull ViewGroup container, int position, @NonNull Object object) {
            container.removeView((ImageView) object);
        }

        @Override
        public boolean isViewFromObject(@NonNull View view, @NonNull Object object) {
            return view == object;
        }



    }

    public void Set_Source_url(){
        PhotoSource photoSource = new PhotoSource();
        Color_Source color_source = new Color_Source();
        switch (kinds){
            case "2d":
                source_url = photoSource.hd_source;
                break;
            case "3d":
                source_url = photoSource.a3d_source;
                break;
            case  "blue":
                source_url = color_source.Blue_url;
                break;
            case "fire":
                source_url = photoSource.fire_source;
                break;
            case "gray":
                source_url = color_source.White_url;
                break;
            case "green":
                source_url = color_source.Green_url;
                break;
            case "love":
                source_url = photoSource.love_source;
                break;
            case "orange":
                source_url = color_source.Orange_url;
                break;
            case "parallax":
                source_url = photoSource.parallax;
                break;
            case  "pink":
                source_url = color_source.Pink_url;
                break;
            case "purple":
                source_url = color_source.Purple_url;
                break;
            case "red":
                source_url = color_source.Red_url;
                break;
            case "yellow":
                source_url = color_source.Yellow_url;
                break;
            case "gallery":
                source_url = photoSource.gallery;
                break;
        }
    }

}