
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <vector>

struct IntPoint2D {
    int xCoord;
    int yCoord;
};

struct Segment {
    double start;
    double end;
    std::string AsString() {
        return std::to_string(start) + ", " + std::to_string(end);
    }
};

std::optional<Segment> GetIntersectionOnX(const IntPoint2D& point, double rad) {
    if (std::abs(point.yCoord) > rad)
        return {};

    double start = point.xCoord - sqrt(pow(rad, 2) - pow(point.yCoord, 2));
    double end = point.xCoord + sqrt(pow(rad, 2) - pow(point.yCoord, 2));

    return Segment{start, end};
}

int CountIntersection(const std::vector<Segment>& segments) {
    std::vector<std::pair<double, int>> events;

    for (const auto& seg : segments) {
        events.emplace_back(seg.start, 1);
        events.emplace_back(seg.end, -1);
    }

    auto cmp = [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
        return a.first < b.first;
    };

    std::sort(events.begin(), events.end(), cmp);

    int max_intersections = 0;
    int intersection_count = 0;

    for (const auto& e : events) {
        intersection_count += e.second;
        max_intersections = std::max(max_intersections, intersection_count);
    }

    return max_intersections;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int pointsNumber;
    int targetPoints;
    std::cin >> pointsNumber;
    std::cin >> targetPoints;

    std::vector<IntPoint2D> points;
    points.reserve(pointsNumber);

    for (int i = 0; i < pointsNumber; ++i) {
        int xVal;
        int yVal;
        std::cin >> xVal;
        std::cin >> yVal;
        points.emplace_back(IntPoint2D{xVal, yVal});
    }

    double minR = 0.0;
    double maxR = 4000.0;
    double prevR = 0.0;
    double currR = maxR;

    while (std::fabs(currR - prevR) >= 0.00005) {
        prevR = currR;
        currR = (minR + maxR) / 2.0;

        std::vector<Segment> intersections;
        for (const auto& point : points) {
            auto intersection = GetIntersectionOnX(point, currR);
            if (intersection)
                intersections.emplace_back(*intersection);
        }
        int intersectionsCnt = CountIntersection(intersections);
        if (intersectionsCnt >= targetPoints)
            maxR = currR;
        else
            minR = currR;
    }

    std::cout << std::fixed;
    std::cout << std::setprecision(6) << currR << std::endl;

    return 0;
}
