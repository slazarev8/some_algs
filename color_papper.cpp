#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Rectangle {
    int x1, y1, x2, y2, color, index;
};

struct TEvent {
    int coord;
    bool open;
    int rectId;
    int color;
};

void calculate_top_areas(const std::vector<Rectangle>& rectangles) {
    std::set<int> x_splits;

    for (const auto& rect : rectangles) {
        x_splits.insert(rect.x1);
        x_splits.insert(rect.x2);
    }

    std::vector<int> x_borders(x_splits.begin(), x_splits.end());
    std::map<int, int> color_areas;

    for (size_t i = 0; i < x_borders.size() - 1; ++i) {
        std::vector<bool> processed(rectangles.size(), false);
        int x_from = x_borders[i];
        int x_to = x_borders[i + 1];
        int xWidth = x_to - x_from;
        std::unordered_map<int, int> areas;

        std::vector<Rectangle> intersecting_rectangles;
        for (size_t j = 0; j < rectangles.size(); ++j) {
            const auto& rect = rectangles[j];
            if (rect.x1 <= x_from && rect.x2 >= x_to) {
                intersecting_rectangles.push_back(rect);
            }
        }

        std::vector<TEvent> events;
        for (const auto& rect : intersecting_rectangles) {
            events.emplace_back(TEvent{rect.y1, true, rect.index, rect.color});
            events.emplace_back(TEvent{rect.y2, false, rect.index, rect.color});
        }
        std::sort(events.begin(), events.end(),
                  [](const TEvent& lhs, const TEvent& rhs) { return lhs.coord < rhs.coord; });

        std::priority_queue<int, std::vector<int>> pq;
        std::optional<int> prevCoord;

        for (const auto& event : events) {
            int currCoord = event.coord;

            if (prevCoord) {
                int currArea = currCoord - *prevCoord;
                int currColor = rectangles[pq.top()].color;
                areas[currColor] += currArea;
            }
            if (event.open)
                pq.push(event.rectId);
            else
                processed[event.rectId] = true;

            prevCoord = currCoord;
            while (!pq.empty() && processed[pq.top()])
                pq.pop();
        }

        for (const auto& [color, area] : areas) {
            if (area == 0)
                continue;
            color_areas[color] += area * xWidth;
        }
    }

    for (const auto& entry : color_areas) {
        std::cout << entry.first << " " << entry.second << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int whiteA, whiteB;
    std::cin >> whiteA;
    std::cin >> whiteB;

    int rectsCnt;
    std::cin >> rectsCnt;

    std::vector<Rectangle> rectangles;
    rectangles.reserve(rectsCnt + 1);
    rectangles.emplace_back(Rectangle{0, 0, whiteA, whiteB, 1, 0});

    for (int i = 0; i < rectsCnt; ++i) {
        int x1, x2, y1, y2, color;
        std::cin >> x1;
        std::cin >> y1;
        std::cin >> x2;
        std::cin >> y2;
        std::cin >> color;
        rectangles.emplace_back(Rectangle{x1, y1, x2, y2, color, i + 1});
    }

    calculate_top_areas(rectangles);

    return 0;
}
