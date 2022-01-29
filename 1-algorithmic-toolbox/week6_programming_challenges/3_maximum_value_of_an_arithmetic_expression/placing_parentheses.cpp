#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#else
#define PROFILE_FUNCTION()
#endif

typedef struct MinMax
{
    int64_t min;
    int64_t max;

    MinMax(int64_t newMin, int64_t newMax)
        : min{ newMin }
        , max{ newMax }
    {}

    MinMax()
        : min{ std::numeric_limits<int64_t>::max() }
        , max{ std::numeric_limits<int64_t>::min() }
    {}
} MinMax;

enum class Operation
{
    ADD,
    SUB,
    MUL,
    LAST
};

inline int64_t AsciiToInt(char num)
{
    assert(num >= 0x30);
    return static_cast<int64_t>(num - 0x30);
}

int64_t EvalPair(int64_t a, int64_t b, char op)
{
    if (op == '*') {
        return a * b;
    } else if (op == '+') {
        return a + b;
    } else if (op == '-') {
        return a - b;
    } else {
        throw std::runtime_error("Impossible op!");
    }
}

int64_t GetMaximumValue(const std::string& expr)
{
    PROFILE_FUNCTION();

    /**
     * For the convenience reasons, I've created 2 separate arrays with numbers
     * and operations. But it is possible to use raw expr.
     */
    std::vector<char> operations;
    operations.reserve((expr.size() + 1) / 2);

    std::copy_if(std::begin(expr),
                 std::end(expr),
                 std::back_inserter(operations),
                 [](const char& ch) { return ch == '-' || ch == '+' || ch == '*'; });

    std::vector<int64_t> values;
    values.reserve((expr.size() + 1) / 2);

    std::copy_if(std::begin(expr), std::end(expr), std::back_inserter(values), [](const char& ch) {
        return std::isdigit(ch);
    });
    std::for_each(values.begin(), values.end(), [](int64_t& val) { val -= 0x30; });

    // Create DP matrix
    std::vector<std::vector<MinMax>> dpMatrix(values.size(), std::vector<MinMax>(values.size()));

    for (uint32_t i = 0; i < dpMatrix.size(); ++i) {
        dpMatrix[i][i] = MinMax(values[i], values[i]);
    }

    for (int32_t i = 0; i < values.size() - 1; ++i) {
        for (int32_t j = 0; j < values.size() - i - 1; ++j) {
            uint32_t k = i + j + 1;

            int64_t curMin = std::numeric_limits<int64_t>::max();
            int64_t curMax = std::numeric_limits<int64_t>::min();

            for (int32_t opPos = j; opPos < k; ++opPos) {
                MinMax left = dpMatrix[j][opPos];
                MinMax right = dpMatrix[opPos + 1][k];

                int64_t minmin = EvalPair(left.min, right.min, operations[opPos]);
                int64_t minmax = EvalPair(left.min, right.max, operations[opPos]);
                int64_t maxmin = EvalPair(left.max, right.min, operations[opPos]);
                int64_t maxmax = EvalPair(left.max, right.max, operations[opPos]);

                curMin = std::min({ curMin, minmin, minmax, maxmin, maxmax });
                curMax = std::max({ curMax, minmin, minmax, maxmin, maxmax });
            }

            dpMatrix[j][k] = MinMax(curMin, curMax);
        }
    }

    return dpMatrix[0].back().max;
}

#ifdef LOCAL_ENV
std::string GenerateRandomExpr(uint64_t totalOperations)
{
    uint32_t totalExprLength = 2 * totalOperations + 1;
    std::string expr;
    expr.reserve(totalExprLength);

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int32_t> distr(0, static_cast<uint32_t>(Operation::LAST) - 1);

    expr.push_back(static_cast<char>(0x30 + utils::rng::xorshf96() % 10));

    for (uint32_t i = 0; i < totalOperations; ++i) {
        Operation randomOp = static_cast<Operation>(distr(generator));

        switch (randomOp) {
            case Operation::ADD:
                expr.push_back('+');
                break;
            case Operation::SUB:
                expr.push_back('-');
                break;
            case Operation::MUL:
                expr.push_back('*');
                break;
            default:
                throw std::runtime_error("Impossible random op!");
                break;
        }
        expr.push_back(static_cast<char>(0x30 + utils::rng::xorshf96() % 10));
    }

    return expr;
}

bool CheckSolution()
{
    struct ProblemStatement
    {
        std::string expr;
        int32_t answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .expr{ "1+5" }, .answer = 6, .name{ "1 - testcase" } },
        ProblemStatement{ .expr{ "2*3+5" }, .answer = 16, .name{ "2 - testcase" } },
        ProblemStatement{ .expr{ "2*3+5-2" }, .answer = 14, .name{ "3 - testcase" } },
        ProblemStatement{ .expr{ "1+2-3*4-5" }, .answer = 6, .name{ "4 - testcase" } },
        ProblemStatement{ .expr{ "5-8+7*4-8+9" }, .answer = 200, .name{ "5 - testcase" } },
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = GetMaximumValue(testcase.expr);
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

    for (uint32_t n = 0; n <= 20; n++) {
        std::string expr = GenerateRandomExpr(n);
        auto myAlgoAns = GetMaximumValue(expr);
        std::cout << "Expr (" << std::to_string(n) << "): " << expr << "\n"
                  << "Answer: " << myAlgoAns << std::endl;
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
    std::string s;
    std::cin >> s;
    std::cout << GetMaximumValue(s) << '\n';
#endif
}
