#include "vulcan/heap.h"
#include <limits.h>

/** Header placed before every heap block: payload size, next block, and whether it is free. */
typedef struct Block {
    uint64 size;
    struct Block *next;
    bool free;
} Block;

#define HEAP_ALIGNMENT 8U
#define HEAP_ALIGNMENT_MASK (HEAP_ALIGNMENT - 1U)

static Block *heap_head = 0;

static void heap_merge(void) {
    Block *current = heap_head;
    while (current && current->next) {
        Block *next = current->next;
        uint8 *currentEnd = (uint8 *)(current + 1) + current->size;
        if (current->free && next->free && currentEnd == (uint8 *)next) {
            current->size += sizeof(Block) + next->size;
            current->next = next->next;
        } else {
            current = next;
        }
    }
}

void heap_init(void *start, uint64 size) {
    heap_head = 0;
    if (!start || size <= sizeof(Block)) return;

    uint64 startAddress = (uint64)start;
    if (size > UINT64_MAX - startAddress) return;
    uint64 alignmentOffset = (HEAP_ALIGNMENT - (startAddress & HEAP_ALIGNMENT_MASK))
            & HEAP_ALIGNMENT_MASK;
    if (startAddress > UINT64_MAX - alignmentOffset || size <= alignmentOffset + sizeof(Block)) return;

    heap_head = (Block *)(startAddress + alignmentOffset);
    heap_head->size = size - alignmentOffset - sizeof(Block);
    heap_head->next = 0;
    heap_head->free = 1;
}

void *kmalloc(uint64 size) {
    if (!size || size > UINT64_MAX - HEAP_ALIGNMENT_MASK) return 0;
    size = (size + HEAP_ALIGNMENT_MASK) & ~((uint64)HEAP_ALIGNMENT_MASK);

    Block *current = heap_head;
    while (current) {
        if (current->free && current->size >= size) {
            if (current->size - size >= sizeof(Block) + HEAP_ALIGNMENT) {
                Block *next = (Block *)((uint8 *)(current + 1) + size);
                next->size = current->size - size - sizeof(Block);
                next->next = current->next;
                next->free = 1;
                current->size = size;
                current->next = next;
            }
            current->free = 0;
            return (void *)(current + 1);
        }
        current = current->next;
    }
    return 0;
}

void kfree(void *ptr) {
    if (!ptr) return;

    for (Block *current = heap_head; current; current = current->next) {
        if ((uint8 *)(current + 1) == (uint8 *)ptr) {
            current->free = 1;
            heap_merge();
            return;
        }
    }
}

void *kcalloc(uint64 count, uint64 size) {
    if (!count || !size || count > UINT64_MAX / size) return 0;

    uint64 totalBytes = count * size;
    uint8 *memory = (uint8 *)kmalloc(totalBytes);
    if (!memory) return 0;
    for (uint64 index = 0; index < totalBytes; index++) memory[index] = 0;
    return memory;
}

void *krealloc(void *ptr, uint64 size) {
    if (!ptr) return kmalloc(size);
    if (!size) {
        kfree(ptr);
        return 0;
    }

    Block *block = heap_head;
    while (block && (uint8 *)(block + 1) != (uint8 *)ptr) block = block->next;
    if (!block || block->free) return 0;
    if (block->size >= size) return ptr;

    uint8 *replacement = (uint8 *)kmalloc(size);
    if (!replacement) return 0;
    uint8 *source = (uint8 *)ptr;
    for (uint64 index = 0; index < block->size; index++) replacement[index] = source[index];
    kfree(ptr);
    return replacement;
}
