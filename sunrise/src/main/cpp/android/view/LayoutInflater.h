#ifndef AND_LAYOUTINFLATER_H
#define AND_LAYOUTINFLATER_H

#include "ViewGroup.h"


class LayoutInflater : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject from(const jobject& context);

public:
    LayoutInflater(const jobject& core);

    LayoutInflater(const LayoutInflater&)= default;
    LayoutInflater& operator=(const LayoutInflater&)=delete;

    jobject inflate(jint resource, ViewGroup& root, bool attachToRoot);

private:
    LayoutInflater(){}

};


#endif //AND_LAYOUTINFLATER_H
