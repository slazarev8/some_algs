#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <stdexcept>
#include <vector>

#include <chrono>
#include <queue>
#include <stack>

enum class TStateColor {
    UpperAny,
    UpperLeft,
    UpperRight,

    LeftAny,
    LeftLeft,
    LeftRight,

    RightAny,
    RightLeft,
    RightRight,

    DownAny,
    DownLeft,
    DownRight,
};

enum class TAllowedRotation { Any, Left, Right };

enum class TDir { Up, Right, Down, Left };

TStateColor ToColor(TAllowedRotation allowed, TDir currDir) {
    TStateColor stateColors[4][3] = {
        {TStateColor::UpperAny, TStateColor::UpperLeft, TStateColor::UpperRight},
        {TStateColor::RightAny, TStateColor::RightLeft, TStateColor::RightRight},
        {TStateColor::DownAny, TStateColor::DownLeft, TStateColor::DownRight},
        {TStateColor::LeftAny, TStateColor::LeftLeft, TStateColor::LeftRight}};

    return stateColors[static_cast<int>(currDir)][static_cast<int>(allowed)];
}

TDir Rotate(TDir curr) {
    if (curr == TDir::Up)
        return TDir::Down;
    if (curr == TDir::Left)
        return TDir::Right;
    if (curr == TDir::Right)
        return TDir::Left;
    return TDir::Up;
}

struct TPoint {
    int x;
    int y;
    bool operator==(const TPoint& other) const {
        return other.x == x && other.y == y;
    }

    bool operator<(const TPoint& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

std::vector<TDir> GeneratePossibleDirections(TDir currDir, TAllowedRotation allowed) {
    if (allowed == TAllowedRotation::Any)
        return {TDir::Right, TDir::Left, TDir::Up, TDir::Down};
    std::vector<TDir> result;
    result.emplace_back(currDir);
    result.emplace_back(Rotate(currDir));
    if (allowed == TAllowedRotation::Left) {
        auto newDir = static_cast<int>(currDir) - 1;
        if (newDir < 0)
            newDir = 3;
        result.emplace_back(static_cast<TDir>(newDir));
    }

    if (allowed == TAllowedRotation::Right) {
        auto newDir = static_cast<int>(currDir) + 1;
        if (newDir > 3)
            newDir = 0;

        result.emplace_back(static_cast<TDir>(newDir));
    }
    return result;
}

struct TState {
    TPoint pos;
    TDir dir;
    TAllowedRotation allowed;
    bool operator<(const TState& other) const {
        return std::tie(pos, dir, allowed) < std::tie(other.pos, other.dir, other.allowed);
    }
};

bool inBounds(int x, int y, int n, int m) {
    return x >= 0 && x < m && y >= 0 && y < n;
}

std::vector<TPoint>
ReconstructPath(std::vector<std::vector<std::vector<std::optional<TState>>>>& prev,
                const TState& finish, const TPoint& start) {
    std::vector<TPoint> path;

    TState cur = finish;
    while (cur.pos.x != start.x || cur.pos.y != start.y) {
        path.push_back(cur.pos);

        const auto stateCode = static_cast<int>(ToColor(cur.allowed, cur.dir));
        cur = *prev[cur.pos.y][cur.pos.x][stateCode];
    }

    path.push_back(start);

    reverse(path.begin(), path.end());
    return path;
}

std::vector<TPoint> Solve(const std::vector<std::vector<bool>>& map, const TPoint& start,
                          const TPoint& finish) {
    std::vector<std::vector<std::vector<std::optional<TState>>>> prev;

    prev.resize(map.size());
    for (auto& line : prev) {
        line.resize(map[0].size());
        for (auto& cell : line) {
            cell.resize(12);
        }
    }

    std::queue<TState> q;
    for (auto dir : {TDir::Right, TDir::Left, TDir::Down, TDir::Up}) {
        q.emplace(TState{start, dir, TAllowedRotation::Any});
    }

    while (!q.empty()) {
        auto currEl = q.front();
        q.pop();

        auto currPos = currEl.pos;
        auto currDir = currEl.dir;
        auto allowed = currEl.allowed;

        if (!inBounds(currEl.pos.x, currEl.pos.y, map.size(), map[0].size()))
            continue;

        if (map[currPos.y][currPos.x])
            continue;

        if (currEl.pos == finish) {
            return ReconstructPath(prev, currEl, start);
        }

        for (auto possibleDir : GeneratePossibleDirections(currDir, allowed)) {
            int diff = static_cast<int>(currDir) - static_cast<int>(possibleDir);
            if (diff == -3)
                diff = 1;
            if (diff == 3)
                diff = -1;

            if (diff == 1) {
                if (allowed == TAllowedRotation::Right)
                    continue;
                allowed = TAllowedRotation::Right;
            }
            if (diff == -1) {
                if (allowed == TAllowedRotation::Left)
                    continue;
                allowed = TAllowedRotation::Left;
            }

            TPoint nextPos;
            if (possibleDir == TDir::Up) {
                nextPos = TPoint{currPos.x, currPos.y - 1};
            } else if (possibleDir == TDir::Right) {
                nextPos = TPoint{currPos.x + 1, currPos.y};
            } else if (possibleDir == TDir::Left) {
                nextPos = TPoint{currPos.x - 1, currPos.y};
            } else if (possibleDir == TDir::Down) {
                nextPos = TPoint{currPos.x, currPos.y + 1};
            }

            TState nextState = {nextPos, possibleDir, allowed};

            if (!inBounds(nextPos.x, nextPos.y, map.size(), map[0].size()) ||
                map[nextPos.y][nextPos.x])
                continue;

            const auto newStateCode = static_cast<int>(ToColor(allowed, possibleDir));

            if (!prev[nextState.pos.y][nextState.pos.x][newStateCode]) {
                prev[nextState.pos.y][nextState.pos.x][newStateCode] = currEl;
                q.push(nextState);
            }
        }
    }

    return {};
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int nSize;
    int mSize;
    std::cin >> nSize;
    std::cin >> mSize;

    std::vector<std::vector<bool>> map;
    map.resize(nSize);
    for (auto& line : map) {
        line.resize(mSize);
    }

    for (int i = 0; i < nSize; ++i) {
        for (int j = 0; j < mSize; ++j) {
            char cell;
            std::cin >> cell;
            if (cell == '#')
                map[i][j] = true;
            else
                map[i][j] = false;
        }
    }

    TPoint start;
    TPoint finish;

    int x, y;

    std::cin >> y;
    std::cin >> x;
    start.x = x - 1;
    start.y = y - 1;

    std::cin >> y;
    std::cin >> x;
    finish.x = x - 1;
    finish.y = y - 1;

    auto res = Solve(map, start, finish);

    if (res.empty()) {
        std::cout << "-1" << std::endl;
    } else {
        std::cout << res.size() - 1 << std::endl;
        for (const auto& point : res) {
            std::cout << point.y + 1 << " " << point.x + 1 << "\n";
        }
    }

    return 0;
}
