#include "b2h.h"

#include <emmintrin.h>
#include <tmmintrin.h>
#include <cstring>

#include "iacaMarks.h"

#ifndef IACA
#undef IACA_START
#define IACA_START
#undef IACA_END
#define IACA_END
#else
#warning "Compiled with IACA instrumentation: not for execution"
#endif

union m128 {
    __m128i v;
    uint64_t q[2];
    char b[16];
    m128(u64 hi, u64 lo) : q{hi, lo} { }
    m128(const char b[16]) {
        std::memcpy(this->b, b, 16);
    }
};

void bin2hex(u64 bin, char* hex) {
   IACA_START
    const char digs[] = "0123456789abcdef";
    hex[16] = 0;
    
    for(int i = 15; i>= 0 ; i--) {
        hex[i] = digs[bin & 0xf];
        bin >>= 4;
    }
   IACA_END
}

void bin2hex2(u64 bin, char* hex) {
   IACA_START
    m128 x(bin, 0);
    m128 y(0, 0);
    y.v = _mm_srli_epi32(x.v, 4);
    x.q[1] = y.q[0];
    m128 mask(0x0f0f0f0f0f0f0f0full, 0x0f0f0f0f0f0f0f0full);
    m128 digs("0123456789abcdef");
    char order[16] = {15, 7, 14, 6, 13, 5, 12, 4, 11, 3, 10, 2, 9, 1, 8, 0};
    x.v = _mm_and_si128(x.v, mask.v);
    x.v = _mm_shuffle_epi8(digs.v, x.v);
    x.v = _mm_shuffle_epi8(x.v, m128(order).v);
    hex[16] = 0;
    std::memcpy(hex, &x, 16);
   IACA_END
}
