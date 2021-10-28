#include <iostream>

int GetChange(int money) {
    uint32_t minNumbersOfCoins{ 0 };

    minNumbersOfCoins += money / 10;
    money %= 10;

    minNumbersOfCoins += money / 5;
    money %= 5;

    minNumbersOfCoins += money;

    return minNumbersOfCoins;
}

int main(int argc, char* argv[]) {
    uint32_t n;
    std::cin >> n;
    std::cout << GetChange(n) << std::endl;
    return 0;
}