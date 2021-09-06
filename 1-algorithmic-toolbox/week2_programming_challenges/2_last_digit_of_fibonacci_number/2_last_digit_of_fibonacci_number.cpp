#include <vector>
#include <iostream>
#include <algorithm>

uint8_t Fibonacci(uint32_t t_N) {
    std::vector<uint8_t> fibNumbers { 0, 1 };
    fibNumbers.reserve(t_N + 1);

    for (uint32_t i = 2; i <= t_N; i++) {
        fibNumbers[i] = (fibNumbers[i - 1] + fibNumbers[i - 2]) % 10;
    }

    return fibNumbers[t_N];
}

int main(int argc, char* argv[]) {
    int32_t n;
    std::cin >> n;
    std::cout << (uint32_t)Fibonacci(n) << std::endl;

    return 0;
}