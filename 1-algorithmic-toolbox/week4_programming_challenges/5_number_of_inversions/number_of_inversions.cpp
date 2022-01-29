#include <iostream>
#include <vector>
#include <cassert>

#ifdef LOCAL_ENV
#include "utils.hpp"
#endif

uint32_t Merge(std::vector<uint32_t>& arrayToSort, uint32_t left, uint32_t midpoint, uint32_t right) {
    std::vector<uint32_t> leftPart{ std::next(arrayToSort.begin(), left), std::next(arrayToSort.begin(), midpoint + 1) };
    std::vector<uint32_t> rightPart{ std::next(arrayToSort.begin(), midpoint + 1), std::next(arrayToSort.begin(), right + 1) };

    uint32_t sortedArrayIndex = left;
    uint32_t leftPartIdx{ 0 };
    uint32_t rightPartIdx{ 0 };
    
    uint32_t inversionsCount { 0 };

    while (leftPartIdx < leftPart.size() && rightPartIdx < rightPart.size()) {
        uint32_t firstLeftElem = leftPart[leftPartIdx];
        uint32_t firstRightElem = rightPart[rightPartIdx];
        
        if (firstLeftElem <= firstRightElem) {
            arrayToSort[sortedArrayIndex++] = firstLeftElem;
            leftPartIdx++;
        } else {
            // Because leftPart and rightPart are sorted arrays it means
            // that if leftPart[leftPartIdx] > rightPart[rightPartIdx]
            // every element in leftPart starting from index leftPartIdx
            // is bigger than rightPart[rightPartIdx].
            // So the total number of inversions should take into account all 
            // remaining elements from leftPart starting from leftPartIdx.
            inversionsCount = inversionsCount + (leftPart.size() - leftPartIdx);
            rightPartIdx++;
            arrayToSort[sortedArrayIndex++] = firstRightElem;
        }
    }

    for (; leftPartIdx < leftPart.size(); ++leftPartIdx) {
        arrayToSort[sortedArrayIndex++] = leftPart[leftPartIdx];
    }

    for (; rightPartIdx < rightPart.size(); ++rightPartIdx) {
        arrayToSort[sortedArrayIndex++] = rightPart[rightPartIdx];
    }

    assert(sortedArrayIndex <= arrayToSort.size());

    return inversionsCount;
}

uint64_t GetNumberOfInversionsAndSort(std::vector<uint32_t>& arrayToSort, uint32_t left, uint32_t right) {
    uint64_t numberOfInversions{ 0 };
    if (right == left) return 0;
    uint32_t midpoint = left + (right - left) / 2;

    numberOfInversions += GetNumberOfInversionsAndSort(arrayToSort, left, midpoint);
    numberOfInversions += GetNumberOfInversionsAndSort(arrayToSort, midpoint + 1, right);

    numberOfInversions += Merge(arrayToSort, left, midpoint, right);

    return numberOfInversions;
}

#ifdef LOCAL_ENV
bool CheckSolution() {
    struct ProblemStatement {
        std::vector<uint32_t> array;
        uint32_t answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs {
        ProblemStatement{ .array{ 1, 2, 3, 1, 3 }, .answer = 2 },
        ProblemStatement{ .array{ 9, 2, 2, 9 }, .answer = 2 },
        ProblemStatement{ .array{ 3, 9, 2, 9 }, .answer = 2 },
        ProblemStatement{ .array{ 9, 2, 9 }, .answer = 1 },
        ProblemStatement{ .array{ 5, 2, 2, 2, 2 }, .answer = 4 },
        ProblemStatement{ .array{ 5, 2, 2, 2 }, .answer = 3 },
        ProblemStatement{ .array{ 2, 1 }, .answer = 1 },
        ProblemStatement{ .array{ 1, 3, 2, 2, 7 }, .answer = 2 },
        ProblemStatement{ .array{ 1, 2, 3, 4 }, .answer = 0 },
        ProblemStatement{ .array{ 3, 2, 4, 1 }, .answer = 4 },
        ProblemStatement{ .array{ 4, 3, 2, 1  }, .answer = 6 },
        ProblemStatement{ .array{ 1, 4, 2, 3 }, .answer = 2 },
        ProblemStatement{ .array{ 1, 4 }, .answer = 0 },
        ProblemStatement{ .array{ 4, 1 }, .answer = 1 },
        ProblemStatement{ .array{ 1, 1, 1 }, .answer = 0 },
        ProblemStatement{ .array{ 2, 1, 1 }, .answer = 2 },
        ProblemStatement{ .array{ 1, 2, 1 }, .answer = 1 },
        ProblemStatement{ .array{ 3, 2, 1 }, .answer = 3 },
        ProblemStatement{ .array{ 2, 3, 9, 2, 2 }, .answer = 2 }
    };

    for (auto& testcase : problemSolutionPairs) {
        auto temp = testcase.array;
        auto numberOfInversions = GetNumberOfInversionsAndSort(testcase.array, 0, testcase.array.size() - 1);
        if (numberOfInversions != testcase.answer) {
            throw std::runtime_error("Got: " + std::to_string(numberOfInversions) + ".\n"
                                     "Expected: " + std::to_string(testcase.answer) + ".\n"
                                     "On testcase: " + utils::VecToStr(temp) + "\n");
        }
    }

    return true;
}
#endif

int main() {
#ifdef LOCAL_ENV
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }
#else
    uint32_t n;
    std::cin >> n;
    std::vector<uint32_t> array(n);
    for (uint32_t i = 0; i < array.size(); i++) {
        std::cin >> array[i];
    }
    std::cout << GetNumberOfInversionsAndSort(array, 0, array.size() - 1) << '\n';
#endif
}
