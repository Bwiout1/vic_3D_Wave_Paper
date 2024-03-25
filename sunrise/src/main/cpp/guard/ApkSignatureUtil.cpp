#include <string>
#include "ApkSignatureUtil.h"
#include "../util/LocalFrame.h"
#include "../util/digest/md5.h"
#include "../util/Logger.h"
#include "../android/Application.h"
#include "../android/PackageManager.h"
#include "../android/AndroidJni.h"

namespace ApkSignatureUtil {
    int check_apk_signature(JNIEnv *env){
        LocalFrame frame(env);

        int sign_consistent = -1;

        do{
            Application& app = Application::getInstance();

            auto pkgName_jstring = app.getPackageName();
            if(pkgName_jstring== nullptr)
                break;

            PackageManager pkgManager(app.getPackageManager());
            int flags ;
//            if(AndroidJni::get_dev_api_level() >=28){
//                flags = 0x08000000; //PackageManager.GET_SIGNING_CERTIFICATES
//            } else {
                flags = 0x00000040; //PackageManager.GET_SIGNATURES
//            }
            jobject package_info = pkgManager.getPackageInfo(pkgName_jstring, flags);
            if(package_info== nullptr)
                break;

            jclass package_info_clz = env ->GetObjectClass(package_info);
            if(package_info_clz== nullptr)
                break;


            jfieldID signatures_id = env->GetFieldID(package_info_clz, "signatures", "[Landroid/content/pm/Signature;");
            if(signatures_id== nullptr)
                break;


            auto signArray = (jobjectArray) env->GetObjectField(package_info, signatures_id);
            if(signArray == nullptr)
                break;


            jobject sign = env->GetObjectArrayElement(signArray, 0);
            if(sign== nullptr)
                break;


            jclass signature_clz = env->GetObjectClass(sign);
            if(signature_clz== nullptr)
                break;

            jmethodID toByteArray = env->GetMethodID(signature_clz, "toByteArray","()[B");
            if(toByteArray== nullptr)
                break;

            auto sign_jbyteArray = static_cast<jbyteArray>(env->CallObjectMethod(sign, toByteArray));
            if(sign_jbyteArray== nullptr)
                break;

            jsize sign_size= env ->GetArrayLength(sign_jbyteArray);
            const jbyte* sign_pData = env ->GetByteArrayElements(sign_jbyteArray, nullptr);
            string apkSignature = md5(sign_pData, sign_size);

            std::transform(apkSignature.begin(), apkSignature.end(), apkSignature.begin(), std::toupper);
            LOGD("Apk Sign md5 calculated by jni:%s", apkSignature.c_str());
            env->ReleaseByteArrayElements(sign_jbyteArray, const_cast<jbyte *>(sign_pData), 0);

#if defined(APK_SIGN_MD5_UPPER)
            LOGD("Apk Sign md5 configured in build.gradle:%s", APK_SIGN_MD5_UPPER);
            if(apkSignature == APK_SIGN_MD5_UPPER){
                sign_consistent= 0;
            }
#endif
        } while (0);


        return sign_consistent;
    }
};