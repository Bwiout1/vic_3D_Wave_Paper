#ifndef SUNRISE_HASHMAP_H
#define SUNRISE_HASHMAP_H


#include "../android/AndroidClassWrapperBase.h"

class HashMap : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    HashMap();
    HashMap& put(const jobject& key, const jobject& val);
    HashMap& put(const char* key, const jobject& val);
    HashMap& put(const char* str1, const char* str2);

    bool isNull = true;
};


#endif //SUNRISE_HASHMAP_H
