#include "b2d.h"

#include <emmintrin.h>
#include <tmmintrin.h>
#include <cstring>
#include <utility>
#include <cstdio>

union m128 {
    __m128i v;
    u64 q[2];
    u32 p[4]; 
    char b[16];
    m128(__m128i v) : v(v) {}
    m128(u64 hi, u64 lo) : q{hi, lo} { }
    m128(const char b[16]) {
        std::memcpy(this->b, b, 16);
    }
};

void bin2dec(u64 bin, char* dec) {
    // sprintf(dec, "%llu", bin); return;
    char* endp = dec;
    do {
        *endp++ = bin % 10 + '0';
        bin /= 10;
    } while (bin);

    *endp-- = 0;
    while (dec < endp)
        std::swap(*dec++, *endp--);
}

u32 div10(u32 x) {
    if (x > 99999)
        return 0;
    return x / 10;
}

void bin2dec2(u64 bin, char* dec) {
    m128 x(0, 0);
    x.p[0] = bin / 1'00000'00000'00000ull;
    bin %= 1'00000'00000'00000ull;
    x.p[1] = bin / 1'00000'00000ull;
    bin %= 1'00000'00000ull;
    
    x.p[2] = bin / 1'00000;
    x.p[3] = bin % 1'00000;

    char d[20];
    // memset(d, '_', 20);
    // d[20]=0;
    for (int j = 4; j >= 0; j--) {
        for (int i = 3; i >= 0; i--) {
            d[i * 5 + j] = x.p[i] % 10 + '0';
            x.p[i] /= 10;
        }
    // printf("%20s\n", d);
    }

    int i = 0;
    for (; i < 19; i++)
        if (d[i] != '0') break;

    for (; i < 20; i++)
        *dec++ = d[i];
    *dec = 0;
}
    
/*
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
*/



