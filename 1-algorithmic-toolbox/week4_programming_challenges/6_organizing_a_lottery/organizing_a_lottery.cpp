#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#else
#define PROFILE_FUNCTION()
#endif

constexpr int32_t c_MaxCoord{ 100'000'000 };
constexpr int32_t c_MinCoord{ -100'000'000 };

std::vector<int32_t> CountSegmentsFast(std::vector<int32_t>& starts,
                                       std::vector<int32_t>& ends,
                                       std::vector<int32_t> points)
{
    PROFILE_FUNCTION();

    std::vector<int32_t> cnt;
    cnt.reserve(points.size());

    std::sort(std::begin(starts), std::end(starts));
    std::sort(std::begin(ends), std::end(ends));

    for (auto point : points) {
        // UpperBound - returns iterator to the first element that is greater than the point.
        auto itStarts = std::upper_bound(std::begin(starts), std::end(starts), point);
        auto rightStartsCnt = std::distance(std::end(starts), itStarts);

        // LowerBound - return iterator to the first element that is greater than or equal to the
        // point. We are using pair of UpperBound/LowerBound, because we want to handle the case,
        // where end of one segment is start of the other one, and the point is located exactly at
        // this coordinate.
        auto itEnds = std::lower_bound(std::begin(ends), std::end(ends), point);
        auto rightEndsCnt = std::distance(std::end(ends), itEnds);

        cnt.emplace_back(std::abs(rightStartsCnt - rightEndsCnt));
    }

    return cnt;
}

std::vector<int32_t> CountSegmentsSlow(std::vector<int32_t>& starts,
                                       std::vector<int32_t>& ends,
                                       std::vector<int32_t> points)
{
    PROFILE_FUNCTION();

    assert(starts.size() == ends.size());

    std::vector<int32_t> cnt(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = 0; j < starts.size(); j++) {
            cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
        }
    }
    return cnt;
}

std::pair<std::vector<int32_t>, std::vector<int32_t>> GenerateRandomSegments(uint32_t length)
{
    std::vector<int32_t> starts;
    std::vector<int32_t> ends;
    starts.reserve(length);
    ends.reserve(length);

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());

    for (uint32_t currSegmentIndex = 0; currSegmentIndex < length; ++currSegmentIndex) {
        std::uniform_int_distribution<int32_t> distrLeft(c_MinCoord, c_MaxCoord);
        int32_t left = distrLeft(generator);

        std::uniform_int_distribution<int32_t> distrRight(left, c_MaxCoord);
        int32_t right = distrRight(generator);

        assert(right >= left);

        starts.emplace_back(left);
        ends.emplace_back(right);
    }

    return std::move(std::make_pair(starts, ends));
}

