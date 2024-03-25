#ifndef AND_FILEOUTPUTSTREAM_H
#define AND_FILEOUTPUTSTREAM_H


#include "../android/AndroidClassWrapperBase.h"

class FileOutputStream  : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    FileOutputStream(jstring name);
    FileOutputStream(const FileOutputStream&)= default;
    FileOutputStream& operator=(const FileOutputStream&)=delete;

    void write(jbyteArray data);
    void flush();
    void close();

private:
    FileOutputStream(){}
};


#endif //AND_FILEOUTPUTSTREAM_H
