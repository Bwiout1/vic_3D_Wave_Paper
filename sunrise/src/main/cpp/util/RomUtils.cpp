#include "RomUtils.h"
#include <string>
#include "../android/Build.h"


namespace RomUtils{
    bool _isRightRom(const jstring brand, const jstring manufacturer, const char* name){
        bool ret = false ;

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv() ;
        if(jenv!= nullptr && brand!= nullptr && manufacturer!= nullptr && name!= nullptr){
            auto b_ = jenv ->GetStringUTFChars(brand, nullptr);
            auto m_ = jenv ->GetStringUTFChars(manufacturer, nullptr);

            std::string b_str=b_;
            std::string m_str=m_;
            std::string name_str=name;
            std::transform(b_str.begin(), b_str.end(), b_str.begin(), std::tolower);
            std::transform(m_str.begin(), m_str.end(), m_str.begin(), std::tolower);
            std::transform(name_str.begin(), name_str.end(), name_str.begin(), std::tolower);

            if(b_str.find(name_str) != std::string::npos  ||  m_str.find(name_str) != std::string::npos)
                ret = true;

            jenv ->ReleaseStringUTFChars(brand, b_);
            jenv ->ReleaseStringUTFChars(manufacturer, m_);
        }

        return ret;
    }

    bool isVivo(){
        static bool ret = _isRightRom(Build::BRAND(), Build::MANUFACTURER(), "vivo");

        return ret;
    }

    bool isXiaomi(){
        static bool ret = _isRightRom(Build::BRAND(), Build::MANUFACTURER(), "xiaomi");

        return ret;
    }

    bool isOppo(){
        static bool ret = _isRightRom(Build::BRAND(), Build::MANUFACTURER(), "oppo");

        return ret;
    }

    bool isHuawei(){
        static bool ret = _isRightRom(Build::BRAND(), Build::MANUFACTURER(), "huawei");

        return ret;
    }

    bool isGoogle(){
        static bool ret = _isRightRom(Build::BRAND(), Build::MANUFACTURER(), "google");

        return ret;
    }

    //除了国产四大rom以外的rom
    bool isRawRom(){
        return !isXiaomi() && !isOppo() && !isVivo() && !isHuawei();
    }
}
