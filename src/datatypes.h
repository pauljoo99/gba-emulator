#pragma once

#include <cstdint>

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using I32 = int32_t;
using I64 = int64_t;

struct U32Ref {
public:
  inline U32Ref() : ptr(nullptr) {}
  inline U32Ref(U32 *ptr) : ptr(ptr) {}
  inline U32Ref(const U32Ref &other) = delete;
  inline U32Ref(const U32Ref &&other) = delete;
  inline operator U32 &() const { return *ptr; }
  inline operator U32 *() const { return ptr; }
  inline U32Ref &operator=(U32 rhs) {
    *ptr = rhs;
    return *this;
  }
  U32 *ptr;
};