#if defined(_WIN64) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#define PLATFORM_NAME "Windows"
#elif __APPLE__
#define PLATFORM_NAME "MacOS"
#elif __linux__
#define PLATFORM_NAME "Linux"
#else
#define PLATFORM_NAME "Unknown"
#endif
#include <string>

std::string getPlatformName() {
    return PLATFORM_NAME;
}

