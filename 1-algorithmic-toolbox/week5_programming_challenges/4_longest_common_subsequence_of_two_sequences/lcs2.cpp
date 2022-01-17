#include <iostream>
#include <vector>

int32_t LongestCommonSubsequence2(std::vector<int32_t>& a, std::vector<int32_t>& b)
{
    // write your code here
    return std::min(a.size(), b.size());
}

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

int32_t main()
{
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }

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
}
