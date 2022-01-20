#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "utils.hpp"

/**
 * @brief Bottom-up DP algorithm that solves partitioning problem.
 * This problem itself can be expressed through "subset sum problem".
 * We have to find a subset of a set, whose elements sums up to a specific
 * value. In this case, this specific value is totalSum / 3.
 */
int32_t Partition3(std::vector<int32_t>&& souvenirs)
{
    uint32_t totalSum = std::accumulate(souvenirs.begin(), souvenirs.end(), 0);

    if (totalSum % 3 != 0)
        return 0;

    std::vector<std::vector<bool>> matrix(souvenirs.size() + 1,
                                          std::vector<bool>(totalSum / 3 + 1));

    // We always can construct a set with sum equals to 0
    for (uint32_t i = 0; i <= souvenirs.size(); ++i)
        matrix[i][0] = true;

    for (uint32_t i = 1; i <= souvenirs.size(); ++i) {
        for (int32_t maxSum = 1; maxSum <= totalSum / 3; ++maxSum) {
            if (maxSum - souvenirs[i - 1] >= 0) {
                matrix[i][maxSum] = matrix[i - 1][maxSum] || matrix[i][maxSum - souvenirs[i - 1]];
            } else {
                matrix[i][maxSum] = matrix[i - 1][maxSum];
            }
        }
    }

    return matrix[souvenirs.size()][totalSum / 3];
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
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = Partition3(std::move(testcase.souvenirs));
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
