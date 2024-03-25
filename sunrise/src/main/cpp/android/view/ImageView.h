#ifndef AND_IMAGEVIEW_H
#define AND_IMAGEVIEW_H


#include "View.h"


class ImageView : public View {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    ImageView(const jobject& core);

    ImageView(const ImageView&) = delete;
    ImageView& operator=(const ImageView&)=delete;

    void setImageBitmap(const jobject & bm);
};


#endif //AND_IMAGEVIEW_H
