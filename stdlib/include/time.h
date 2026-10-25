#ifndef VULCAN_TIME_H
#define VULCAN_TIME_H

typedef long time_t;
typedef long clock_t;

#define CLOCKS_PER_SEC 1000L

/** Return the current wall-clock time in milliseconds since the Unix epoch. */
time_t time(void);

/** Return the runtime wall-clock timestamp in milliseconds, not process CPU time. */
clock_t clock(void);

/** Return end - start, two time() millisecond stamps, as a double-precision value in seconds. */
double difftime(time_t end, time_t start);

#endif
