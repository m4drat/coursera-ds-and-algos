#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

struct Segment
{
    uint32_t start, end;
};

std::vector<uint32_t> OptimalPoints(std::vector<Segment>& segments)
{
    std::vector<uint32_t> points;

    std::sort(segments.begin(), segments.end(), [](const Segment& a, const Segment& b) -> bool {
        return a.start < b.start;
    });

    uint32_t currentEnd = segments.front().end;

    for (size_t segmentIdx = 1; segmentIdx < segments.size(); segmentIdx++) {
        if (segments[segmentIdx].start <= currentEnd) {
            currentEnd = std::min(currentEnd, segments[segmentIdx].end);
        } else {
            points.push_back(currentEnd);
            currentEnd = segments[segmentIdx].end;
        }
    }

    if (points.empty() || points.back() != currentEnd) {
        points.push_back(currentEnd);
    }

    return points;
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<Segment> segments;
        std::vector<uint32_t> answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .segments = { { 1, 3 }, { 2, 5 }, { 3, 6 } }, .answer{ 3 } },
        ProblemStatement{ .segments = { { 4, 7 }, { 1, 3 }, { 2, 5 }, { 5, 6 } }, .answer{ 3, 6 } },
        ProblemStatement{ .segments = { { 1, 5 }, { 2, 7 }, { 3, 5 }, { 4, 5 }, { 8, 10 } },
                          .answer{ 5, 10 } },
        ProblemStatement{ .segments = { { 1, 1 }, { 2, 2 }, { 2, 3 }, { 3, 4 }, { 4, 4 } },
                          .answer{ 1, 2, 4 } },
        ProblemStatement{
            .segments = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 2, 3 }, { 3, 5 }, { 5, 6 }, { 5, 7 } },
            .answer{ 0, 3, 6 } },
    };

    for (auto& testcase : problemSolutionPairs) {
        if (OptimalPoints(testcase.segments) != testcase.answer) {
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
    std::vector<Segment> segments(n);

    for (size_t i = 0; i < segments.size(); ++i) {
        std::cin >> segments[i].start >> segments[i].end;
    }

    std::vector<uint32_t> points = OptimalPoints(segments);
    std::cout << points.size() << "\n";

    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << points[i] << " ";
    }
#endif
}
