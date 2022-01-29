#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int64_t MaxDotProduct(std::vector<int32_t>& profitPerClick, std::vector<int32_t>& numberOfClicks)
{
    int64_t result = 0;

    for (uint32_t i = 0; i < profitPerClick.size(); ++i) {
        result += profitPerClick[i] * numberOfClicks[i];
    }

    return result;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> firstSequence;
        std::vector<int32_t> secondSequence;
        int64_t answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .firstSequence = { 23 }, .secondSequence{ 39 }, .answer{ 897 } },
        ProblemStatement{
            .firstSequence = { 1, 3, -5 }, .secondSequence{ -2, 4, 1 }, .answer{ 23 } },
        ProblemStatement{ .firstSequence = { 1 }, .secondSequence{ -2, -1 }, .answer{ -1 } },
        ProblemStatement{
            .firstSequence = { 4, 0, 3, -7, -1, -5 }, .secondSequence{ 7, -1 }, .answer{ 28 } },
    };

    for (auto& testcase : problemSolutionPairs) {
        std::sort(
            testcase.firstSequence.begin(), testcase.firstSequence.end(), std::greater<int32_t>());
        std::sort(testcase.secondSequence.begin(),
                  testcase.secondSequence.end(),
                  std::greater<int32_t>());

        if (MaxDotProduct(testcase.firstSequence, testcase.secondSequence) != testcase.answer) {
            std::cerr << "Got incorrect answer: " << testcase.answer << std::endl;
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
    uint32_t n;
    std::cin >> n;
    std::vector<int32_t> profitPerClick(n), numberOfClicks(n);
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> profitPerClick[i];
    }
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> numberOfClicks[i];
    }

    std::sort(profitPerClick.begin(), profitPerClick.end(), std::greater<int32_t>());
    std::sort(numberOfClicks.begin(), numberOfClicks.end(), std::greater<int32_t>());

    std::cout << MaxDotProduct(profitPerClick, numberOfClicks) << std::endl;
#endif
}
