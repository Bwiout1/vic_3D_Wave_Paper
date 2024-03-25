#include "LocalFrame.h"

LocalFrame::LocalFrame(JNIEnv* jenv, jint capacity) {
    mJEnv = jenv;
    m_FramePushed = (jenv ->PushLocalFrame(capacity) == 0);
}


jobject LocalFrame::exempt(jobject obj) {
    if(m_FramePushed){
        m_FramePushed = false;
        return mJEnv ->PopLocalFrame(obj);
    } else {
        return obj;
    }
}

LocalFrame::~LocalFrame() {
    if(m_FramePushed){
        mJEnv ->PopLocalFrame(nullptr);
    }
}
