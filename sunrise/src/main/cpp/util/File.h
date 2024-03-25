#ifndef AND_FILE_H
#define AND_FILE_H


#include "../android/AndroidClassWrapperBase.h"

class File  : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    File(const jobject& core);

    File(jstring pathname);
    File(const File& parent, jstring child);

    File(const File&)= default;
    File& operator=(const File&)=delete;

    jstring getAbsolutePath();
    jstring getPath();
    jboolean exists();

private:
    File(){}
};


#endif //AND_FILE_H
