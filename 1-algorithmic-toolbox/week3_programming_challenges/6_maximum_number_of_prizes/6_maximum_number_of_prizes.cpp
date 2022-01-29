#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

std::vector<int32_t> OptimalSummands(int32_t candiesCount)
{
    auto solveForCandies = [](int32_t n) -> float { return (-1 + std::sqrt(1 + 8 * n)) / 2.0f; };

    auto nSum = [](int32_t n) -> float { return (n * (n + 1)) / 2.0f; };

    float upperBound = solveForCandies(candiesCount);

    std::vector<int32_t> summands{};

    // The candies count is a perfect number
    if (std::floor(upperBound) == upperBound) {
        summands.resize(static_cast<uint32_t>(std::floor(upperBound)));
        std::iota(std::begin(summands), std::end(summands), 1);

        return summands;
    }

    summands.resize(static_cast<uint32_t>(std::floor(upperBound)) - 1);
    std::iota(std::begin(summands), std::end(summands), 1);
    summands.push_back(candiesCount - nSum(std::floor(upperBound)) + std::floor(upperBound));

    return summands;
}

std::vector<int> CorrectSolution(int n)
{
    // using the algorithm described in the pdf
    std::vector<int> summands;
    for (int k = n, l = 1; k > 0; l++) {
        if (k <= 2 * l) {
            summands.push_back(k);
            k -= k;
        } else {
            summands.push_back(l);
            k -= l;
        }
    }
    return summands;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> summands;
        int32_t candiesCount;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .summands = { 1, 2, 3 }, .candiesCount = 6 },
        ProblemStatement{ .summands = { 1, 2, 5 }, .candiesCount = 8 },
        ProblemStatement{ .summands = { 1 }, .candiesCount = 1 },
        ProblemStatement{ .summands = { 2 }, .candiesCount = 2 }
    };

    for (auto& testcase : problemSolutionPairs) {
        if (OptimalSummands(testcase.candiesCount) != testcase.summands) {
            throw std::runtime_error("Incorrect answer!");
        }
    }

    for (int32_t i = 1; i < 10'000'000; i++) {
        std::vector<int32_t> a = std::move(OptimalSummands(i));
        std::vector<int32_t> b = std::move(CorrectSolution(i));
        if (a != b) {
            std::cerr << "Errored index: " << i << std::endl;
            throw std::runtime_error("Incorrect answer!");
        }
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
    std::vector<int32_t> summands = std::move(CorrectSolution(n));
    std::cout << summands.size() << '\n';
    for (size_t i = 0; i < summands.size(); ++i) {
        std::cout << summands[i] << ' ';
    }
#endif
}
