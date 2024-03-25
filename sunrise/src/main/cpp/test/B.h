#ifndef SUNRISE_B_H
#define SUNRISE_B_H

#include "A.h"


class B: public A{
private:
    int num = 0;
public:
    B(int _num);
    void func1();
    void func2();
};


#endif //SUNRISE_B_H
