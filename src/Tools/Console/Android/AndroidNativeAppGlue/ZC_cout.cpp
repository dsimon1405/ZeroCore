#include <ZC/Tools/Console/ZC_cout.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

void ZC_cout(const std::string& msg)
{
    LOGI("%s", msg.c_str());
}