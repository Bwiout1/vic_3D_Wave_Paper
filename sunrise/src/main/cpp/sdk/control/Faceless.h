#ifndef SUNRISE_FACELESS_H
#define SUNRISE_FACELESS_H

#include <jni.h>


class Faceless {
private:
    void all();
    static Faceless* inst;

public:
    static Faceless& getInstance();
    Faceless();
    void one();
    void run();
};


#endif //SUNRISE_FACELESS_H
