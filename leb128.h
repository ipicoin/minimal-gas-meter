#ifndef LEB128_H
#define LEB128_H

#include <stddef.h>
#include <stdint.h>

#define INLINE inline

/*
 * int32LEB128MaxByteCount is the maximum number of bytes a 32-bit integer
 * (signed or unsigned) may be encoded as. From
 * https://webassembly.github.io/spec/core/binary/values.html#binary-int:
 *
 * "the total number of bytes encoding a value of type uN must not exceed ceil(N/7) bytes"
 * "the total number of bytes encoding a value of type sN must not exceed ceil(N/7) bytes"
 */
#define int32LEB128MaxByteCount 5

/*
 * leb128Readuint32_t reads and decodes an unsigned 32-bit integer
 * from the given buffer and stores the value in result.
 * Returns the number of read bytes.
 */
static
INLINE
size_t
leb128ReadU32(
  uint8_t** p,
  uint8_t* end,
  uint32_t* result
) {
  uint32_t value = 0;
  uint32_t shift = 0;
  size_t count = 0;
  uint8_t byte = 0;

  /* Only read up to maximum number of bytes */
  while (count < int32LEB128MaxByteCount && *p < end) {
    byte = *(*p)++;
    count++;
    value |= ((uint32_t) (byte & 0x7F)) << shift;
    shift += 7;
    /* Check the high order bit of byte */
    if ((byte & 0x80) == 0) {
        break;
    }
  }

  *result = value;

  return count;
}

/*
 * leb128Readint32_t reads and decodes a signed 32-bit integer
 * from the given buffer and stores the value in result.
 * Returns the number of read bytes.
 */
static
INLINE
size_t
leb128ReadI32(
  uint8_t** p,
  uint8_t* end,
  int32_t* result
) {
  int32_t value = 0;
  uint32_t shift = 0;
  size_t count = 0;
  uint8_t byte = 0;

  /* Only read up to maximum number of bytes */
  while (count < int32LEB128MaxByteCount && *p < end) {
    byte = *(*p)++;
    count++;
    value |= (int32_t) (((uint32_t) (byte & 0x7F)) << shift);
    shift += 7;
    /* Check the high order bit of byte */
    if ((byte & 0x80) == 0) {
        break;
    }
  }

  if ((shift < 8 * sizeof(int32_t)) && (byte & 0x40)) {
      value |= -((int32_t) 1 << shift);
  }

  *result = value;

  return count;
}

/*
 * int64LEB128MaxByteCount is the maximum number of bytes a 64-bit integer
 * (signed or unsigned) may be encoded as. From
 * https://webassembly.github.io/spec/core/binary/values.html#binary-int:
 *
 * "the total number of bytes encoding a value of type uN must not exceed ceil(N/7) bytes"
 * "the total number of bytes encoding a value of type sN must not exceed ceil(N/7) bytes"
 */
#define int64LEB128MaxByteCount 10

/*
 * leb128Readuint64_t reads and decodes an unsigned 32-bit integer
 * from the given buffer and stores the value in result.
 * Returns the number of read bytes.
 */
static
INLINE
size_t
leb128ReadU64(
    uint8_t** p,
    uint8_t* end,
    uint64_t* result
) {
  uint64_t value = 0;
  uint64_t shift = 0;
  size_t count = 0;
  uint8_t byte = 0;

  /* Only read up to maximum number of bytes */
  while (count < int64LEB128MaxByteCount && *p < end) {
    byte = *(*p)++;
    count++;
    value |= ((uint64_t) (byte & 0x7F)) << shift;
    shift += 7;
    /* Check the high order bit of byte */
    if ((byte & 0x80) == 0) {
        break;
    }
  }

  *result = value;

  return count;
}

/*
 * leb128Readint64_t reads and decodes a signed 32-bit integer
 * from the given buffer and stores the value in result.
 * Returns the number of read bytes.
 */
static
INLINE
size_t
leb128ReadI64(
    uint8_t** p,
    uint8_t* end,
    int64_t* result
) {
  int64_t value = 0;
  uint64_t shift = 0;
  size_t count = 0;
  uint8_t byte = 0;
  /* Only read up to maximum number of bytes */
  while (count < int64LEB128MaxByteCount && *p < end) {
    byte = *(*p)++;
    count++;
    value |= (int64_t) (((uint64_t) (byte & 0x7F)) << shift);
    shift += 7;
    /* Check the high order bit of byte */
    if ((byte & 0x80) == 0) {
        break;
    }
  }

  if ((shift < 8 * sizeof(int64_t)) && (byte & 0x40)) {
    value |= -((int64_t) 1 << shift);
  }

  *result = value;

  return count;
}

static INLINE size_t leb128WriteU32(uint8_t** dest, uint32_t value) {
  uint8_t* start = *dest;
  uint8_t out = value & 0x7f;
  value >>= 7;
  while (value != 0) {
    *(*dest)++ = out | 0x80;
    out = value & 0x7f;
    value >>= 7;
  }
  *(*dest)++ = out;
  return *dest - start;
}

static INLINE size_t leb128WriteU64(uint8_t** dest, uint64_t value) {
  uint8_t* start = *dest;
  uint8_t out = value & 0x7f;
  value >>= 7;
  while (value != 0) {
    *(*dest)++ = out | 0x80;
    out = value & 0x7f;
    value >>= 7;
  }
  *(*dest)++ = out;
  return *dest - start;
}

static INLINE size_t leb128WriteS32(uint8_t** dest, int32_t value) {
  uint8_t* start = *dest;
  uint32_t extra_bits = (uint32_t)(value ^ (value >> 31)) >> 6;
  uint8_t out = value & 0x7f;
  while (extra_bits != 0u) {
    *(*dest)++ = out | 0x80;
    value >>= 7;
    out = value & 0x7f;
    extra_bits >>= 7;
  }
  *(*dest)++ = out;
  return *dest - start;
}

static INLINE size_t leb128WriteS64(uint8_t** dest, int64_t value) {
  uint8_t* start = *dest;
  uint32_t extra_bits = (uint64_t)(value ^ (value >> 63)) >> 6;
  uint8_t out = value & 0x7f;
  while (extra_bits != 0u) {
    *(*dest)++ = out | 0x80;
    value >>= 7;
    out = value & 0x7f;
    extra_bits >>= 7;
  }
  *(*dest)++ = out;
  return *dest - start;
}

static INLINE size_t leb128SizeU32(uint32_t value) {
  size_t count = 1;
  value >>= 7;
  while (value != 0) {
    count++;
    value >>= 7;
  }
  return count;
}

static INLINE size_t leb128SizeU64(uint64_t value) {
  size_t count = 1;
  value >>= 7;
  while (value != 0) {
    count++;
    value >>= 7;
  }
  return count;
}

static INLINE size_t leb128SizeS32(int32_t value) {
  uint32_t extra_bits = (uint32_t)(value ^ (value >> 31)) >> 6;
  size_t count = 1;
  while (extra_bits != 0u) {
    count ++;
    value >>= 7;
    extra_bits >>= 7;
  }
  return count;
}

static INLINE size_t leb128SizeS64(int64_t value) {
  uint32_t extra_bits = (uint64_t)(value ^ (value >> 63)) >> 6;
  size_t count = 1;
  while (extra_bits != 0u) {
    count ++;
    value >>= 7;
    extra_bits >>= 7;
  }
  return count;
}



#endif /* W2C2_LEB128_H */
