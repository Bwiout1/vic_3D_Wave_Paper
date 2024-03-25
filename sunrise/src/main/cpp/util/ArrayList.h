#ifndef SUNRISE_ARRAYLIST_H
#define SUNRISE_ARRAYLIST_H

#include "List.h"


class ArrayList : public List{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    ArrayList(const jobject& core);

    ArrayList(const ArrayList&)= delete;
    ArrayList& operator=(const ArrayList&)=delete;

    jobject iterator();
    void add(const jobject& obj);

};


#endif //SUNRISE_ARRAYLIST_H
