#include <iostream>
#include <limits>
#include <string>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#else
#define PROFILE_FUNCTION()
#endif

std::vector<int32_t> gDenominations{ 1, 3, 4 };

/**
 * @brief Calculate minimum number of coins to return as a change. Uses DP bottom-up approach.
 * @param money
 * @param denominations
 * @return uint32_t
 */
uint32_t GetChange(uint32_t money, std::vector<int32_t> denominations = gDenominations)
{
    std::vector<int32_t> minNumCoins(money + 1);
    int32_t numCoins{ 0 };

    for (uint32_t cur = 1; cur <= money; ++cur) {
        minNumCoins[cur] = std::numeric_limits<int32_t>::max();
        for (const auto& den : denominations) {
            if (cur >= den) {
                numCoins = minNumCoins[cur - den] + 1;
                if (numCoins < minNumCoins[cur]) {
                    minNumCoins[cur] = numCoins;
                }
            }
        }
    }

    return minNumCoins[money];
}

int32_t GetChangeSlow(int32_t money, std::vector<int32_t> denominations = gDenominations)
{
    if (money == 0)
        return 0;

    int32_t numCoins{ 0 };
    int32_t minNumCoins = std::numeric_limits<int32_t>::max();
    for (const auto& den : denominations) {
        if (money >= den) {
            numCoins = GetChangeSlow(money - den) + 1;
            if (numCoins < minNumCoins) {
                minNumCoins = numCoins;
            }
        }
    }

    return minNumCoins;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> denominations;
        int32_t money;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{
            .denominations{ 1, 3, 4 }, .money = 2, .answer = 2, .name{ "1 - testcase" } },
        ProblemStatement{
            .denominations{ 1, 3, 4 }, .money = 34, .answer = 9, .name{ "2 - testcase" } },
        ProblemStatement{
            .denominations{ 1, 3, 4 }, .money = 17, .answer = 5, .name{ "3 - testcase" } },
        ProblemStatement{
            .denominations{ 1, 3, 4 }, .money = 9, .answer = 3, .name{ "4 - testcase" } },
        ProblemStatement{
            .denominations{ 1, 3, 4 }, .money = 43, .answer = 11, .name{ "5 - testcase" } },
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = GetChange(testcase.money, testcase.denominations);
        if (myAlgoAns != testcase.answer) {
            throw std::runtime_error("Got        : " + std::to_string(myAlgoAns) +
                                     ".\n"
                                     "Expected   : " +
                                     std::to_string(testcase.answer) +
                                     ".\n"
                                     "On testcase: " +
                                     testcase.name + "\n");
        }
    }

    for (auto n : { 4, 13, 24, 34 }) {
        auto correctAnswer = GetChangeSlow(n);
        auto myAlgoAns = GetChange(n);
        if (correctAnswer != myAlgoAns) {
            throw std::runtime_error("Got: " + std::to_string(myAlgoAns) +
                                     ".\n"
                                     "Expected: " +
                                     std::to_string(correctAnswer) + ".\n");
        }
    }

    return true;
}
#endif

int32_t main()
{
#ifdef LOCAL_ENV
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }
#else
    uint32_t money;
    std::cin >> money;
    std::cout << GetChange(money) << '\n';
#endif
}
