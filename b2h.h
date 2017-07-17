#pragma once
#include <cstdint>
#include <cstring>
#include <emmintrin.h>

using u64=uint64_t;
union m128 {
    __m128i v;
    uint64_t q[2];
    char b[16];
    m128(u64 hi, u64 lo) : q{hi, lo} {};
    m128(const char b[16]) {
        std::memcpy(this->b, b, 16);
    }
};

void bin2hex(u64 bin, char* hex);

void bin2hex2(u64 bin, char* hex);
