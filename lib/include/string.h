#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

// Set `nbytes` of `src` to `val`.
void memset(void *src, uint8_t val, size_t nbytes);

// Copy `nbytes` of `src` to `dest`.
void memcpy(void *restrict dest, const void *restrict src, size_t nbytes);

#endif
