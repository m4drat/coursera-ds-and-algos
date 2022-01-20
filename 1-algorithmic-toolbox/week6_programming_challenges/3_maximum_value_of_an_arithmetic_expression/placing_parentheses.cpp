#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "utils.hpp"

enum class Operation
{
    ADD,
    SUB,
    MUL,
    LAST
};

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
                std::abort();
                break;
        }
        expr.push_back(static_cast<char>(0x30 + utils::rng::xorshf96() % 10));
    }

    return expr;
}

uint64_t EvalPair(uint64_t a, uint64_t b, char op)
{
    if (op == '*') {
        return a * b;
    } else if (op == '+') {
        return a + b;
    } else if (op == '-') {
        return a - b;
    } else {
        std::abort();
    }
}

uint64_t GetMaximumValue(const std::string& exp)
{
    // write your code here
    return 0;
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
        ProblemStatement{ .expr{ "5-8+7*4-8+9" }, .answer = 200, .name{ "2 - testcase" } },
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

    for (uint32_t n = 0; n <= 14; n++) {
        std::string expr = GenerateRandomExpr(n);
        auto myAlgoAns = GetMaximumValue(expr);
        std::cout << "Expr (" << std::to_string(n) << "): " << expr << "\n"
                  << "Answer: " << myAlgoAns << std::endl;
    }

    return true;
}

int main()
{
    if (CheckSolution()) {
        std::cout << "The solution is correct!\n";
    }

    std::string s;
    std::cin >> s;
    std::cout << GetMaximumValue(s) << '\n';
}
