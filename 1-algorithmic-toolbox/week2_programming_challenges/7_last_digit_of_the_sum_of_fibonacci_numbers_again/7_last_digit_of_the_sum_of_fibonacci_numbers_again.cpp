#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

// uint64_t CalculatePisanoPeriod(uint64_t t_Modulus) {
//     uint64_t prev{ 0 };
//     uint64_t current{ 1 };
//     uint64_t pisanoPeriod{ 0 };

//     for (uint32_t i = 0; i <= t_Modulus * t_Modulus; i++) {
//         uint64_t old = current;
//         current = (prev + current) % t_Modulus;
//         prev = old;
//         pisanoPeriod++;

//         if (prev == 0 && current == 1) {
//             return pisanoPeriod;
//         }
//     }
    
//     return -1;
// }

constexpr int64_t c_Modulus { 10 };

// Known constant
constexpr uint64_t c_PisanoPeriod = 60; // CalculatePisanoPeriod(c_Modulus);

int64_t FibonacciSumLastDigit(uint64_t t_N) {
    uint64_t prev{ 0 };
    uint64_t current{ 1 };
    int64_t sum{ 1 };

    if (t_N % c_PisanoPeriod == 0) {
        return 0;
    } else if (t_N % c_PisanoPeriod == 1) {
        return 1;
    }

    for (uint32_t i = 2; i <= t_N % c_PisanoPeriod; i++) {
        uint64_t old = current;
        current = (prev + current) % c_Modulus;
        prev = old;

        sum = (sum + current) % c_Modulus;
    }

    return sum;
}

int64_t FibonacciPartialSumLastDigit(uint64_t t_M, uint64_t t_N) {
    if (t_M == 0) {
        return FibonacciSumLastDigit(t_N);
    }

    return FibonacciSumLastDigit(t_N) - FibonacciSumLastDigit(t_M - 1);
}

int main(int argc, char* argv[]) {
    uint64_t m, n;
    std::cin >> m >> n;
    std::cout << ((FibonacciPartialSumLastDigit(m, n) % c_Modulus) + c_Modulus) % c_Modulus << std::endl;

    return 0;
}