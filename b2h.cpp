#include "b2h.h"

#include <emmintrin.h>
#include <tmmintrin.h>
#include <cstring>

union m128 {
    __m128i v;
    uint64_t q[2];
    char b[16];
    m128(__m128i v) : v(v) {}
    m128(u64 hi, u64 lo) : q{hi, lo} { }
    m128(const char b[16]) {
        std::memcpy(this->b, b, 16);
    }
};

void bin2hex(u64 bin, char* hex) {
    const char digs[] = "0123456789abcdef";
    hex[16] = 0;
     
    for(int i = 15; i >= 0 ; i--) {
        hex[i] = digs[bin & 0xf];
        bin >>= 4;
    }
}

void bin2hex2(u64 bin, char* hex) {
    // Set x to { bin >> 4, bin }.
    m128 x(bin, 0);
    m128 y(_mm_srli_epi32(x.v, 4));
    x.q[1] = y.q[0];

    // Mask out low order bits, so x.q[1] has odd nibbles, and x.q[0] has even.
    const m128 mask(0x0f0f0f0f0f0f0f0full, 0x0f0f0f0f0f0f0f0full);
    x.v = _mm_and_si128(x.v, mask.v);

    // Map 4-bit nibbles to hex digits.
    const m128 digs("0123456789abcdef");
    x.v = _mm_shuffle_epi8(digs.v, x.v);

    // Reorder all digits.
    const char order[16] = {15, 7, 14, 6, 13, 5, 12, 4, 11, 3, 10, 2, 9, 1, 8, 0};
    x.v = _mm_shuffle_epi8(x.v, m128(order).v);

    // Copy the result to the output, with a trailing 0
    hex[16] = 0;
    std::memcpy(hex, &x, 16);
}


