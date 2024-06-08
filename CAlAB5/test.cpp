#include <iostream>
#include <immintrin.h>

int main() {
    if (__builtin_cpu_supports("avx")) {
        std::cout << "AVX is supported on this CPU." << std::endl;
    } else {
        std::cout << "AVX is not supported on this CPU." << std::endl;
    }
    return 0;
}