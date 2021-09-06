#include <vector>
#include <iostream>
#include <algorithm>

uint64_t Gcd(uint64_t t_A, uint64_t t_B) {
    if (t_B == 0) {
        return t_A;
    }

    return Gcd(t_B, t_A % t_B);
}

uint64_t Lcm(uint64_t t_A, uint64_t t_B) {
    return t_A / Gcd(t_A, t_B) * t_B;
}

int main(int argc, char* argv[]) {
    uint64_t a, b;
    std::cin >> a >> b;
    std::cout << Lcm(a, b) << std::endl;

    return 0;
}