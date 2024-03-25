#ifndef AND_INPUTSTREAM_H
#define AND_INPUTSTREAM_H


#include "../android/AndroidClassWrapperBase.h"

class InputStream : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    InputStream(const jobject& core);

    InputStream(const InputStream&)= default;
    InputStream& operator=(const InputStream&)=delete;

    jint available();
    jlong skip(jlong n);
    jint read(jbyteArray buf);
    void close();

private:
    InputStream(){}
};


#endif //AND_INPUTSTREAM_H
