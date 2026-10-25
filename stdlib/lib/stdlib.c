#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALLOCATOR_ARENA_SIZE 32768U
#define ALLOCATOR_HEADER_SIZE 8U
#define ALLOCATOR_ALIGNMENT 4U

div_t div(int numerator, int denominator) {
    div_t result;
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;
    return result;
}

ldiv_t ldiv(long numerator, long denominator) {
    ldiv_t result;
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;
    return result;
}

int abs(int n) {
    return n < 0 ? -n : n;
}

long labs(long n) {
    return n < 0 ? -n : n;
}

unsigned long strtoul(const char *s, char **end, int base) {
    const char *start = s;
    while (isspace(*s)) s++;

    int negative = 0;
    if (*s == '-' || *s == '+') {
        negative = *s == '-';
        s++;
    }

    if (base == 0) {
        base = 10;
        if (*s == '0') {
            base = 8;
            if (s[1] == 'x' || s[1] == 'X') base = 16;
        }
    }

    if (base == 16 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X') && isxdigit(s[2])) s += 2;
    if (base < 2 || base > 36) {
        if (end) *end = (char *)start;
        return 0;
    }

    unsigned long result = 0;
    int any = 0;
    while (*s) {
        int digit = isdigit(*s) ? *s - '0' : tolower(*s) - 'a' + 10;
        if (digit < 0 || digit >= base) break;
        result = result * base + digit;
        s++;
        any = 1;
    }

    if (end) *end = (char *)(any ? s : start);
    return negative ? -result : result;
}

long strtol(const char *s, char **end, int base) {
    return (long)strtoul(s, end, base);
}

int atoi(const char *s) {
    return (int)strtol(s, (char **)0, 10);
}

long atol(const char *s) {
    return strtol(s, (char **)0, 10);
}

double strtod(const char *s, char **end) {
    const char *start = s;
    while (isspace(*s)) s++;

    int negative = 0;
    if (*s == '-' || *s == '+') {
        negative = *s == '-';
        s++;
    }

    if (tolower(s[0]) == 'i' && tolower(s[1]) == 'n' && tolower(s[2]) == 'f') {
        s += 3;
        if (tolower(s[0]) == 'i' && tolower(s[1]) == 'n' && tolower(s[2]) == 'i'
                && tolower(s[3]) == 't' && tolower(s[4]) == 'y') s += 5;
        if (end) *end = (char *)s;
        double infinity = 1.0 / 0.0;
        return negative ? -infinity : infinity;
    }

    if (tolower(s[0]) == 'n' && tolower(s[1]) == 'a' && tolower(s[2]) == 'n') {
        s += 3;
        if (*s == '(') {
            const char *payload = s + 1;
            while (isalnum(*payload) || *payload == '_') payload++;
            if (*payload == ')') s = payload + 1;
        }
        if (end) *end = (char *)s;
        return 0.0 / 0.0;
    }

    int radix = 10;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')
            && (isxdigit(s[2]) || (s[2] == '.' && isxdigit(s[3])))) {
        radix = 16;
        s += 2;
    }

    double result = 0.0;
    double fraction = 1.0;
    int any = 0;
    int point = 0;
    while (*s) {
        if (*s == '.' && !point) {
            point = 1;
            s++;
            continue;
        }

        int digit = isdigit(*s) ? *s - '0' : tolower(*s) - 'a' + 10;
        if (digit < 0 || digit >= radix) break;
        if (point) {
            fraction /= radix;
            result += digit * fraction;
        } else {
            result = result * radix + digit;
        }
        any = 1;
        s++;
    }

    if (!any) {
        if (end) *end = (char *)start;
        return 0.0;
    }

    int marker = radix == 16 ? 'p' : 'e';
    if (tolower(*s) == marker) {
        const char *exponentStart = s;
        int exponentNegative = 0;
        int exponent = 0;
        s++;
        if (*s == '+' || *s == '-') {
            exponentNegative = *s == '-';
            s++;
        }

        if (!isdigit(*s)) {
            s = exponentStart;
        } else {
            while (isdigit(*s)) {
                if (exponent < 4096) exponent = exponent * 10 + *s - '0';
                s++;
            }

            double scale = radix == 16 ? 2.0 : 10.0;
            while (exponent--) {
                if (exponentNegative) result /= scale;
                else result *= scale;
            }
        }
    }

    if (end) *end = (char *)s;
    return negative ? -result : result;
}

