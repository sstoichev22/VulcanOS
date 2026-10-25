#ifndef VULCANOS_HEAP_H
#define VULCANOS_HEAP_H

/** Turns [start, start + size) into one free block, aligning the start; an unusable range leaves the heap empty. */
void heap_init(void *start, uint64 size);

/** Allocates an aligned block of at least size bytes using first fit, splitting larger blocks; returns null for size 0 or when full. */
void *kmalloc(uint64 size);

/** Frees a block returned by kmalloc and merges adjacent free blocks; null and unknown pointers are ignored. */
void kfree(void *ptr);

/** Allocates count * size zeroed bytes; returns null on overflow, zero size, or exhaustion. */
void *kcalloc(uint64 count, uint64 size);

/** Resizes a block, keeping its contents; null ptr allocates, size 0 frees and returns null, failure keeps the old block. */
void *krealloc(void *ptr, uint64 size);

#endif
