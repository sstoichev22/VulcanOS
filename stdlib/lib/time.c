#include <time.h>

clock_t clock(void) {
    return (clock_t)time();
}

double difftime(time_t end, time_t start) {
    return ((double)end - (double)start) / (double)CLOCKS_PER_SEC;
}
