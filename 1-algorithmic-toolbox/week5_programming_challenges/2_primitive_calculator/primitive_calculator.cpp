#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

std::vector<int32_t> OptimalSequence(int32_t number)
{
    std::vector<int32_t> minOperations(number + 1);

    for (int32_t curNum = 2; curNum <= number; ++curNum) {
        int32_t curMin = std::numeric_limits<int32_t>::max();

        if (curNum % 2 == 0 && minOperations[curNum / 2] + 1 < curMin) {
            curMin = minOperations[curNum / 2] + 1;
        }

        if (curNum % 3 == 0 && minOperations[curNum / 3] + 1 < curMin) {
            curMin = minOperations[curNum / 3] + 1;
        }

        if (curNum >= 1 && minOperations[curNum - 1] + 1 < curMin) {
            curMin = minOperations[curNum - 1] + 1;
        }

        minOperations[curNum] = curMin;
    }

    std::vector<int32_t> sequence{};
    sequence.reserve(minOperations[number]);
    sequence.push_back(number);

    // Reconstruct optimal solution
    for (int32_t curNum = number; curNum > 1;) {
        int32_t curMin = std::numeric_limits<int32_t>::max();
        int32_t curNumCopy = curNum;

        sequence.push_back(curNum);

        if (curNum % 2 == 0 && minOperations[curNum / 2] < curMin) {
            curMin = minOperations[curNum / 2];
            sequence.back() = curNum / 2;
            curNumCopy = curNum / 2;
        }

        if (curNum % 3 == 0 && minOperations[curNum / 3] < curMin) {
            curMin = minOperations[curNum / 3];
            sequence.back() = curNum / 3;
            curNumCopy = curNum / 3;
        }

        if (curNum >= 1 && minOperations[curNum - 1] < curMin) {
            curMin = minOperations[curNum - 1];
            sequence.back() = curNum - 1;
            curNumCopy = curNum - 1;
        }

        curNum = curNumCopy;
    }

    std::reverse(std::begin(sequence), std::end(sequence));

    return sequence;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        int32_t number;
        int32_t optimalSequenceLen;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .number = 1, .optimalSequenceLen = 0, .name{ "1 - testcase" } },
        ProblemStatement{ .number = 5, .optimalSequenceLen = 3, .name{ "2 - testcase" } },
        ProblemStatement{ .number = 96234, .optimalSequenceLen = 14, .name{ "3 - testcase" } },
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = OptimalSequence(testcase.number);
        if (myAlgoAns.size() - 1 != testcase.optimalSequenceLen) {
            throw std::runtime_error("Got        : " + std::to_string(myAlgoAns.size()) +
                                     ".\n"
                                     "Expected   : " +
                                     std::to_string(testcase.optimalSequenceLen) +
                                     ".\n"
                                     "On testcase: " +
                                     testcase.name + "\n");
        }
    }

    // for (auto n : { 4, 13, 24, 34 }) {
    //     auto correctAnswer = GetChangeSlow(n);
    //     auto myAlgoAns = GetChange(n);
    //     if (correctAnswer != myAlgoAns) {
    //         throw std::runtime_error("Got: " + std::to_string(myAlgoAns) + ".\n"
    //                                  "Expected: " + std::to_string(correctAnswer) + ".\n");
    //     }
    // }

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
    std::vector<int32_t> sequence = OptimalSequence(n);
    std::cout << sequence.size() - 1 << std::endl;
    for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << sequence[i] << " ";
    }
#endif
}
