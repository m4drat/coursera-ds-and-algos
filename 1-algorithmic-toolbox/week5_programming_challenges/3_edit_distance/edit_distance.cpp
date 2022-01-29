#include <iostream>
#include <limits>
#include <string>
#include <vector>

/**
 * @brief Calculates EditDistance between two strings str1 and str2.
 * Uses bottom-up DP approach
 *
 * @param str1
 * @param str2
 * @return int32_t
 */
int32_t EditDistance(const std::string& str1, const std::string& str2)
{
    std::vector<std::vector<int32_t>> dpMat(str1.length() + 1,
                                            std::vector<int32_t>(str2.length() + 1));

    // Initial DP matrix values
    for (uint32_t i = 0; i <= str1.size(); i++)
        dpMat[i][0] = i;

    for (uint32_t i = 0; i <= str2.size(); i++)
        dpMat[0][i] = i;

    for (uint32_t rowIdx = 1; rowIdx <= str1.length(); ++rowIdx) {
        for (uint32_t colIdx = 1; colIdx <= str2.length(); ++colIdx) {
            int32_t curMin = std::numeric_limits<int32_t>::max();

            // Deletion
            if (dpMat[rowIdx - 1][colIdx] + 1 < curMin)
                curMin = dpMat[rowIdx - 1][colIdx] + 1;

            // Insertion
            if (dpMat[rowIdx][colIdx - 1] + 1 < curMin)
                curMin = dpMat[rowIdx][colIdx - 1] + 1;

            // Substitution
            if (str1[rowIdx - 1] != str2[colIdx - 1] && dpMat[rowIdx - 1][colIdx - 1] + 1 < curMin)
                curMin = dpMat[rowIdx - 1][colIdx - 1] + 1;

            // Match
            if (str1[rowIdx - 1] == str2[colIdx - 1] && dpMat[rowIdx - 1][colIdx - 1] < curMin)
                curMin = dpMat[rowIdx - 1][colIdx - 1];

            dpMat[rowIdx][colIdx] = curMin;
        }
    }

    return dpMat[str1.length()][str2.length()];
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::string str1;
        std::string str2;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .str1{ "ab" }, .str2{ "ab" }, .answer = 0, .name{ "1 - testcase" } },
        ProblemStatement{
            .str1{ "short" }, .str2{ "ports" }, .answer = 3, .name{ "2 - testcase" } },
        ProblemStatement{
            .str1{ "editing" }, .str2{ "distance" }, .answer = 5, .name{ "3 - testcase" } },
        ProblemStatement{ .str1{ "cat" }, .str2{ "cut" }, .answer = 1, .name{ "4 - testcase" } },
        ProblemStatement{
            .str1{ "sunday" }, .str2{ "saturday" }, .answer = 3, .name{ "5 - testcase" } },
        ProblemStatement{
            .str1{ "ATGTTATA" }, .str2{ "ATCGTCC" }, .answer = 5, .name{ "6 - testcase" } },
        ProblemStatement{ .str1{ "aacudfoennnndpperdsdpernskfytskkjjjfornthshgf" },
                          .str2{ "ssacudfoenndpeeprerdsddnskfytskkjjjfoppthsggf" },
                          .answer = 13,
                          .name{ "7 - testcase" } },
        ProblemStatement{ .str1{ "aacudfoennnndpperdsdpernskfytskkjjjforntdkihkjhkaysduiasydhshgfas"
                                 "udaiuyasdadiuastdgjbegjuagsjhdasd" },
                          .str2{ "sdeudfoenfffdpperdsweernskfytskkjjjforntdkihkjhkasydhshgfasudasdj"
                                 "fgwhghjqyasdadiuastdgjbegjuagsjdjihsgjdwsd" },
                          .answer = 32,
                          .name{ "8 - testcase" } }
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = EditDistance(testcase.str1, testcase.str2);
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
    std::string str1;
    std::string str2;
    std::cin >> str1 >> str2;
    std::cout << EditDistance(str1, str2) << std::endl;
    return 0;
#endif
}
