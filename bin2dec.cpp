#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>


#include <ctime>
#include <ratio>
#include <chrono>

#include "b2d.h"

void bench(u64 bin) {
    using namespace std::chrono;
    using namespace std;
    char out[1001] = {' '};
    char out2[1001] = {' '};

    auto t1 = high_resolution_clock::now();
    u64 x = bin;
    const u64 iterations = 1E6;
    for (int i = 0; i < iterations; i++) {
        bin2dec(x, out + i * 20 % 1000);
        x += 0x0123456789abcdefull;
    }
    auto t2 = high_resolution_clock::now();
    x = bin;
    for (int i = 0; i < iterations; i++) {
        bin2dec2(x, out2 + i * 20 % 1000);
        x += 0x0123456789abcdefull;
    }
    auto t3 = high_resolution_clock::now();
    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
    duration<double> time_span2 = duration_cast<duration<double>>(t3 - t2);
    cout << "bin2hex: " << time_span1.count()*(1E9 / iterations) << " nanoseconds" << endl;
    cout << "bin2hex2: " << time_span2.count()*(1E9 / iterations) << " nanoseconds" << endl;
    if (memcmp(out, out2, sizeof(out))) {
        for (int i = 0; i < sizeof(out) - 1; i++) {
            if (!out[i]) out[i] = '_';
            if (!out2[i]) out2[i] = '_';
        }
        cerr << "Miscompare: " << out << " != " << out2 << "!\n";
        std::terminate();
    }
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        char dec[21];
        char dec2[21];
        u64 bin = static_cast<u64>(strtoll(argv[i], nullptr, 10));
        bin2dec(bin, dec);
        bin2dec2(bin, dec2);
        printf("%lld: dec %s, dec %s\n", bin, dec, dec2);
        bench(bin);
    }
    return 0;
}
