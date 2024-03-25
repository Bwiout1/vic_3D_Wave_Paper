#ifndef AND_EXISTINGWORKPOLICY_H
#define AND_EXISTINGWORKPOLICY_H


#include "../AndroidClassWrapperBase.h"

class ExistingWorkPolicy : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject APPEND_OR_REPLACE();

private:
    ExistingWorkPolicy();

    ExistingWorkPolicy(const ExistingWorkPolicy &) = delete;
    ExistingWorkPolicy &operator=(const ExistingWorkPolicy &) = delete;
};


#endif //AND_EXISTINGWORKPOLICY_H
