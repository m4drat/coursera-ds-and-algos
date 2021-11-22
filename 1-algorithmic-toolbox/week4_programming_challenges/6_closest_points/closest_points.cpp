#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <cassert>
#include <limits>

#include "utils.hpp"

constexpr int32_t c_MaxCoord{  1'000'000'000 };
constexpr int32_t c_MinCoord{ -1'000'000'000 };

double Distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

struct Point {
    int32_t x, y;

    double DistanceTo(const Point& point) const {
        return Distance(x, y, point.x, point.y);
    }

    double DistanceToOrigin() const {
        return Distance(x, y, 0, 0);
    }
};

double MinimalDistanceFastLogic(std::vector<Point>&& points);

double MinimalDistanceFast(std::vector<int32_t>& pointsX, std::vector<int32_t>& pointsY) {
    PROFILE_FUNCTION();

    assert(pointsX.size() == pointsY.size());
    
    std::vector<Point> pointsSortedByX;
    pointsSortedByX.reserve(pointsX.size());

    for (uint32_t pointIdx = 0; pointIdx < pointsX.size(); pointIdx++) {
        pointsSortedByX.emplace_back(Point{ .x = pointsX[pointIdx], .y = pointsY[pointIdx]});
    }

    // sort all points by the x coordinate
    std::sort(pointsSortedByX.begin(), pointsSortedByX.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    });

    return MinimalDistanceFastLogic(std::move(pointsSortedByX));
}

double MinimalDistanceFastLogic(std::vector<Point>&& points) {
    assert(points.size() >= 2 && "Got less than two points in the subset");

    // We have two basecase:
    // Size of points is equal to 2 or to 3
    if (points.size() == 2) {
        return points.front().DistanceTo(points.back());
    } else if (points.size() == 3) {
        double d1 = points[0].DistanceTo(points[1]);
        double d2 = points[0].DistanceTo(points[2]);
        double d3 = points[1].DistanceTo(points[2]);
        return std::min({ d1, d2, d3 });
    }

    // Find the middle point (and it's coordinate)
    uint32_t mid = points.size() / 2;
    int32_t splitLineXCoord = points[mid].x;

    // Recursively find minimal distances to the left and to the right of the 
    // middle point
    double leftMinDist = MinimalDistanceFastLogic(
        std::move(std::vector<Point>{ points.begin(), std::next(points.begin(), mid) })
    );
    double rightMinDist = MinimalDistanceFastLogic(
        std::move(std::vector<Point>{ std::next(points.begin(), mid), points.end() })
    );

    // Combine result of previous two recursive calls by examining all points inside
    // strip [splitLineXCoord - leftRightMinDist, splitLineXCoord + leftRightMinDist]
    double leftRightMinDist = std::min(leftMinDist, rightMinDist);
    double stripLeftXCoord = splitLineXCoord - leftRightMinDist;
    double stripRightXCoord = splitLineXCoord + leftRightMinDist;

    // Returns first point which x coordinate is greater 
    // than or equal to the stripLeftXCoord
    auto stripLeftmostPointIter = std::lower_bound(
        points.begin(), points.end(), stripLeftXCoord,
        [](const Point& point, const double& stripLeftX) {
            return point.x < static_cast<double>(stripLeftX) &&
                    !utils::CompareFloat(stripLeftX, static_cast<double>(point.x));
        }
    );
    // Finds point which x coordinate is strictly bigger than
    // the stripRightXCoord (previous point, still will be inside the strip)
    auto stripRightmostPointIter = std::upper_bound(
        points.begin(), points.end(), stripRightXCoord,
        [](const double& stripRightX, const Point& point) {
            return stripRightX < static_cast<double>(point.x) &&
                    !utils::CompareFloat(stripRightX, static_cast<double>(point.x));
        }
    );

    std::vector<Point> stripPoints{stripLeftmostPointIter, stripRightmostPointIter};
    // Sort all points in the strip by their y coordinates
    std::sort(stripPoints.begin(), stripPoints.end(), [](const Point& p1, const Point& p2) {
        return p1.y < p2.y;
    });

    double stripMinDistance = std::numeric_limits<double>::infinity();
    // For each point compare this point with 6 other which
    // may be located at a distance less than or equal to d.
    for (auto stripPointIter = stripPoints.begin(); stripPointIter != stripPoints.end(); ++stripPointIter) {
        // Returns first point which y coordinate is greater 
        // than or equal to the stripPointIter->y - leftRightMinDist
        auto stripLowermostPointIter = std::lower_bound(
            stripPoints.begin(), stripPoints.end(), stripPointIter->y - leftRightMinDist,
            [](const Point& point, const double& stripLowermostY) {
                return point.y < static_cast<double>(stripLowermostY) &&
                        !utils::CompareFloat(stripLowermostY, static_cast<double>(point.y));
            }
        );
        // Finds point which y coordinate is strictly bigger than
        // the stripPointIter->y + leftRightMinDist, and picks the previous one
        auto stripUppermostPointIter = std::prev(std::upper_bound(
            stripPoints.begin(), stripPoints.end(), stripPointIter->y + leftRightMinDist,
            [](const double& stripUppermostY, const Point& point) {
                return stripUppermostY < static_cast<double>(point.y) &&
                        !utils::CompareFloat(stripUppermostY, static_cast<double>(point.y));
            }
        ));

        // For all points in the region, compare them with current point
        // and if needed, update stripMinDistance
        for (; stripLowermostPointIter != stripUppermostPointIter; stripLowermostPointIter++) {
            // Do not calculate distance for the same points, because in this case we will 
            // get 0 distance value
            if (stripPointIter == stripLowermostPointIter) continue;
            double currDist = stripLowermostPointIter->DistanceTo(*stripPointIter);
            stripMinDistance = (currDist < stripMinDistance) ?
                currDist :
                stripMinDistance;
        }
    }

    return std::min({leftMinDist, rightMinDist, stripMinDistance});
}

double MinimalDistanceSlow(const std::vector<int32_t>& pointsX, const std::vector<int32_t>& pointsY) {
    PROFILE_FUNCTION();

    assert(pointsX.size() == pointsY.size());

    double minDistance { +std::numeric_limits<double>::infinity() };

    for (uint32_t point1 = 0; point1 < pointsX.size(); point1++) {
        for (uint32_t point2 = 0; point2 < pointsX.size(); point2++) {
            if (point1 == point2) continue;

            double dist = Distance(pointsX[point1], pointsY[point1], pointsX[point2], pointsY[point2]);
            if (dist < minDistance) {
                minDistance = dist;
            }
        }
    }

    return minDistance;
}

std::pair<std::vector<int32_t>, std::vector<int32_t>> GenerateRandomPoints(uint32_t length) {
    std::vector<int32_t> pointsX;
    std::vector<int32_t> pointsY;
    pointsX.reserve(length);
    pointsY.reserve(length);

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int32_t> distr(c_MinCoord, c_MaxCoord);

    for (uint32_t currSegmentIndex = 0; currSegmentIndex < length; ++currSegmentIndex) {
        int32_t x = std::clamp(distr(generator), c_MinCoord, c_MaxCoord);
        int32_t y = std::clamp(distr(generator), c_MinCoord, c_MaxCoord);

        pointsX.emplace_back(x);
        pointsY.emplace_back(y);
    }

    return std::move(std::make_pair(pointsX, pointsY));
}

void CheckSolution() {
    struct ProblemStatement {
        std::vector<int32_t> pointsX;
        std::vector<int32_t> pointsY;
        double minDistance;
        std::string name;
    };

    std::vector<ProblemStatement> problemSolutionPairs {
        ProblemStatement{ .pointsX{ 0, 3 }, .pointsY{ 0, 4 }, .minDistance = 5.0f, .name{ "1 - testcase" } },
        ProblemStatement{ .pointsX{ 7, 1, 4, 7 }, .pointsY{ 7, 100, 8, 7 }, .minDistance = 0.0f, .name{ "2 - testcase" } },
        ProblemStatement{ .pointsX{ 4, -2, -3, -1, 2, -4, 1, -1, 3, -4, -2 }, .pointsY{ 4, -2, -4, 3, 3, 0, 1, -1, -1, 2, 4 }, .minDistance = std::sqrt(2.0f), .name{ "3 - testcase" } },
        ProblemStatement{ .pointsX{ -2, 4, 3, -4, -1, 2 }, .pointsY{ 1, -5, 2, 4, -4, -1 }, .minDistance = std::sqrt(10.0f), .name{ "4 - testcase" } },
        ProblemStatement{ .pointsX{ -4, -1, 0, 5, -4 }, .pointsY{ 4, 3, 3, 6, 0 }, .minDistance = 1.0, .name{ "5 - testcase" } },
        ProblemStatement{ .pointsX{ -5, -5, -1, -4, 0, 0, 3, 5, 1, 5, 1, -5, -7 }, .pointsY{ 6, 4, 4, 1, 5, 2, 1, 6, 0, 0, -2, -3, -5}, .minDistance = std::sqrt(2.0), .name{ "6 - testcase" } },
        ProblemStatement{ .pointsX{ -5, -5, -2, -4, 0, 0, 3, 5, 1, 5, 1, -5, -7, -1, 1 }, .pointsY{ 7, 4, 3, 1, 5, 2, 1, 6, 0, 0, -5, -3, -5, 8, 8 }, .minDistance = 2.0, .name{ "7 - testcase" } },
        ProblemStatement{ .pointsX{ -2, -2, 0, 1, 3, 5, 4 }, .pointsY{ 2, 0, 0, 1, 3, 2, 0 }, .minDistance = std::sqrt(2.0), .name{ "8 - testcase" } }
    };

    for (auto& testcase : problemSolutionPairs) {
        assert(testcase.pointsX.size() == testcase.pointsY.size());
        double correctAnswer = MinimalDistanceSlow(testcase.pointsX, testcase.pointsY);
        double myAlgoAns = MinimalDistanceFast(testcase.pointsX, testcase.pointsY);

        if (!(utils::CompareFloat(correctAnswer, testcase.minDistance) && utils::CompareFloat(correctAnswer, myAlgoAns) && utils::CompareFloat(myAlgoAns, testcase.minDistance))) {
            throw std::runtime_error("\n"
                                     "Got        : " + std::to_string(myAlgoAns) + ".\n"
                                     "Expected   : " + std::to_string(correctAnswer) + ".\n"
                                     "On testcase: " + testcase.name + "\n");
        }
    }

    for (auto n : { 5'000, 10'000, 15'000, 20'000, 25'000, 30'000, 35'000, 40'000,
                    45'000, 50'000, 60'000, 70'000, 80'000, 90'000, 100'000 }) {
        auto randomPoints = std::move(GenerateRandomPoints(n));
        auto correctAnswer = MinimalDistanceSlow(randomPoints.first, randomPoints.second);
        auto myAlgoAns = MinimalDistanceFast(randomPoints.first, randomPoints.second);
        if (correctAnswer != myAlgoAns) {
            throw std::runtime_error("Got: " + std::to_string(myAlgoAns) + ".\n"
                                     "Expected: " + std::to_string(correctAnswer) + ".\n"
                                     "Testcase size: " + std::to_string(n) + ".\n");
        }
    }
}

int main() {
    CheckSolution();
    // size_t n;
    // std::cin >> n;
    // std::vector<int32_t> pointsX(n);
    // std::vector<int32_t> pointsY(n);
    // for (size_t i = 0; i < n; i++) {
    //   std::cin >> pointsX[i] >> pointsY[i];
    // }

    // std::cout << std::fixed;
    // std::cout << std::setprecision(9) << MinimalDistance(pointsX, pointsY) << "\n";
}
