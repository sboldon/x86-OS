#include "string.h"
#include <stdint.h>
#include <stddef.h>

void memset(void *src, uint8_t val, size_t nbytes) {
  uint8_t *ptr = (uint8_t*)src;
  uint8_t *end = ptr + nbytes;
  while (ptr != end) { *ptr++ = val; }
}

void memcpy(void *restrict dest, const void *restrict src, size_t nbytes) {
  uint8_t *d = (uint8_t*)dest;
  uint8_t *s = (uint8_t*)src;
  uint8_t *end = s + nbytes;
  while (s != end) { *d++ = *s++; }
}
