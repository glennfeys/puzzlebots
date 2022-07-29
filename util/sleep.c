#include "sleep.h"

#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#else
//use POSIX nanosleep  (doesn't work with -std=c99, you need -std=gnu99)
#include <time.h>
#endif

void sleep_ms(uint64_t milliSeconds) {
#ifdef _WIN32
    Sleep(milliSeconds);
#else
    struct timespec request, actual;
    request.tv_sec = milliSeconds / 1000L;
    request.tv_nsec = (milliSeconds % 1000L) * 1000000L;
    
    if(nanosleep(&request , &actual) < 0) {
        fprintf(stderr, "Warning: nanosleep system call failed \n");
        return;
    }
#endif

    return;
}
