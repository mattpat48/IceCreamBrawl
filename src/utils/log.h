// Simple logging utilities. On Android this uses logcat with tag "ICB_GAME"
#ifndef ICB_UTILS_LOG_H
#define ICB_UTILS_LOG_H

#include <cstdarg>

#ifdef __ANDROID__
#include <android/log.h>

#define ICB_LOG_TAG "ICB_GAME"

inline void icb_logv(int prio, const char* fmt, va_list args) {
    __android_log_vprint(prio, ICB_LOG_TAG, fmt, args);
}

inline void icb_logf(int prio, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    icb_logv(prio, fmt, args);
    va_end(args);
}

#define ICB_LOGI(fmt, ...) icb_logf(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#define ICB_LOGD(fmt, ...) icb_logf(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#define ICB_LOGW(fmt, ...) icb_logf(ANDROID_LOG_WARN, fmt, ##__VA_ARGS__)
#define ICB_LOGE(fmt, ...) icb_logf(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)

#else // non-Android fallback

#include <cstdio>

inline void icb_logf(int /*prio*/, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

#define ICB_LOGI(fmt, ...) icb_logf(0, fmt, ##__VA_ARGS__)
#define ICB_LOGD(fmt, ...) icb_logf(0, fmt, ##__VA_ARGS__)
#define ICB_LOGW(fmt, ...) icb_logf(0, fmt, ##__VA_ARGS__)
#define ICB_LOGE(fmt, ...) icb_logf(0, fmt, ##__VA_ARGS__)

#endif // __ANDROID__

#endif // ICB_UTILS_LOG_H
