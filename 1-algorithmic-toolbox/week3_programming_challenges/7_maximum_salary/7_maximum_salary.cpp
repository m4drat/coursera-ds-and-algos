#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

bool CompareByDigits(const std::string& firstNum, const std::string& secondNum)
{
    std::string first = firstNum + secondNum;
    std::string second = secondNum + firstNum;

    return first.compare(second) > 0 ? true : false;
}

std::string LargestNumber(std::vector<std::string>& numbers)
{
    std::string result;
    result.reserve(numbers.size());

    std::sort(numbers.begin(), numbers.end(), CompareByDigits);

    for (size_t i = 0; i < numbers.size(); i++) {
        result += numbers[i];
    }

    return result;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<std::string> numbers;
        std::string answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{
            .numbers = { "2",  "8",  "2", "3", "6", "4", "1",  "1", "10", "6",  "3",  "3",  "6",
                         "1",  "3",  "8", "4", "6", "1", "10", "8", "4",  "10", "4",  "1",  "3",
                         "2",  "3",  "2", "6", "1", "5", "2",  "9", "8",  "5",  "10", "8",  "7",
                         "9",  "6",  "4", "2", "6", "3", "8",  "8", "9",  "8",  "2",  "9",  "10",
                         "3",  "10", "7", "5", "7", "1", "7",  "5", "1",  "4",  "7",  "6",  "1",
                         "10", "5",  "4", "8", "4", "2", "7",  "8", "1",  "1",  "7",  "4",  "1",
                         "1",  "9",  "8", "6", "5", "9", "9",  "3", "7",  "6",  "3",  "10", "8",
                         "10", "7",  "2", "5", "1", "1", "9",  "9", "5" },
            .answer = "9999999998888888888887777777776666666666555555554444444443333333333222222222"
                      "111111111111111101010101010101010" },
        ProblemStatement{ .numbers = { "21", "2" }, .answer{ "221" } },
        ProblemStatement{ .numbers = { "9", "4", "6", "1", "9" }, .answer{ "99641" } },
        ProblemStatement{ .numbers = { "23", "39", "92" }, .answer{ "923923" } },
        ProblemStatement{ .numbers = { "29", "249", "237", "2" }, .answer{ "292492372" } },
        ProblemStatement{ .numbers = { "200", "2", "210" }, .answer{ "2210200" } }
    };

    for (auto& testcase : problemSolutionPairs) {
        if (LargestNumber(testcase.numbers) != testcase.answer) {
            throw std::runtime_error("Incorrect answer!");
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
    std::vector<std::string> numbers(n);
    for (size_t i = 0; i < numbers.size(); i++) {
        std::cin >> numbers[i];
    }
    std::cout << LargestNumber(numbers) << std::endl;
#endif
}
