#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

/**
 * @brief Binary search implementation. Default version will return
 * a random index in a data set for a key that occurs more than once.
 *
 * @param array
 * @param elementToFind
 * @return int32_t
 */
int32_t BinarySearch(const std::vector<int32_t>& array, int32_t elementToFind)
{
    if (array.empty()) {
        return -1;
    }

    int32_t left = 0, right = static_cast<int32_t>(array.size()) - 1;

    while (left <= right) {
        int32_t mid = left + (right - left) / 2;

        if (elementToFind == array[mid]) {
            return mid;
        } else if (elementToFind <= array[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;
}

int32_t LinearSearch(const std::vector<int32_t>& array, int32_t elementToFind)
{
    for (size_t i = 0; i < array.size(); ++i) {
        if (array[i] == elementToFind)
            return i;
    }
    return -1;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> array;
        int32_t elementToFind;
        int32_t answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .array{ 1 }, .elementToFind = 1, .answer = 0 },
        ProblemStatement{ .array{ 1 }, .elementToFind = 2, .answer = -1 },
        ProblemStatement{ .array{ 1 }, .elementToFind = 0, .answer = -1 },

        ProblemStatement{ .array{ 1, 2, 3, 4 }, .elementToFind = 2, .answer = 1 },

        ProblemStatement{ .array{ 1, 2, 3 }, .elementToFind = 4, .answer = -1 },
        ProblemStatement{ .array{ 1, 2, 3 }, .elementToFind = 0, .answer = -1 },

        ProblemStatement{ .array{ 1, 2, 2, 3, 4, 5 }, .elementToFind = 2, .answer = 2 },
        ProblemStatement{ .array{ 3, 3, 3, 3, 3 }, .elementToFind = 3, .answer = 2 },

        ProblemStatement{ .array{ 1, 5, 8, 12, 13 }, .elementToFind = 8, .answer = 2 },
        ProblemStatement{ .array{ 1, 5, 8, 12, 13 }, .elementToFind = 1, .answer = 0 },
        ProblemStatement{ .array{ 1, 5, 8, 12, 13 }, .elementToFind = 23, .answer = -1 },
        ProblemStatement{ .array{ 1, 5, 8, 12, 13 }, .elementToFind = 1, .answer = 0 },
        ProblemStatement{ .array{ 1, 5, 8, 12, 13 }, .elementToFind = 11, .answer = -1 },

    };

    for (const auto& testcase : problemSolutionPairs) {
        int32_t answer = BinarySearch(testcase.array, testcase.elementToFind);
        if (answer != testcase.answer) {
            throw std::runtime_error("Got: " + std::to_string(answer) +
                                     ". Expected: " + std::to_string(testcase.answer));
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
    std::vector<int32_t> array(n);
    for (size_t i = 0; i < array.size(); i++) {
        std::cin >> array[i];
    }

    int32_t m;
    std::cin >> m;
    std::vector<int> keys(m);

    for (int i = 0; i < m; ++i) {
        std::cin >> keys[i];
    }

    for (int i = 0; i < m; ++i) {
        // replace with the call to binary_search when implemented
        std::cout << BinarySearch(array, keys[i]) << ' ';
    }
#endif
}
