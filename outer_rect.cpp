#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <utility>
#include <vector>

struct TRect {
    int xMin;
    int yMin;
    int xMax;
    int yMax;

    bool OtherIsInsideThis(const TRect& other) const {
        return other.xMin > xMin && other.yMin > yMin && other.xMax < xMax && other.yMax < yMax;
    }

    bool ThisIsInsideOther(const TRect& other) const {
        return xMin > other.xMin && yMin > other.yMin && xMax < other.xMax && yMax < other.yMax;
    }

    void Dump() const {
        std::cout << "xMin:" << xMin << std::endl;
        std::cout << "yMin:" << yMin << std::endl;
        std::cout << "xMax:" << xMax << std::endl;
        std::cout << "yMax:" << yMax << std::endl;
    }

    int IsBig() const {
        int width = xMax - xMin;
        int lenght = yMax - yMin;
        return width >= 2 && lenght >= 2;
    }

    int Area() const {
        int width = xMax - xMin;
        return width;
    }
};

bool HaveIntersection(const TRect& first, const TRect& second) {
    if (first.xMin > second.xMax || second.xMin > first.xMax || first.yMin > second.yMax ||
        second.yMin > first.yMax) {
        return false;
    }
    return true;
}

class TQuadTreeNode {
public:
    explicit TQuadTreeNode(const TRect& zone)
        : Zone_(zone)
        , Leaf_(true) {
    }

    bool Insert(TRect* rect) {
        if (!HaveIntersection(Zone_, *rect))
            return false;
        if (!Leaf_)
            return DelegateInsert(rect);
        auto inserted = InsertIntoThis(rect);

        if (Rects_.size() > NodeSize_) {
            Quadrotize();
        }
        return inserted;
    }

    int Dump() const {
        if (Leaf_) {
            std::cout << "I'm leaf node with zone: " << std::endl;
            Zone_.Dump();
            std::cout << "I keep rects: " << std::endl;
            for (const auto& rect : Rects_) {
                rect->Dump();
            }
            return Rects_.size();
        }
        int cnt = 0;
        std::cout << "I'm agregator with zone: " << std::endl;
        Zone_.Dump();
        std::cout << "My Childs: " << std::endl;
        for (const auto& child : Childs_)
            cnt += child->Dump();

        return cnt;
    }

private:
    void Quadrotize() {
        int newMidlieX = (Zone_.xMin + Zone_.xMax) / 2;
        int newMidlieY = (Zone_.yMin + Zone_.yMax) / 2;
        TRect newRectUpperLeft{Zone_.xMin, newMidlieY, newMidlieX, Zone_.yMax};
        TRect newRectUpperRight{newMidlieX, newMidlieY, Zone_.xMax, Zone_.yMax};
        TRect newRectLowerLeft{Zone_.xMin, Zone_.yMin, newMidlieX, newMidlieY};
        TRect newRectLowerRight{newMidlieX, Zone_.yMin, Zone_.xMax, newMidlieY};
        Childs_.emplace_back(std::make_shared<TQuadTreeNode>(newRectUpperLeft));
        Childs_.emplace_back(std::make_shared<TQuadTreeNode>(newRectUpperRight));
        Childs_.emplace_back(std::make_shared<TQuadTreeNode>(newRectLowerLeft));
        Childs_.emplace_back(std::make_shared<TQuadTreeNode>(newRectLowerRight));
        for (const auto& rect : Rects_) {
            for (auto& child : Childs_) {
                child->Insert(rect);
            }
        }

        Leaf_ = false;
        Rects_.clear();
    }

    bool InsertIntoThis(TRect* rect) {
        if (!HaveIntersection(Zone_, *rect))
            return false;

        for (const auto& zoneRect : Rects_) {
            if (HaveIntersection(*zoneRect, *rect))
                return false;
        }
        if (rect->IsBig())
            Rects_.emplace_back(rect);
        return true;
    }

    bool DelegateInsert(TRect* rect) {
        bool inserted = false;
        for (auto& child : Childs_) {
            inserted |= child->Insert(rect);
        }
        return inserted;
    }

    static const int NodeSize_ = 64;
    TRect Zone_;
    bool Leaf_;
    std::vector<TRect*> Rects_;
    std::vector<std::shared_ptr<TQuadTreeNode>> Childs_;
};

int BFSolver(const std::vector<TRect>& rects) {
    if (rects.size() <= 1)
        return rects.size();

    int result = 0;
    for (size_t rectInd = 0; rectInd < rects.size(); ++rectInd) {
        bool outer = true;
        for (size_t checkInd = 0; checkInd < rects.size(); ++checkInd) {
            if (rectInd == checkInd)
                continue;
            if (rects[rectInd].ThisIsInsideOther(rects[checkInd])) {
                outer = false;
                break;
            }
        }
        if (outer)
            result++;
    }
    return result;
}

bool RectsOk(const TRect& first, const TRect& second) {
    return !(first.xMin == second.xMin || first.yMin == second.yMin || first.xMax == second.xMax ||
             first.yMax == second.yMax);
}

TRect generateRandomRectangle(int limit) {
    TRect rect;
    rect.xMin = rand() % limit;
    rect.yMin = rand() % limit;
    rect.xMax = rect.xMin + 1;
    rect.yMax = rect.yMin + 1;
    return rect;
}

std::vector<TRect> generateRandomRectangles(int numberOfRectangles, int limit) {
    std::vector<TRect> rectangles;
    for (int i = 0; i < numberOfRectangles; ++i) {
        rectangles.push_back(generateRandomRectangle(1 + rand() % limit));
    }
    return rectangles;
}

void test() {
    //   srand(time(0));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    constexpr int MaxAmpl = 1000000000;
    int answer = 0;
    TQuadTreeNode myTree(TRect{-MaxAmpl, -MaxAmpl, MaxAmpl, MaxAmpl});

    for (auto& rect : generateRandomRectangles(1000000, 10000000)) {
        answer += myTree.Insert(&rect);
    }
    std::cout << answer << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << "[µs]" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int cnt;
    std::cin >> cnt;
    std::vector<TRect> rects;

    rects.reserve(cnt);
    for (int i = 0; i < cnt; ++i) {
        int xFirst, yFirst, xSecond, ySecond;
        std::cin >> xFirst;
        std::cin >> yFirst;
        std::cin >> xSecond;
        std::cin >> ySecond;

        rects.emplace_back(TRect{std::min(xFirst, xSecond), std::min(yFirst, ySecond),
                                 std::max(xFirst, xSecond), std::max(yFirst, ySecond)});
    }

    std::sort(rects.begin(), rects.end(),
              [](const TRect& lhs, const TRect& rhs) { return lhs.Area() > rhs.Area(); });

    constexpr int MaxAmpl = 1000000000;
    int answer = 0;
    TQuadTreeNode myTree(TRect{-MaxAmpl, -MaxAmpl, MaxAmpl, MaxAmpl});
    for (auto& rect : rects) {
        answer += myTree.Insert(&rect);
    }
    // auto size = myTree.Dump();
    std::cout << answer << std::endl;
    return 0;
}
