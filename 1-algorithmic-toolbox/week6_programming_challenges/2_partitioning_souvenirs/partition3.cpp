#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

#include "utils.hpp"

bool Partition3Backtracking(const std::vector<int32_t>& souvenirs)
{
    return false;
}

bool Partition3(const std::vector<int32_t>& souvenirs)
{
    uint32_t totalSum = std::accumulate(souvenirs.begin(), souvenirs.end(), 0);

    if (totalSum % 3 != 0)
        return false;

    // [item-index][first knapsack][second knapsack]
    // By default all knapsack combinations are impossible
    std::vector<std::vector<std::vector<int32_t>>> matrix(
        souvenirs.size() + 1,
        std::vector<std::vector<int32_t>>(totalSum / 3 + 1,
                                          std::vector<int32_t>(totalSum / 3 + 1)));

    for (uint32_t souvenirIdx = 1; souvenirIdx <= souvenirs.size(); ++souvenirIdx) {
        for (int32_t firstKnapsackCap = 0; firstKnapsackCap <= totalSum / 3; ++firstKnapsackCap) {
            for (int32_t secondKnapsackCap = 0; secondKnapsackCap <= totalSum / 3;
                 ++secondKnapsackCap) {
                int32_t possibleMax1 = matrix[souvenirIdx - 1][firstKnapsackCap][secondKnapsackCap];
                int32_t possibleMax2 = std::numeric_limits<int32_t>::min();
                int32_t possibleMax3 = std::numeric_limits<int32_t>::min();

                if (firstKnapsackCap - souvenirs[souvenirIdx - 1] >= 0) {
                    possibleMax2 =
                        matrix[souvenirIdx - 1][firstKnapsackCap - souvenirs[souvenirIdx - 1]]
                              [secondKnapsackCap] +
                        souvenirs[souvenirIdx - 1];
                }

                if (secondKnapsackCap - souvenirs[souvenirIdx - 1] >= 0) {
                    possibleMax3 = matrix[souvenirIdx - 1][firstKnapsackCap]
                                         [secondKnapsackCap - souvenirs[souvenirIdx - 1]] +
                                   souvenirs[souvenirIdx - 1];
                }

                matrix[souvenirIdx][firstKnapsackCap][secondKnapsackCap] =
                    std::max({ possibleMax1, possibleMax2, possibleMax3 });
            }
        }
    }

    return matrix[souvenirs.size()][totalSum / 3][totalSum / 3] == 2 * totalSum / 3;
}

/**
 * @brief Bottom-up DP algorithm that solves 2-partitioning problem.
 * This problem itself can be expressed through "subset sum problem".
 * We have to find a subset of a set, whose elements sums up to a specific
 * value. In this case, this specific value is totalSum / 2.
 */
bool Partition2(const std::vector<int32_t>& souvenirs)
{
    uint32_t totalSum = std::accumulate(souvenirs.begin(), souvenirs.end(), 0);

    if (totalSum % 2 != 0)
        return false;

    std::vector<std::vector<bool>> matrix(souvenirs.size() + 1,
                                          std::vector<bool>(totalSum / 2 + 1));

    // We always can construct a set with sum equals to 0
    for (uint32_t i = 0; i <= souvenirs.size(); ++i)
        matrix[i][0] = true;

    for (uint32_t i = 1; i <= souvenirs.size(); ++i) {
        for (int32_t maxSum = 1; maxSum <= totalSum / 2; ++maxSum) {
            if (maxSum - souvenirs[i - 1] >= 0) {
                matrix[i][maxSum] = matrix[i - 1][maxSum] || matrix[i][maxSum - souvenirs[i - 1]];
            } else {
                matrix[i][maxSum] = matrix[i - 1][maxSum];
            }
        }
    }

    return matrix[souvenirs.size()][totalSum / 2];
}

/**
 * @brief Checks whether it is possible to find a subset of "set",
 * wchich values sums up to the targetSum.
 */
