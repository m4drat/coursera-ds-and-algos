#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <unordered_map>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#else
#define PROFILE_FUNCTION()
#endif

bool Partition3Recursive(const std::vector<int32_t>&,
                         int32_t,
                         int32_t,
                         int32_t,
                         int32_t,
                         std::unordered_map<std::size_t, bool>);

bool Partition3SolveRecursive(const std::vector<int32_t>& souvenirs)
{
    PROFILE_FUNCTION();

    uint32_t totalSum = std::accumulate(souvenirs.begin(), souvenirs.end(), 0);
    std::unordered_map<std::size_t, bool> lookupTable;

    if (totalSum % 3 != 0) {
        return false;
    }

    return Partition3Recursive(
        souvenirs, souvenirs.size() - 1, totalSum / 3, totalSum / 3, totalSum / 3, lookupTable);
}

/**
 * @brief Recursive top-down solution for the partition-3 problem.
 * Total complexity of this function is: totalSouvenirs * (sum / 3) ^ 3 => O(totalSouvenirs * sum ^
 * 3)
 */
bool Partition3Recursive(const std::vector<int32_t>& souvenirs,
                         int32_t souvenirIdx,
                         int32_t firstSubsetSum,
                         int32_t secondSubsetSum,
                         int32_t thirdSubsetSum,
                         std::unordered_map<std::size_t, bool> lookupTable)
{
    if (firstSubsetSum == 0 && secondSubsetSum == 0 && secondSubsetSum == 0) {
        return true;
    }

    if (souvenirIdx < 0) {
        return false;
    }

    std::size_t hash = 0;
    utils::hash_combine(hash, firstSubsetSum, secondSubsetSum, secondSubsetSum, souvenirIdx);

    if (lookupTable.find(hash) == lookupTable.end()) {
        bool res1 = false;
        if (firstSubsetSum - souvenirs[souvenirIdx] >= 0) {
            res1 = Partition3Recursive(souvenirs,
                                       souvenirIdx - 1,
                                       firstSubsetSum - souvenirs[souvenirIdx],
                                       secondSubsetSum,
                                       thirdSubsetSum,
                                       lookupTable);
        }

        bool res2 = false;
        if (secondSubsetSum - souvenirs[souvenirIdx] >= 0) {
            res2 = Partition3Recursive(souvenirs,
                                       souvenirIdx - 1,
                                       firstSubsetSum,
                                       secondSubsetSum - souvenirs[souvenirIdx],
                                       thirdSubsetSum,
                                       lookupTable);
        }

        bool res3 = false;
        if (thirdSubsetSum - souvenirs[souvenirIdx] >= 0) {
            res3 = Partition3Recursive(souvenirs,
                                       souvenirIdx - 1,
                                       firstSubsetSum,
                                       secondSubsetSum,
                                       thirdSubsetSum - souvenirs[souvenirIdx],
                                       lookupTable);
        }

        lookupTable[hash] = res1 || res2 || res3;
    }

    return lookupTable[hash];
}

/**
 * @brief The idea here is to use DP bottom-up algorithm.
 * We are trying to construct 2 knapsacks with values at most totalSum / 3.
 * If in the end both sums of the knapsacks are equal to 2 * totalSum / 3, it means
 * that we've found a solution.
 * Total complexity of this function: O(totalSouvenirs * totalSum ^ 2)
 */
bool Partition3(const std::vector<int32_t>& souvenirs)
{
    PROFILE_FUNCTION();

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
                // We ignore the last element
                int32_t possibleMax1 = matrix[souvenirIdx - 1][firstKnapsackCap][secondKnapsackCap];
                int32_t possibleMax2 = std::numeric_limits<int32_t>::min();
                int32_t possibleMax3 = std::numeric_limits<int32_t>::min();

                // We place the last element inside the first knapsack
                if (firstKnapsackCap - souvenirs[souvenirIdx - 1] >= 0) {
                    possibleMax2 =
                        matrix[souvenirIdx - 1][firstKnapsackCap - souvenirs[souvenirIdx - 1]]
                              [secondKnapsackCap] +
                        souvenirs[souvenirIdx - 1];
                }

                // We place the last element inside the second knapsack
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

    // The solution is found if the sum of both knapsacks is equal to 2 * sum / 3
    return matrix[souvenirs.size()][totalSum / 3][totalSum / 3] + totalSum / 3 == totalSum;
}

/**
 * @brief Checks whether it is possible to find a subset of "set",
 * wchich values sums up to the targetSum.
 * Partition2 problem is exactly the IsSubsetSum problem.
 * Just use sum(set) / 2 as a targetSum.
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

#ifdef LOCAL_ENV
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
        ProblemStatement{
            .souvenirs{ 0, 1, 2, 0, 3, 4, 4, 5, 0, 0, 8, 0 }, .answer = 1, .name{ "8 - testcase" } }
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = Partition3(testcase.souvenirs);
        auto recursiveAnswer = Partition3SolveRecursive(testcase.souvenirs);
        if (myAlgoAns != testcase.answer || myAlgoAns != recursiveAnswer) {
            throw std::runtime_error("Got (Bottom-UP DP): " + std::to_string(myAlgoAns) + ".\n" +
                                     "Got (Top-down DP) : " + std::to_string(recursiveAnswer) +
                                     ".\n"
                                     "Expected   : " +
                                     std::to_string(testcase.answer) +
                                     ".\n"
                                     "On testcase: " +
                                     testcase.name + "\n");
        }
    }

    // Partition3 - works fast enough until n <= 100
    // Partition3SolveRecursive is much, muuuch slower
    for (uint32_t n = 1; n <= 25; n++) {
        auto input = utils::GenerateRandomVector<int32_t>(n, 1, 30);
        auto myAlgoAns = Partition3(input);
        auto recursiveAnswer = Partition3SolveRecursive(input);

        if (myAlgoAns != recursiveAnswer) {
            throw std::runtime_error("Got (Bottom-UP DP): " + std::to_string(myAlgoAns) + ".\n" +
                                     "Got (Top-down DP) : " + std::to_string(recursiveAnswer) +
                                     ".\n"
                                     "On testcase: " +
                                     utils::VecToStr(input) + "\n");
        }
        // std::cout << "Total souvenirs: " << n << "\n"
        //           << "Answer: " << myAlgoAns << std::endl;
    }

    return true;
}
#endif

int main()
{
#ifdef LOCAL_ENV
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }
#else
    int32_t n;
    std::cin >> n;
    std::vector<int32_t> A(n);
    for (size_t i = 0; i < A.size(); ++i) {
        std::cin >> A[i];
    }
    std::cout << Partition3(std::move(A)) << '\n';
#endif
}
