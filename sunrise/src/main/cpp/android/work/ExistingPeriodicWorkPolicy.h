#ifndef AND_EXISTINGPERIODICWORKPOLICY_H
#define AND_EXISTINGPERIODICWORKPOLICY_H


#include "../AndroidClassWrapperBase.h"

class ExistingPeriodicWorkPolicy : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject KEEP();

private:
    ExistingPeriodicWorkPolicy(){}

    ExistingPeriodicWorkPolicy(const ExistingPeriodicWorkPolicy &) = delete;
    ExistingPeriodicWorkPolicy &operator=(const ExistingPeriodicWorkPolicy &) = delete;
};


#endif //AND_EXISTINGPERIODICWORKPOLICY_H
