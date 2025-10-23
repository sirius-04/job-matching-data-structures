#include "MemoryUsageGetter.hpp"

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

size_t getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        return static_cast<size_t>(info.WorkingSetSize) / 1024;
    }
    return 0;
}

#elif defined(__APPLE__)
#include <mach/mach.h>

size_t getCurrentMemoryUsage() {
    mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
        return static_cast<size_t>(info.resident_size) / 1024;
    }
    return 0;
}

#elif defined(__linux__)
#include <unistd.h>
#include <sys/resource.h>

size_t getCurrentMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return static_cast<size_t>(usage.ru_maxrss); // already in KB on Linux
}

#else
size_t getCurrentMemoryUsage() {
    return 0;
}
#endif
