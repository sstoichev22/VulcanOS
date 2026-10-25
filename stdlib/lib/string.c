#include <string.h>

void *memcpy(void *destination, const void *source, size_t count) {
    char *output = destination;
    const char *input = source;
    while (count--) *output++ = *input++;
    return destination;
}

void *memmove(void *destination, const void *source, size_t count) {
    char *output = destination;
    const char *input = source;
    if (output > input) {
        while (count) {
            count--;
            output[count] = input[count];
        }
    } else {
        while (count--) *output++ = *input++;
    }
    return destination;
}

void *memset(void *destination, int value, size_t count) {
    char *output = destination;
    while (count--) *output++ = value;
    return destination;
}

int memcmp(const void *left, const void *right, size_t count) {
    const char *first = left;
    const char *second = right;
    while (count--) {
        if (*first != *second) return *first - *second;
        first++;
        second++;
    }
    return 0;
}

void *memchr(const void *memory, int value, size_t count) {
    const char *current = memory;
    while (count--) {
        if (*current == (char)value) return (void *)current;
        current++;
    }
    return (void *)0;
}

size_t strlen(const char *string) {
    size_t length = 0;
    while (string[length]) length++;
    return length;
}

char *strcpy(char *destination, const char *source) {
    char *output = destination;
    while ((*output++ = *source++)) {}
    return destination;
}

char *strncpy(char *destination, const char *source, size_t count) {
    size_t index = 0;
    while (index < count && source[index]) {
        destination[index] = source[index];
        index++;
    }
    while (index < count) destination[index++] = 0;
    return destination;
}

char *strcat(char *destination, const char *source) {
    strcpy(destination + strlen(destination), source);
    return destination;
}

char *strncat(char *destination, const char *source, size_t count) {
    char *output = destination + strlen(destination);
    while (count-- && *source) *output++ = *source++;
    *output = 0;
    return destination;
}

int strcmp(const char *left, const char *right) {
    while (*left && *left == *right) {
        left++;
        right++;
    }
    return *left - *right;
}

int strncmp(const char *left, const char *right, size_t count) {
    while (count--) {
        if (*left != *right) return *left - *right;
        if (!*left) return 0;
        left++;
        right++;
    }
    return 0;
}

char *strchr(const char *string, int value) {
    do {
        if (*string == (char)value) return (char *)string;
    } while (*string++);
    return (char *)0;
}

char *strrchr(const char *string, int value) {
    char *lastMatch = (char *)0;
    do {
        if (*string == (char)value) lastMatch = (char *)string;
    } while (*string++);
    return lastMatch;
}

char *strstr(const char *string, const char *needle) {
    size_t length = strlen(needle);
    if (!length) return (char *)string;
    while (*string) {
        if (!strncmp(string, needle, length)) return (char *)string;
        string++;
    }
    return (char *)0;
}

size_t strspn(const char *string, const char *accepted) {
    size_t length = 0;
    while (string[length] && strchr(accepted, string[length])) length++;
    return length;
}

size_t strcspn(const char *string, const char *rejected) {
    size_t length = 0;
    while (string[length] && !strchr(rejected, string[length])) length++;
    return length;
}

char *strpbrk(const char *string, const char *accepted) {
    while (*string) {
        if (strchr(accepted, *string)) return (char *)string;
        string++;
    }
    return (char *)0;
}

static char *nextToken;

char *strtok(char *string, const char *delimiters) {
    if (string) nextToken = string;
    if (!nextToken) return (char *)0;

    nextToken += strspn(nextToken, delimiters);
    if (!*nextToken) return (char *)0;
    char *token = nextToken;
    nextToken += strcspn(nextToken, delimiters);
    if (*nextToken) *nextToken++ = 0;
    else nextToken = (char *)0;
    return token;
}
