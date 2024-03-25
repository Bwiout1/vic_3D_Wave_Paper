#ifndef PAPERSDK_GLIDE_H
#define PAPERSDK_GLIDE_H


#include "../android/AndroidClassWrapperBase.h"


class Glide : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

    jobject manager = nullptr;
    jobject builder = nullptr;

public:
    Glide();
    Glide& with(const jobject& context);
    Glide& load(jstring url);
    Glide& into(const jobject& v);

    ~Glide();

};


#endif //PAPERSDK_GLIDE_H
