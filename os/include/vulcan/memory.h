#ifndef VULCANOS_MEMORY_H
#define VULCANOS_MEMORY_H

/** First byte of the memory-mapped framebuffer. */
#define VULCAN_VRAM_START 0xF0000000

/** Kernel heap range handed to heap_init at boot: 2 MiB starting at 2 MiB. */
#define VULCAN_HEAP_START 0x00200000
#define VULCAN_HEAP_SIZE 0x00200000

#endif
