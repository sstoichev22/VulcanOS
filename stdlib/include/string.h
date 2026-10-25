#ifndef VULCAN_STRING_H
#define VULCAN_STRING_H

typedef unsigned long size_t;

/** Copy count bytes; source and destination must not overlap. Return destination. */
void *memcpy(void *destination, const void *source, size_t count);

/** Copy count bytes with overlap allowed. Return destination. */
void *memmove(void *destination, const void *source, size_t count);

/** Fill count bytes with the unsigned-char conversion of value. Return destination. */
void *memset(void *destination, int value, size_t count);

/** Compare at most count bytes; return zero or a value with the first differing byte's sign. */
int memcmp(const void *left, const void *right, size_t count);

/** Find the first byte matching value among count bytes, or return null. */
void *memchr(const void *memory, int value, size_t count);

/** Return the number of non-null bytes before the terminator; string must be null-terminated. */
size_t strlen(const char *string);

/** Copy through the terminator. Source and destination must not overlap. Return destination. */
char *strcpy(char *destination, const char *source);

/** Copy at most count bytes and pad with nulls; may not terminate when source length >= count. Return destination. */
char *strncpy(char *destination, const char *source, size_t count);

/** Append source including its terminator; destination must have enough space and both strings terminate. */
char *strcat(char *destination, const char *source);

/** Append at most count non-null bytes and always terminate; destination must have enough space. Return destination. */
char *strncat(char *destination, const char *source, size_t count);

/** Compare terminated strings through the first mismatch; return zero or the mismatch's sign. */
int strcmp(const char *left, const char *right);

/** Compare at most count bytes; return zero or the first mismatch's sign. */
int strncmp(const char *left, const char *right, size_t count);

/** Return a pointer to the first match, including the terminator when value is zero, or null. */
char *strchr(const char *string, int value);

/** Return a pointer to the last match, including the terminator when value is zero, or null. */
char *strrchr(const char *string, int value);

/** Return the first occurrence of needle; an empty needle matches at string, and no match returns null. */
char *strstr(const char *string, const char *needle);

/** Return the leading length containing only bytes from accepted. */
size_t strspn(const char *string, const char *accepted);

/** Return the leading length containing no bytes from rejected. */
size_t strcspn(const char *string, const char *rejected);

/** Find the first byte from accepted, or return null. */
char *strpbrk(const char *string, const char *accepted);

/** Modify string to split at delimiters; use one saved token state, continued by null input and reset by new input. */
char *strtok(char *string, const char *delimiters);

#endif