float strtof(const char *s, char **end) {
    return (float)strtod(s, end);
}

double atof(const char *s) {
    return strtod(s, (char **)0);
}

static unsigned int random_state = 1;

void srand(unsigned int seed) {
    random_state = seed;
}

int rand(void) {
    random_state = random_state * 1103515245U + 12345U;
    return (int)((random_state >> 16) & 32767);
}

static unsigned char *arena = (unsigned char *)0x0000010000000000UL;
static unsigned int used = 0;

void *malloc(size_t n) {
    if (n == 0) n = 1;
    if (n > ALLOCATOR_ARENA_SIZE - ALLOCATOR_HEADER_SIZE) return (void *)0;
    n = (n + ALLOCATOR_ALIGNMENT - 1) & ~(ALLOCATOR_ALIGNMENT - 1);

    unsigned int offset = 0;
    while (offset < used) {
        unsigned int *header = (unsigned int *)(arena + offset);
        if (header[1] == 0 && header[0] >= n) {
            header[1] = 1;
            return arena + offset + ALLOCATOR_HEADER_SIZE;
        }
        offset += header[0] + ALLOCATOR_HEADER_SIZE;
    }

    if (n + ALLOCATOR_HEADER_SIZE > ALLOCATOR_ARENA_SIZE - used) return (void *)0;
    unsigned int *header = (unsigned int *)(arena + used);
    header[0] = n;
    header[1] = 1;
    void *result = arena + used + ALLOCATOR_HEADER_SIZE;
    used += n + ALLOCATOR_HEADER_SIZE;
    return result;
}

void free(void *p) {
    if (p) {
        unsigned int *header = (unsigned int *)p - 2;
        header[1] = 0;
    }
}

void *calloc(size_t n, size_t size) {
    if (size && n > (ALLOCATOR_ARENA_SIZE - ALLOCATOR_HEADER_SIZE) / size) return (void *)0;
    void *p = malloc(n * size);
    if (p) memset(p, 0, n * size);
    return p;
}

void *realloc(void *p, size_t n) {
    if (!p) return malloc(n);
    if (!n) {
        free(p);
        return (void *)0;
    }

    unsigned int *header = (unsigned int *)p - 2;
    if (header[0] >= n) return p;

    void *replacement = malloc(n);
    if (replacement) {
        memcpy(replacement, p, header[0]);
        free(p);
    }
    return replacement;
}

void *bsearch(const void *key, const void *base, size_t n, size_t size,
        int (*compare)(const void *, const void *)) {
    const char *data = base;
    while (n) {
        size_t middle = n / 2;
        const char *element = data + middle * size;
        int order = compare(key, element);
        if (order == 0) return (void *)element;
        if (order < 0) {
            n = middle;
        } else {
            data = element + size;
            n -= middle + 1;
        }
    }
    return (void *)0;
}

void qsort(void *base, size_t n, size_t size, int (*compare)(const void *, const void *)) {
    char *data = base;
    for (size_t i = 1; i < n; i++) {
        size_t j = i;
        while (j && compare(data + (j - 1) * size, data + j * size) > 0) {
            for (size_t k = 0; k < size; k++) {
                char value = data[(j - 1) * size + k];
                data[(j - 1) * size + k] = data[j * size + k];
                data[j * size + k] = value;
            }
            j--;
        }
    }
}
