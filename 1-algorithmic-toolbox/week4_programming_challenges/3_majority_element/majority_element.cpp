#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

#ifdef LOCAL_ENV
#include "utils.hpp"
#endif

constexpr int32_t NO_MAJORITY_ELEMENT{ -1 };

/**
 * 6 different algorithms to implement: https://leetcode.com/problems/majority-element/solution/
 * Great article: http://users.ece.northwestern.edu/~dda902/336/hw4-sol.pdf
 */

int32_t Count(const std::vector<int32_t>& array, uint32_t key, uint32_t start, uint32_t end) {
    return std::count_if(
        std::next(array.begin(), start), 
        std::next(array.begin(), end + 1), 
        [key](int32_t elem) {
            return elem == key;
        }
    );
}

int32_t GetMajorityElement(std::vector<int32_t>& array, int32_t left, int32_t right) {
    // Exit condition
    if (left == right) return array[left];

    uint32_t midPoint = static_cast<uint32_t>(std::floor(left + (right - left) / 2.0f));

    int32_t leftPartMaj = GetMajorityElement(array, left, midPoint);
    int32_t rightPartMaj = GetMajorityElement(array, midPoint + 1, right);

    if (leftPartMaj == rightPartMaj) {
        return leftPartMaj;
    }

    auto leftPartMajCount = (NO_MAJORITY_ELEMENT == leftPartMaj) ? 0 : Count(array, leftPartMaj, left, right);
    auto rightPartMajCount = (NO_MAJORITY_ELEMENT == rightPartMaj) ? 0 : Count(array, rightPartMaj, left, right);

    if (leftPartMajCount >= std::floor((right - left + 1) / 2.0f) + 1) {
        return leftPartMaj;
    }

    if (rightPartMajCount >= std::floor((right - left + 1) / 2.0f) + 1) {
        return rightPartMaj;
    }

    return NO_MAJORITY_ELEMENT;
}

int32_t GetMajorityElementCorrect(std::vector<int32_t>& array) {
    for (uint32_t i = 0; i < array.size(); i++) {
        int32_t curElem = array[i];
        uint32_t count = 0;

        for (uint32_t j = 0; j < array.size(); j++) {
            if (array[j] == curElem) count++;
        }

        if (count > array.size() / 2) return curElem;
    }

    return NO_MAJORITY_ELEMENT;
}

#ifdef LOCAL_ENV
bool CheckSolution() {
    struct ProblemStatement {
        std::vector<int32_t> array;
        int32_t answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs {
        ProblemStatement{ .array{ 2, 3, 9, 2, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 1, 1, 1, 3 }, .answer = 1 },
        ProblemStatement{ .array{ 2, 3, 9, 2, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 1, 2, 3, 4 }, .answer = -1 },
        ProblemStatement{ .array{ 2, 2, 3, 4 }, .answer = -1 },
        ProblemStatement{ .array{ 2, 2, 1, 3, 4 }, .answer = -1 },
        ProblemStatement{ .array{ 1 }, .answer = 1 },
        ProblemStatement{ .array{ 1, 1 }, .answer = 1 },
        ProblemStatement{ .array{ 1, 2 }, .answer = -1 },
        ProblemStatement{ .array{ 1, 2, 3 }, .answer = -1 },
        ProblemStatement{ .array{ 1, 2, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 2, 1, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 2, 2, 1 }, .answer = 2 },
        ProblemStatement{ .array{ 2, 2, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 2, 2, 2, 3, 1, 0, 2, 4, 2, 2, 7, 2, 9, 1, 2, 2, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 2, 2, 2, 3, 1, 0, 2, 4, 2, 2, 7, 2, 9, 1, 2, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 1, 2, 1, 2, 1 }, .answer = 1 },
        ProblemStatement{ .array{ 2, 1, 2, 1, 2, 1, 2 }, .answer = 2 },
        ProblemStatement{ .array{ 0 }, .answer = 0 }
    };

    for (auto& testcase : problemSolutionPairs) {
        int32_t myAlgoAns = GetMajorityElement(testcase.array, 0, testcase.array.size() - 1);
        int32_t correctAns = GetMajorityElementCorrect(testcase.array);
        if (myAlgoAns != testcase.answer) {
            throw std::runtime_error("Got: " + std::to_string(myAlgoAns) + ". Expected: " + std::to_string(testcase.answer) + ". Testcase: " + utils::VecToStr(testcase.array));
        }
    }

    return true;
}
#endif

int32_t main() {
#ifdef LOCAL_ENV
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }
#else
    int32_t n;
    std::cin >> n;
    std::vector<int32_t> array(n);

    for (size_t i = 0; i < array.size(); ++i) {
        std::cin >> array[i];
    }

    std::cout << (GetMajorityElement(array, 0, array.size() - 1) != NO_MAJORITY_ELEMENT) << '\n';
#endif
}
