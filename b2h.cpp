#include "b2h.h"

#include <tmmintrin.h>
#include <cstring>

void bin2hex(u64 bin, char* hex) {
    const char digs[] = "0123456789abcdef";
    hex[16] = 0;
    
    for(int i = 15; i>= 0 ; i--) {
        hex[i] = digs[bin & 0xf];
        bin >>= 4;
    }
}

void bin2hex2(u64 bin, char* hex) {
    m128 x(bin >> 4, bin);
    m128 mask(0x0f0f0f0f0f0f0f0full, 0x0f0f0f0f0f0f0f0full);
    m128 digs("0123456789abcdef");
    char order[16] = {7, 15, 6, 14, 5, 13, 4, 12, 3, 11, 2, 10, 1, 9, 0, 8};
    x.v = _mm_and_si128(x.v, mask.v);
    x.v = _mm_shuffle_epi8(digs.v, x.v);
    x.v = _mm_shuffle_epi8(x.v, m128(order).v);
    hex[16] = 0;
    std::memcpy(hex, &x, 16);
}
