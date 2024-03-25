#ifndef AND_VIEW_H
#define AND_VIEW_H

#include "../AndroidClassWrapperBase.h"

class View : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static const jint VISIBLE = 0;
    static const jint INVISIBLE = 4;
    static const jint GONE = 8;

public:
    View(const jobject& core);

    View(const View&) = delete;
    View& operator=(const View&)=delete;

    jobject findViewById(jint id);
    jobject getContext();
    jobject getParent();
    jobject getTag();
    void setVisibility(jint visibility);
    void setOnClickListener(jobject l);

protected:
    View(){
        if(!inited){
            init();
        }
    };
};


#endif //AND_VIEW_H