#ifdef LOCAL_ENV
bool CheckSolution()
{
    struct ProblemStatement
    {
        std::vector<int32_t> starts;
        std::vector<int32_t> ends;
        std::vector<int32_t> points;
        std::vector<int32_t> answer;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .starts{ 0, 7 },
                          .ends{ 5, 10 },
                          .points{ 1, 6, 11 },
                          .answer{ 1, 0, 0 },
                          .name{ "1 - testcase" } },
        ProblemStatement{ .starts{ 1, 5, 6 },
                          .ends{ 5, 9, 11 },
                          .points{ 7, 1, 12 },
                          .answer{ 2, 1, 0 },
                          .name{ "2 - testcase" } },
        ProblemStatement{ .starts{ -10 },
                          .ends{ 10 },
                          .points{ -100, 100, 0 },
                          .answer{ 0, 0, 1 },
                          .name{ "3 - testcase" } },
        ProblemStatement{ .starts{ 0, -3, 7 },
                          .ends{ 5, 2, 10 },
                          .points{ 1, 6 },
                          .answer{ 2, 0 },
                          .name{ "4 - testcase" } },
        ProblemStatement{ .starts{ 2, 11 },
                          .ends{ 10, 14 },
                          .points{ 1, 6, 12, 15 },
                          .answer{ 0, 1, 1, 0 },
                          .name{ "5 - testcase" } },
        ProblemStatement{ .starts{ 2, 10 },
                          .ends{ 10, 14 },
                          .points{ 1, 2, 6, 10, 12, 14, 15 },
                          .answer{ 0, 1, 1, 2, 1, 1, 0 },
                          .name{ "6 - testcase" } },
        ProblemStatement{ .starts{ 2, 8 },
                          .ends{ 12, 14 },
                          .points{ 1, 2, 6, 8, 10, 12, 14, 15 },
                          .answer{ 0, 1, 1, 2, 2, 2, 1, 0 },
                          .name{ "7 - testcase" } },
        ProblemStatement{ .starts{ 2, 2, 2 },
                          .ends{ 3, 4, 5 },
                          .points{ 1, 2, 3, 4, 5, 6 },
                          .answer{ 0, 3, 3, 2, 1, 0 },
                          .name{ "8 - testcase" } },
        ProblemStatement{ .starts{ 3, 4, 5 },
                          .ends{ 5, 5, 5 },
                          .points{ 2, 3, 4, 5, 6 },
                          .answer{ 0, 1, 2, 3, 0 },
                          .name{ "9 - testcase" } },
        ProblemStatement{ .starts{ 1, 2, 5, 8, 13, 15 },
                          .ends{ 5, 5, 9, 12, 14, 15 },
                          .points{ 0, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13, 14, 15 },
                          .answer{ 0, 1, 2, 2, 2, 3, 1, 2, 2, 1, 1, 1, 1, 1 },
                          .name{ "10 - testcase" } },
        ProblemStatement{ .starts{ 0, 2, 4, 6, 7 },
                          .ends{ 9, 10, 11, 11, 11 },
                          .points{ -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
                          .answer{ 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 5, 4, 3, 0 },
                          .name{ "11 - testcase" } },
    };

    for (auto& testcase : problemSolutionPairs) {
        auto correctAnswer = CountSegmentsSlow(testcase.starts, testcase.ends, testcase.points);
        auto myAlgoAns = CountSegmentsFast(testcase.starts, testcase.ends, testcase.points);
        if (correctAnswer != myAlgoAns || myAlgoAns != testcase.answer ||
            correctAnswer != testcase.answer) {
            throw std::runtime_error("\n"
                                     "Got        : " +
                                     utils::VecToStr(myAlgoAns) +
                                     ".\n"
                                     "Expected   : " +
                                     utils::VecToStr(correctAnswer) +
                                     ".\n"
                                     "On testcase: " +
                                     testcase.name + "\n");
        }
    }

    for (auto n : { 100, 101, 128, 256, 257, 263, 1023, 1024, 12534, 28900, 35201, 43720, 50000 }) {
        auto randomSegments = std::move(GenerateRandomSegments(n));
        auto randomPoints =
            std::move(utils::GenerateRandomVector<int32_t>(n, c_MinCoord, c_MaxCoord));
        auto correctAnswer =
            CountSegmentsSlow(randomSegments.first, randomSegments.second, randomPoints);
        auto myAlgoAns =
            CountSegmentsFast(randomSegments.first, randomSegments.second, randomPoints);
        if (correctAnswer != myAlgoAns) {
            throw std::runtime_error("Got: " + utils::VecToStr(myAlgoAns) +
                                     ".\n"
                                     "Expected: " +
                                     utils::VecToStr(correctAnswer) + ".\n");
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
    int32_t n, m;
    std::cin >> n >> m;
    std::vector<int32_t> starts(n), ends(n);
    for (size_t i = 0; i < starts.size(); i++) {
        std::cin >> starts[i] >> ends[i];
    }
    std::vector<int32_t> points(m);
    for (size_t i = 0; i < points.size(); i++) {
        std::cin >> points[i];
    }

    std::vector<int32_t> cnt = CountSegmentsFast(starts, ends, points);
    for (size_t i = 0; i < cnt.size(); i++) {
        std::cout << cnt[i] << ' ';
    }
#endif
}
