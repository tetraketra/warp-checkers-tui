#include "millis.h"

/* 
    Returns a monotonic time in miliseconds.
    
    @returns The time in miliseconds, or `-1` if failed.
    @note Uses `<time.h>`, so Linux-only right now. 
    @todo Use a build flag here when you want to support Windows.
*/
int64_t millis(void) {
    struct timespec ts;

    if (!clock_gettime(CLOCK_MONOTONIC, &ts)) {
        return (int64_t)ts.tv_sec*1000 + (ts.tv_nsec+5e5)/1e6; /* "(N+5)/10" is `math.h`-less rounding. */
    } else {
        return -1;
    }
}