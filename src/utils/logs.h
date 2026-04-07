#ifdef __ANDROID__
#include <android/log.h>
#define APP_LOG(...) __android_log_print(ANDROID_LOG_INFO, "ICB_GAME", __VA_ARGS__)
#else
#include <stdio.h>
#define APP_LOG(...) printf(__VA_ARGS__); printf("\n")
#endif