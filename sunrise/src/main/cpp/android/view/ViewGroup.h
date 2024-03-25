#ifndef AND_VIEWGROUP_H
#define AND_VIEWGROUP_H

#include "View.h"


class ViewGroup : public View {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    ViewGroup(const jobject& core);

    ViewGroup(const ViewGroup&) = delete;
    ViewGroup& operator=(const ViewGroup&)=delete;

    void removeAllViews();
    void addView(const jobject& child);

    jint getChildCount();
};


#endif //AND_VIEWGROUP_H
