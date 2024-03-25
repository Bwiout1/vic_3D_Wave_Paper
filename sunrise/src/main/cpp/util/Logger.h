#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include <jni.h>
#include <stdio.h>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STR
#define STR(x)  #x
#endif

#ifndef STR_MACRO_VAL
#define STR_MACRO_VAL(macro) STR(macro)
#endif

#if defined(__FILE_NAME__)
#define AT __FILE_NAME__ ":" STR_MACRO_VAL(__LINE__) "\t"
#elif defined(__FILENAME__)
#define AT __FILENAME__ ":" STR_MACRO_VAL(__LINE__) "\t"
#elif defined(__BASE_FILE__)
#define AT __BASE_FILE__ ":" STR_MACRO_VAL(__LINE__) "\t"
#else
#define AT "line-" STR_MACRO_VAL(__LINE__) "\t"
#endif

#ifdef NDEBUG
#   define LOGV(...) do{} while(0) // 定义LOGV类型
#   define LOGD(...) do{} while(0) // 定义LOGD类型
#   define LOGI(...) do{} while(0) // 定义LOGI类型
#   define LOGW(...) do{} while(0) // 定义LOGW类型
#   define LOGE(...) do{} while(0) // 定义LOGE类型
#   define LOGF(...) do{} while(0) // 定义LOGF类型

#   define logv(...) do{} while(0) // 定义LOGV类型
#   define logd(...) do{} while(0) // 定义LOGD类型
#   define logi(...) do{} while(0) // 定义LOGI类型
#   define logw(...) do{} while(0) // 定义LOGW类型
#   define loge(...) do{} while(0) // 定义LOGE类型
#   define logf(...) do{} while(0) // 定义LOGF类型

#else
#   define TAG "xmas" // 这个是自定义的LOG的标识
#   define LOGV(...) do{__android_log_print(ANDROID_LOG_VERBOSE, TAG, AT __VA_ARGS__);} while(0) // 定义LOGV类型
#   define LOGD(...) do{__android_log_print(ANDROID_LOG_DEBUG, TAG, AT __VA_ARGS__);} while(0) // 定义LOGD类型
#   define LOGI(...) do{__android_log_print(ANDROID_LOG_INFO, TAG, AT  __VA_ARGS__);} while(0) // 定义LOGI类型
#   define LOGW(...) do{__android_log_print(ANDROID_LOG_WARN, TAG, AT __VA_ARGS__);} while(0) // 定义LOGW类型
#   define LOGE(...) do{__android_log_print(ANDROID_LOG_ERROR, TAG, AT __VA_ARGS__);} while(0) // 定义LOGE类型
#   define LOGF(...) do{__android_log_print(ANDROID_LOG_FATAL, TAG, AT __VA_ARGS__);} while(0) // 定义LOGF类型

#define tag "sunrise"
#   define logv(...) do{__android_log_print(ANDROID_LOG_VERBOSE, tag, AT __VA_ARGS__);} while(0) // 定义LOGV类型
#   define logd(...) do{__android_log_print(ANDROID_LOG_DEBUG, tag, AT __VA_ARGS__);} while(0) // 定义LOGD类型
#   define logi(...) do{__android_log_print(ANDROID_LOG_INFO, tag, AT  __VA_ARGS__);} while(0) // 定义LOGI类型
#   define logw(...) do{__android_log_print(ANDROID_LOG_WARN, tag, AT __VA_ARGS__);} while(0) // 定义LOGW类型
#   define loge(...) do{__android_log_print(ANDROID_LOG_ERROR, tag, AT __VA_ARGS__);} while(0) // 定义LOGE类型
#   define logf(...) do{__android_log_print(ANDROID_LOG_FATAL, tag, AT __VA_ARGS__);} while(0) // 定义LOGF类型
#endif

#ifdef __cplusplus
}
#endif
#endif
