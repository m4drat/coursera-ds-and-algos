#include <iostream>
#include <limits>
#include <random>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#else
#define PROFILE_FUNCTION()
#endif

int32_t OptimalWeight(int32_t capacity, const std::vector<int32_t>& weights)
{
    std::vector<std::vector<int32_t>> dpMatrix(weights.size() + 1,
                                               std::vector<int32_t>(capacity + 1));

    int32_t currValue = 0;

    for (uint32_t curItemIdx = 1; curItemIdx <= weights.size(); ++curItemIdx) {
        for (int32_t curKnapsackCapacity = 1; curKnapsackCapacity <= capacity;
             ++curKnapsackCapacity) {
            currValue = std::numeric_limits<int32_t>::min();

            if (curKnapsackCapacity - weights[curItemIdx - 1] >= 0) {
                currValue =
                    dpMatrix[curItemIdx - 1][curKnapsackCapacity - weights[curItemIdx - 1]] +
                    weights[curItemIdx - 1];
            }

            dpMatrix[curItemIdx][curKnapsackCapacity] =
                std::max(currValue, dpMatrix[curItemIdx - 1][curKnapsackCapacity]);
        }
    }

    return dpMatrix[weights.size()][capacity];
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    PROFILE_FUNCTION();

    struct ProblemStatement
    {
        int32_t capacity;
        std::vector<int32_t> weights;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{
            .capacity = 10, .weights{ 1, 4, 8 }, .answer = 9, .name{ "1 - testcase" } },
        ProblemStatement{
            .capacity = 10, .weights{ 1, 4, 10 }, .answer = 10, .name{ "2 - testcase" } },
        ProblemStatement{
            .capacity = 12, .weights{ 1, 4, 10 }, .answer = 11, .name{ "3 - testcase" } },
        ProblemStatement{
            .capacity = 12, .weights{ 9, 4, 2, 8, 10 }, .answer = 12, .name{ "4 - testcase" } },
        ProblemStatement{
            .capacity = 3, .weights{ 3, 2, 1 }, .answer = 3, .name{ "5 - testcase" } },
        ProblemStatement{ .capacity = 30,
                          .weights{ 10, 12, 12, 12, 10, 9 },
                          .answer = 29,
                          .name{ "6 - testcase" } },
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = OptimalWeight(testcase.capacity, testcase.weights);
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

    for (auto n : { 1, 15, 40, 70, 82, 99, 113, 222, 256, 300 }) {
        int32_t curWeight = utils::rng::xorshf96() % 10000;
        auto myAlgoAns = OptimalWeight(curWeight, utils::GenerateRandomVector<int32_t>(n, 0, 1E5));
        std::cout << "Weight: " << curWeight << " "
                  << "Total weights: " << n << " "
                  << "Answer: " << myAlgoAns << std::endl;
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
    int32_t n, W;
    std::cin >> W >> n;
    std::vector<int32_t> weights(n);
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> weights[i];
    }
    std::cout << OptimalWeight(W, weights) << '\n';
#endif
}
