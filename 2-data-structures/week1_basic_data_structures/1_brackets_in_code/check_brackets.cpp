#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <vector>

struct Bracket
{
    Bracket(char type, int position)
        : type(type)
        , position(position)
    {}

    bool Matchc(char c)
    {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
    uint32_t position;
};

/**
 * @brief Checks whether the given string is balanced or not.
 * Returns empty optional if every
 */
std::string IsBalanced(const std::string& str)
{
    std::stack<Bracket> stack;
    for (uint32_t i = 0; i < str.length(); i++) {
        char ch = str[i];

        // If we have opening bracket, push it to the stack
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(Bracket(ch, i));
        }

        // If we have closing bracket
        if (ch == ')' || ch == ']' || ch == '}') {
            // If stack is empty it means, that we don't have matching bracket
            // Print out current unmatched closing bracket position
            if (stack.empty())
                return std::to_string(i + 1);

            Bracket top = stack.top();
            stack.pop();

            // If Our current bracket is not matched with the last saved on stack
            // Write out current unmatched bracket position
            if (!top.Matchc(ch))
                return std::to_string(i + 1);
        }
    }

    // If the stack is empty it means that all brackets are processed and the string is correctly
    // balanced Otherwise, extract last unmatched opening bracket from the stack, and print its
    // position
    return (stack.empty()) ? "Success" : std::to_string(stack.top().position + 1);
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::string input;
        std::string answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .input{ "[]" }, .answer{ "Success" }, .name{ "1 - testcase" } },
        ProblemStatement{ .input{ "{}[]" }, .answer{ "Success" }, .name{ "2 - testcase" } },
        ProblemStatement{ .input{ "(())" }, .answer{ "Success" }, .name{ "3 - testcase" } },
        ProblemStatement{ .input{ "[()]" }, .answer{ "Success" }, .name{ "4 - testcase" } },
        ProblemStatement{ .input{ "{[]}()" }, .answer{ "Success" }, .name{ "5 - testcase" } },
        ProblemStatement{ .input{ "{" }, .answer{ "1" }, .name{ "6 - testcase" } },
        ProblemStatement{ .input{ "{[}" }, .answer{ "3" }, .name{ "7 - testcase" } },
        ProblemStatement{ .input{ "foo(bar);" }, .answer{ "Success" }, .name{ "8 - testcase" } },
        ProblemStatement{ .input{ "foo(bar[i)" }, .answer{ "10" }, .name{ "9 - testcase" } },
        ProblemStatement{ .input{ "[[]]{" }, .answer{ "5" }, .name{ "10 - testcase" } },
        ProblemStatement{ .input{ "[[]]{]" }, .answer{ "6" }, .name{ "11 - testcase" } },
        ProblemStatement{ .input{ "[[]])" }, .answer{ "5" }, .name{ "13 - testcase" } }
    };

    for (auto& testcase : problemSolutionPairs) {
        auto myAlgoAns = IsBalanced(testcase.input);
        if (myAlgoAns != testcase.answer) {
            throw std::runtime_error("Got        : " + myAlgoAns +
                                     ".\n"
                                     "Expected   : " +
                                     testcase.answer +
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
    std::string text;
    getline(std::cin, text);

    auto answer = IsBalanced(text);
    std::cout << answer << std::endl;
#endif
    return 0;
}
