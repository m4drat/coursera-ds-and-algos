#include <vector>
#include <iostream>
#include <algorithm>

/* Alternative slow solution
uint32_t fibonacci(uint32_t t_N) {
    if (t_N <= 1) {
        return t_N;
    }

    return fibonacci(t_N - 1) + fibonacci(t_N - 2);
}
*/

uint32_t Fibonacci(uint32_t t_N) {
    std::vector<uint32_t> fibNumbers { 0, 1 };
    fibNumbers.reserve(t_N + 1);

    for (uint32_t i = 2; i <= t_N; i++) {
        fibNumbers[i] = fibNumbers[i - 1] + fibNumbers[i - 2];
    }

    return fibNumbers[t_N];
}

int main(int argc, char* argv[]) {
    int32_t n;
    std::cin >> n;
    std::cout << Fibonacci(n) << std::endl;

    return 0;
}