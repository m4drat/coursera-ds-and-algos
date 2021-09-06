#include <vector>
#include <iostream>
#include <algorithm>

uint32_t Gcd(uint32_t t_A, uint32_t t_B) {
    if (t_B == 0) {
        return t_A;
    }

    return gcd(t_B, t_A % t_B);
}

int main(int argc, char* argv[]) {
    uint32_t a, b;
    std::cin >> a >> b;
    std::cout << Gcd(a, b) << std::endl;

    return 0;
}