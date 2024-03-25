#ifndef AND_ACTIVITY_H
#define AND_ACTIVITY_H


#include "Context.h"

class Activity : public Context {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Activity(const jobject& context);

    jobject getResources();
    void setTaskDescription(const jobject& description);
    jobject findViewWithTag(const char* str);
    jobject getIntent();
    jobject getWindow();
    jboolean isDestroyed();
    jboolean isFinishing();

private:
    Activity(const Activity&)= delete;
    Activity& operator=(const Activity&)=delete;
};


#endif //SUNRISE_ACTIVITY_H
