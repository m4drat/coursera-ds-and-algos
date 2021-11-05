#include <iostream>

uint32_t GetChange(uint32_t t_Money) {
    uint32_t minNumbersOfCoins{ 0 };

    minNumbersOfCoins += t_Money / 10;
    t_Money %= 10;

    minNumbersOfCoins += t_Money / 5;
    t_Money %= 5;

    minNumbersOfCoins += t_Money;

    return minNumbersOfCoins;
}

int main(int argc, char* argv[]) {
    uint32_t n;
    std::cin >> n;
    std::cout << GetChange(n) << std::endl;
    return 0;
}