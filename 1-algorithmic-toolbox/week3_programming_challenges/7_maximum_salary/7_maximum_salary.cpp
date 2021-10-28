#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>


bool CompareByDigits(const std::string& firstNum, const std::string& secondNum)
{
    auto firstNumIter = firstNum.begin();
    auto secondNumIter = secondNum.begin();

    for (uint32_t i = 0; i < std::max(firstNum.length(), secondNum.length()); ++i) {
        if (*firstNumIter > *secondNumIter) {
            return true;
        } else if (*firstNumIter < *secondNumIter) {
            return false;
        }

        // If the first number isn't 'over' yet, advance the iterator
        if (firstNumIter + 1 != firstNum.end()) {
            firstNumIter++;
        }

        // If the second number isn't 'over' yet, advance the iterator
        if (secondNumIter + 1 != secondNum.end()) {
            secondNumIter++;
        }
    }

    // If numbers are equal, return true.
    return true;
}


std::string LargestNumber(std::vector<std::string>& numbers) {
    std::string result;
    result.reserve(numbers.size());

    std::sort(numbers.begin(), numbers.end(), CompareByDigits);

    for (size_t i = 0; i < numbers.size(); i++) {
        result += numbers[i];
    }

    return result;
}


void CheckSolution() {
    struct ProblemStatement {
        std::vector<std::string> numbers;
        std::string answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs {
        ProblemStatement{ .numbers = { "21", "2" }, .answer{ "221" }},
        ProblemStatement{ .numbers = { "9", "4", "6", "1", "9" }, .answer{ "99641" }},
        ProblemStatement{ .numbers = { "23", "39", "92" }, .answer{ "923923" }},
        ProblemStatement{ .numbers = { "29", "249", "237", "2" }, .answer{ "292492372" }},
        ProblemStatement{ .numbers = { "200", "2", "210" }, .answer{ "2210200" }}
    };

    for (auto& testcase : problemSolutionPairs) {
        if (LargestNumber(testcase.numbers) != testcase.answer) {
            throw std::runtime_error("Incorrect answer!");
        }
    }
}

int main() {
    // int32_t n;
    // std::cin >> n;
    // std::vector<std::string> numbers(n);
    // for (size_t i = 0; i < numbers.size(); i++) {
    //     std::cin >> numbers[i];
    // }
    // std::cout << LargestNumber(numbers);

    CheckSolution();
}