bool IsSubsetSum(const std::vector<int32_t>& set, uint32_t targetSum)
{
    std::vector<std::vector<bool>> matrix(set.size() + 1, std::vector<bool>(targetSum + 1));

    // We always can construct a set with sum equals to 0
    for (uint32_t i = 0; i <= set.size(); ++i)
        matrix[i][0] = true;

    /* So, how does this algorithm work?
     * First, what is the subproblem here?
     */
    for (uint32_t i = 1; i <= set.size(); ++i) {
        for (int32_t maxSum = 1; maxSum <= targetSum; ++maxSum) {
            matrix[i][maxSum] = matrix[i - 1][maxSum];

            if (maxSum - set[i - 1] >= 0) {
                matrix[i][maxSum] = matrix[i][maxSum] | matrix[i][maxSum - set[i - 1]];
            }
        }
    }

    return matrix[set.size()][targetSum];
}

bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> souvenirs;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .souvenirs{ 3, 3, 3, 3 }, .answer = 0, .name{ "1 - testcase" } },
        ProblemStatement{ .souvenirs{ 40 }, .answer = 0, .name{ "2 - testcase" } },
        ProblemStatement{ .souvenirs{ 17, 59, 34, 57, 17, 23, 67, 1, 18, 2, 59 },
                          .answer = 1,
                          .name{ "3 - testcase" } },
        ProblemStatement{ .souvenirs{ 1, 2, 3, 4, 5, 5, 7, 7, 8, 10, 12, 19, 25 },
                          .answer = 1,
                          .name{ "4 - testcase" } },
        ProblemStatement{ .souvenirs{ 7, 3, 2, 1, 5, 4, 8 }, .answer = 1, .name{ "5 - testcase" } },
        ProblemStatement{ .souvenirs{ 1, 2, 3, 4, 4, 5, 8 }, .answer = 1, .name{ "6 - testcase" } },
        ProblemStatement{ .souvenirs{ 1, 2, 3, 1, 2, 3, 9 }, .answer = 0, .name{ "7 - testcase" } },
        ProblemStatement{ .souvenirs{ 0, 1, 2, 0, 3, 4, 4, 5, 0, 0, 8, 0 }, .answer = 1, .name{ "8 - testcase" } }
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = Partition3(testcase.souvenirs);
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

    for (uint32_t n = 1; n <= 20; n++) {
        auto myAlgoAns = Partition3(utils::GenerateRandomVector<int32_t>(n, 1, 30));
        std::cout << "Total souvenirs: " << n << "\n"
                  << "Answer: " << myAlgoAns << std::endl;
    }

    return true;
}

int main()
{
    // std::cout << IsSubsetSum({1, 2, 3}, 1) << std::endl;
    // std::cout << IsSubsetSum({2, 2, 3}, 1) << std::endl;
    // std::cout << IsSubsetSum({2, 2, 3}, 5) << std::endl;
    // std::cout << IsSubsetSum({1, 9, 4, 4, 20, 6, 1, 6, 8, 0, 1, 6, 9, 2, 6, 4, 8, 9, 1, 6, 3, 7,
    // 9, 5, 5, 4, 8, 9, 2, 6, 3, 3, 4, 6}, 59) << std::endl; std::cout << IsSubsetSum({1, 9, 4, 4,
    // 20, 6, 1, 6, 8, 0, 1, 6, 9, 2, 6, 4, 8, 9, 1, 6, 3, 7, 9, 5, 5, 4, 8, 9, 2, 6, 3, 3, 4, 6},
    // 50) << std::endl; std::cout << IsSubsetSum({1, 9, 4, 4, 20, 6, 1, 6, 8, 0, 1, 6, 9, 2, 6, 4,
    // 8, 9, 1, 6, 3, 7, 9, 5, 5, 4, 8, 9, 2, 6, 3, 3, 4, 6}, 36) << std::endl;
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }

    int32_t n;
    std::cin >> n;
    std::vector<int32_t> A(n);
    for (size_t i = 0; i < A.size(); ++i) {
        std::cin >> A[i];
    }
    std::cout << Partition3(std::move(A)) << '\n';
}
