#ifndef AND_TEXTVIEW_H
#define AND_TEXTVIEW_H


#include "View.h"

class TextView : public View {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    TextView(const jobject& core);

    TextView(const TextView&) = delete;
    TextView& operator=(const TextView&)=delete;

    void setText(jstring text);
};


#endif //AND_TEXTVIEW_H
