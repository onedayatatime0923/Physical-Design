
#include "global.hpp"

void showMemoryUsage(bool peak) {
    if (peak)
        fprintf(stderr, "Peak Memory Usage: %f MB\n", getPeakMemoryUsage() / MEMORY_SCALE);
    else
        fprintf(stderr, "Curr Memory Usage: %f MB\n", getCurrMemoryUsage() / MEMORY_SCALE);
}

double getPeakMemoryUsage() {
    #ifdef __linux__
        FILE* fmem = fopen("/proc/self/status", "r");
        char membuf[128];
        double vmPeak = 0;
        while (fgets(membuf, 128, fmem)) {
            char* ch;
            if ((ch = strstr(membuf, "VmPeak:"))) {
            vmPeak = atol(ch + 7);
            continue;
            }
        }
        fclose(fmem);
        return vmPeak;
    #else
        return -1;
    #endif
}

double getCurrMemoryUsage() {
    #ifdef __linux__
        FILE* fmem = fopen("/proc/self/status", "r");
        char membuf[128];
        double vmSize = 0;
        while (fgets(membuf, 128, fmem)) {
            char* ch;
            if ((ch = strstr(membuf, "VmSize:"))) {
                vmSize = atol(ch + 7);
                break;
            }
        }
        fclose(fmem);
        return vmSize;
    #else
        return -1;
    #endif
}

