#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <random>

using vec = std::vector<int64_t>;

std::mt19937_64 gen64;

vec makeVec(int64_t size) {
    vec result(size);
    for (auto& elem : result)
        elem = gen64();
    return result;
}

std::string showVec(const vec& v) {
    std::string result = "[";
    for (auto elem : v)
        result += " " + std::to_string(elem);
    result += " ]";
    return result;
}

template<class ForwardIt>
void sort(ForwardIt first, ForwardIt last) {
    std::for_each(first, last, [first, last](auto& left){
        std::for_each(first, last, [left](auto& right){
            if (left < right) std::swap(left, right);
        });
    });
}
 
int main() {
    vec v = makeVec(5);
    std::cout << "random: " << showVec(v) << "\n";
    sort(v.begin(), v.end());
    std::cout << "sorted: " << showVec(v) << "\n";
    return 0;
}
