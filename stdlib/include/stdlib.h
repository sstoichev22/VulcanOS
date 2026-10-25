#ifndef VULCAN_STDLIB_H
#define VULCAN_STDLIB_H

#include <stddef.h>

#define RAND_MAX 32767
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

/** Divide numerator by denominator using truncation toward zero. The remainder
   has the numerator's sign and satisfies numerator == quot * denominator + rem.
   A zero denominator and an unrepresentable quotient are undefined inputs. */
div_t div(int numerator, int denominator);
ldiv_t ldiv(long numerator, long denominator);

/** Return the magnitude of n. The minimum signed value cannot be represented
   as a positive value, so passing it has undefined behavior. */
int abs(int n);
long labs(long n);

/** Parse a base-10 signed integer prefix and return zero when no digits begin
   the conversion. No error status is available; out-of-range behavior follows
   the underlying strtol conversion. */
int atoi(const char *s);
long atol(const char *s);

/** Parse an optional sign followed by an integer in base 2 through 36, or use
   base 0 to select decimal, octal, or hexadecimal from the prefix. If end is
   non-null, store the first unparsed character; when no digits are consumed or
   the base is invalid, store the original input pointer. Unsigned overflow
   wraps modulo the unsigned-long width. */
long strtol(const char *s, char **end, int base);
unsigned long strtoul(const char *s, char **end, int base);

/** Parse a decimal or hexadecimal floating prefix, including inf/infinity and
   nan with an optional alphanumeric-or-underscore payload. A digitless input
   returns zero and sets end to the original input; an incomplete exponent is
   left unconsumed after the parsed value. The routines do not expose range
   errors; atof is the no-end-pointer convenience form. */
double strtod(const char *s, char **end);
float strtof(const char *s, char **end);
double atof(const char *s);

/** Stop the program with failure status or the supplied exit status. */
void abort(void);
void exit(int status);

/** Generate values in [0, RAND_MAX]. The sequence is deterministic; srand
   replaces its seed, and the default seed before the first call is 1. */
int rand(void);
void srand(unsigned int seed);

/** Allocate from the runtime's fixed 32 KiB arena. Successful blocks are
   four-byte aligned. A zero-byte request reserves one byte; exhaustion returns
   null. free accepts null and releases a block returned by this allocator. */
void *malloc(size_t n);
void free(void *p);

/** Allocate n elements of size bytes and clear their product bytes. Overflow
   or arena exhaustion returns null; zero-sized requests follow malloc's rule. */
void *calloc(size_t n, size_t size);

/** Resize a runtime-arena block. Shrinking or keeping the current size returns
   the same pointer; growth preserves the previous allocation's contents and
   may move it. A zero size frees the block and returns null. On failure, the
   original block remains allocated. */
void *realloc(void *p, size_t n);

/** Search n sorted elements of size bytes. compare(key, element) must define a
   consistent ordering. Return a matching element or null when none exists. */
void *bsearch(const void *key, const void *base, size_t n, size_t size,
        int (*compare)(const void *, const void *));

/** Sort n elements of size bytes in place according to compare(left, right),
   which must define a consistent ordering. */
void qsort(void *base, size_t n, size_t size,
        int (*compare)(const void *, const void *));

#endif
