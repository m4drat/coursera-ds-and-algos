#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

uint64_t CalculatePisanoPeriod(uint64_t t_Modulus) {
    uint64_t prev{ 0 };
    uint64_t current{ 1 };
    uint64_t pisanoPeriod{ 0 };

    for (uint32_t i = 0; i <= t_Modulus * t_Modulus; i++) {
        uint64_t old = current;
        current = (prev + current) % t_Modulus;
        prev = old;
        pisanoPeriod++;

        if (prev == 0 && current == 1) {
            return pisanoPeriod;
        }
    }
    
    return -1;
}

uint8_t FibonacciSumLastDigit(uint64_t t_N) {
    constexpr uint64_t c_Modulus { 10 };

    uint64_t pisanoPeriod = CalculatePisanoPeriod(c_Modulus);
    uint64_t prev{ 0 };
    uint64_t current{ 1 };
    uint8_t sum{ 1 };

    if (t_N % pisanoPeriod == 0) {
        return 0;
    } else if (t_N % pisanoPeriod == 1) {
        return 1;
    }

    for (uint32_t i = 2; i <= t_N % pisanoPeriod; i++) {
        uint64_t old = current;
        current = (prev + current) % c_Modulus;
        prev = old;

        sum = (sum + current) % c_Modulus;
    }

    return sum;
}

int main(int argc, char* argv[]) {
    uint64_t n;
    std::cin >> n;
    std::cout << (uint32_t)FibonacciSumLastDigit(n) << std::endl;

    return 0;
}