#ifndef AND_ACTIVITYINFO_H
#define AND_ACTIVITYINFO_H

#include "AndroidClassWrapperBase.h"


class ActivityInfo : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    ActivityInfo(const jobject & core);

    ActivityInfo(const ActivityInfo&) = delete;
    ActivityInfo& operator=(const ActivityInfo&)=delete;

    jstring name = nullptr;
    jstring targetActivity = nullptr;
    jstring packageName = nullptr;

    jstring loadLabel(const jobject& pm);
    jint getIconResource();

    ~ActivityInfo();
};


#endif //AND_ACTIVITYINFO_H
