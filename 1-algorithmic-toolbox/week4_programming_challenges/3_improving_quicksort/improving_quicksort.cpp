#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "utils.hpp"

struct Segment {
    int32_t start;
    int32_t end;
};

/**
 * @brief This function takes and array of n elements and partitions it
 * in such a way that every element less than a pivot is going to be placed
 * to the left, every element larger than a pivot is going to be placed
 * to the right and every element that is equal to the pivot will be placed
 * with all of the other pivot elements. 
 * 
 * @param array 
 * @param left 
 * @param right 
 * @param pivotElem 
 * @return Segment 
 */
Segment RandomPartition3(std::vector<int32_t>& array, int32_t left, int32_t right, int32_t pivotElem) {
    for (uint32_t i = left; i <= right; i++) {
        if (array[i] < pivotElem) {
            std::swap(array[left++], array[i]);
        } else if (array[i] > pivotElem) {
            std::swap(array[right--], array[i--]);
        } else {
            /* No-op case. Just increase the counter. */
        }
    }

    return { .start = left, .end = right };
}

void RandomizedQuickSort(std::vector<int32_t>& array, int32_t left, int32_t right) {
    if (left >= right) {
        return;
    }

    int32_t pivotIdx = left + utils::rng::xorshf96() % (right - left + 1);
    int32_t pivotElem = array[pivotIdx];

    Segment sortedRange = RandomPartition3(array, left, right, pivotElem);

    RandomizedQuickSort(array, left, sortedRange.start - 1);
    RandomizedQuickSort(array, sortedRange.end + 1, right);
}

void CheckSolution() {
    struct ProblemStatement {
        std::vector<int32_t> array;
        std::vector<int32_t> answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs {
        ProblemStatement{ .array{ 5, 3, 7, 1, 2, 3 }, .answer{ 1, 2, 3, 3, 5, 7 }},
        ProblemStatement{ .array{ 1, 1, 1, 3 }, .answer{ 1, 1, 1, 3 }},
        ProblemStatement{ .array{ 0, 8, 2, 3, 3, 3, 3, 4, 9 }, .answer{ 0, 2, 3, 3, 3, 3, 4, 8, 9 }},
        ProblemStatement{ .array{ 1, 1, 1, 1 }, .answer{ 1, 1, 1, 1 }},
        ProblemStatement{ .array{ 1, 2, 3, 4 }, .answer{ 1, 2, 3, 4 }},
        ProblemStatement{ .array{ 4, 3, 2, 1 }, .answer{ 1, 2, 3, 4 }},
        ProblemStatement{ .array{ 5, 4, 3, 2, 1 }, .answer{ 1, 2, 3, 4, 5 }},
        ProblemStatement{ .array{ 3, 1, 2, 5 }, .answer{ 1, 2, 3, 5 }},
        ProblemStatement{ .array{ 1 }, .answer{ 1 }},
        ProblemStatement{ .array{ 1, 2 }, .answer{ 1, 2 }},
        ProblemStatement{ .array{ 2, 1 }, .answer{ 1, 2 }},
        ProblemStatement{ .array{ 2, 3, 1 }, .answer{ 1, 2, 3 }},
        ProblemStatement{ .array{ 2, 3, 1, 1, 2 }, .answer{ 1, 1, 2, 2, 3 }},
        ProblemStatement{ .array{ 2, 3, 9, 2, 2 }, .answer{ 2, 2, 2, 3, 9 }},
        ProblemStatement{ .array{ 1, 3, 1, 2, 3, 3, 1, 7, 3, 3 }, .answer{ 1, 1, 1, 2, 3, 3, 3, 3, 3, 7 }},
    };

    for (auto& testcase : problemSolutionPairs) {
        RandomizedQuickSort(testcase.array, 0, testcase.array.size() - 1);
        if (testcase.array != testcase.answer) {
            throw std::runtime_error("Got: " + utils::VecToStr(testcase.array) + ". Expected: " + utils::VecToStr(testcase.answer));
        }
    }
}

int main() {
    // uint32_t n;
    // std::cin >> n;
    // std::vector<uint32_t> array(n);
    // for (size_t i = 0; i < array.size(); ++i) {
    //     std::cin >> array[i];
    // }
    // RandomizedQuickSort(array, 0, array.size() - 1);

    // for (size_t i = 0; i < array.size(); ++i) {
    //     std::cout << array[i] << ' ';
    // }

    CheckSolution();
}
