#include <cstdint>
#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <ctime>
#include <ratio>
#include <chrono>

#include "b2h.h"

void bench(u64 bin) {
    using namespace std::chrono;
    using namespace std;
    char out[1025];
    char out2[1025];

    auto t1 = high_resolution_clock::now();
    u64 x = bin;
    for (int i = 0; i < 1E6; i++) {
        bin2hex(x, out + i * 16 % 1024);
        x += 0x0123456789abcdefull;
    }
    auto t2 = high_resolution_clock::now();
    x = bin;
    for (int i = 0; i < 1E6; i++) {
        bin2hex2(x, out2 + i * 16 % 1024);
        x += 0x0123456789abcdefull;
    }
    auto t3 = high_resolution_clock::now();
    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
    duration<double> time_span2 = duration_cast<duration<double>>(t3 - t2);
    cout << "bin2hex: " << time_span1.count()*1000 << " nanoseconds" << endl;
    cout << "bin2hex2: " << time_span2.count()*1000 << " nanoseconds" << endl;
    cout << "out: " << out << endl;
    cout << "out2: " << out2 << endl;
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        char hex[17];
        char hex2[17];
        u64 bin = static_cast<u64>(strtoll(argv[i], nullptr, 16));
        bin2hex(bin, hex);
        bin2hex2(bin, hex2);
        printf("%llx: hex %s, hex2 %s\n", bin, hex, hex2);
        bench(bin);
    }
    return 0;
}
