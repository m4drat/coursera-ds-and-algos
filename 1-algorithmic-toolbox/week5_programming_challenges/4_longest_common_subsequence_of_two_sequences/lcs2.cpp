#include <iostream>
#include <limits>
#include <vector>

int32_t LongestCommonSubsequence2(std::vector<int32_t>& a, std::vector<int32_t>& b)
{
    // Initial DP matrix values
    std::vector<std::vector<int32_t>> dpMatrix(a.size() + 1, std::vector<int32_t>(b.size() + 1));

    for (uint32_t rowIdx = 1; rowIdx <= a.size(); ++rowIdx) {
        for (uint32_t colIdx = 1; colIdx <= b.size(); ++colIdx) {
            int32_t curMax = std::numeric_limits<int32_t>::min();

            if (a[rowIdx - 1] == b[colIdx - 1] && dpMatrix[rowIdx - 1][colIdx - 1] + 1 > curMax)
                curMax = dpMatrix[rowIdx - 1][colIdx - 1] + 1;

            if (a[rowIdx - 1] != b[colIdx - 1] && dpMatrix[rowIdx - 1][colIdx] > curMax)
                curMax = dpMatrix[rowIdx - 1][colIdx];

            if (a[rowIdx - 1] != b[colIdx - 1] && dpMatrix[rowIdx][colIdx - 1] > curMax)
                curMax = dpMatrix[rowIdx][colIdx - 1];

            dpMatrix[rowIdx][colIdx] = curMax;
        }
    }

    return dpMatrix[a.size()][b.size()];
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> seq1;
        std::vector<int32_t> seq2;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .seq1{ 2, 7, 5 }, .seq2{ 2, 5 }, .answer = 2, .name{ "1 - testcase" } },
        ProblemStatement{ .seq1{ 7 }, .seq2{ 1, 2, 3, 4 }, .answer = 0, .name{ "2 - testcase" } },
        ProblemStatement{
            .seq1{ 2, 7, 8, 3 }, .seq2{ 5, 2, 8, 7 }, .answer = 2, .name{ "3 - testcase" } },
        ProblemStatement{ .seq1{ 1, 2, 3, 3, 2, 2, 2, 2, 4, 6, 7, 8, 9 },
                          .seq2{ 1, 2, 3, 6, 8, 2, 5, 3, 4, 7, 5, 8, 0, 8, 1, 6, 7, 8, 9 },
                          .answer = 9,
                          .name{ "4 - testcase" } },
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = LongestCommonSubsequence2(testcase.seq1, testcase.seq2);
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

    return true;
}
#endif

int32_t main()
{
#ifdef LOCAL_ENV
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }
#else
    std::size_t n;
    std::cin >> n;
    std::vector<int32_t> a(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    size_t m;
    std::cin >> m;
    std::vector<int32_t> b(m);
    for (size_t i = 0; i < m; i++) {
        std::cin >> b[i];
    }

    std::cout << LongestCommonSubsequence2(a, b) << std::endl;
#endif
}
