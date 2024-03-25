#ifndef LOCAL_FRAME_H
#define LOCAL_FRAME_H

#include <jni.h>

class LocalFrame {
public:
    LocalFrame(JNIEnv* jenv, jint capacity=64);
    /**
     * 当LocalFrame结束生命周期时候，所有localRef均被删除，但当函数需要返回一个对象时，可用函数exempt()豁免该返回对象。
     * {
     *      LocalFrame frame(env);
     *      ...
     *      jobect ret = xxxx;
     *      ...
     *      return frame.exempt(ret);
     * }
     * @param obj
     * @return
     */
    jobject exempt(jobject obj);
    ~LocalFrame();

private:
    LocalFrame(const LocalFrame& frame);
    LocalFrame& operator=(const LocalFrame& rhs);

    JNIEnv* mJEnv;
    bool m_FramePushed;
};


#endif //LOCAL_FRAME_H
