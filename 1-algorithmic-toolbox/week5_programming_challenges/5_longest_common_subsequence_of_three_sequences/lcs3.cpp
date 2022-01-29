#include <iostream>
#include <limits>
#include <vector>

int32_t LongestCommonSubsequence3(std::vector<int32_t>& a,
                                  std::vector<int32_t>& b,
                                  std::vector<int32_t>& c)
{
    // Initial DP matrix values
    std::vector<std::vector<std::vector<int32_t>>> dpMatrix(
        a.size() + 1,
        std::vector<std::vector<int32_t>>(b.size() + 1, std::vector<int32_t>(c.size() + 1)));

    for (uint32_t i = 1; i <= a.size(); ++i) {
        for (uint32_t j = 1; j <= b.size(); ++j) {
            for (uint32_t k = 1; k <= c.size(); ++k) {
                int32_t curMax = std::numeric_limits<int32_t>::min();

                // A[i] == B[j] == C[k]
                if (a[i - 1] == b[j - 1] && b[j - 1] == c[k - 1]) {
                    curMax = dpMatrix[i - 1][j - 1][k - 1] + 1;
                }

                if (dpMatrix[i][j][k - 1] > curMax) {
                    curMax = dpMatrix[i][j][k - 1];
                }
                if (dpMatrix[i][j - 1][k] > curMax) {
                    curMax = dpMatrix[i][j - 1][k];
                }
                if (dpMatrix[i - 1][j][k] > curMax) {
                    curMax = dpMatrix[i - 1][j][k];
                }

                dpMatrix[i][j][k] = curMax;
            }
        }
    }

    return dpMatrix[a.size()][b.size()][c.size()];
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> seq1;
        std::vector<int32_t> seq2;
        std::vector<int32_t> seq3;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{ ProblemStatement{ .seq1{ 1, 2, 3 },
                                                                          .seq2{ 2, 1, 3 },
                                                                          .seq3{ 1, 3, 5 },
                                                                          .answer = 2,
                                                                          .name{ "1 - testcase" } },
                                                        ProblemStatement{
                                                            .seq1{ 8, 3, 2, 1, 7 },
                                                            .seq2{ 8, 2, 1, 3, 8, 10, 7 },
                                                            .seq3{ 6, 8, 3, 1, 4, 7 },
                                                            .answer = 3,
                                                            .name{ "2 - testcase" } } };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = LongestCommonSubsequence3(testcase.seq1, testcase.seq2, testcase.seq3);
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

int main()
{
#ifdef LOCAL_ENV
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }
#else
    std::size_t an;
    std::cin >> an;
    std::vector<int32_t> a(an);
    for (size_t i = 0; i < an; i++) {
        std::cin >> a[i];
    }

    std::size_t bn;
    std::cin >> bn;
    std::vector<int32_t> b(bn);
    for (size_t i = 0; i < bn; i++) {
        std::cin >> b[i];
    }

    std::size_t cn;
    std::cin >> cn;
    std::vector<int32_t> c(cn);
    for (size_t i = 0; i < cn; i++) {
        std::cin >> c[i];
    }
    std::cout << LongestCommonSubsequence3(a, b, c) << std::endl;
#endif
}
