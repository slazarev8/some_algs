#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <chrono>
#include <map>
#include <random>

constexpr int32_t infin = 10000000;

struct TCoin {
    int Pos;
    int Deadline;

    TCoin operator+(const TCoin& rhs) const {
        return {Pos + rhs.Pos, Deadline + rhs.Deadline};
    }
};

std::optional<int64_t> solveForPosBase(const std::vector<TCoin>& coins, int curr, int left,
                                       int right, int64_t currTime) {
    if (coins[curr].Deadline < currTime)
        return {};

    if (left == -1 && right == static_cast<int>(coins.size()))
        return currTime;

    std::optional<int64_t> leftResult;
    std::optional<int64_t> rightResult;
    if (left >= 0) {
        auto distToLeft = std::abs(coins[curr].Pos - coins[left].Pos);
        leftResult = solveForPosBase(coins, left, left - 1, right, currTime + distToLeft);
    }

    if (right < static_cast<int>(coins.size())) {
        auto distToRight = std::abs(coins[curr].Pos - coins[right].Pos);
        rightResult = solveForPosBase(coins, right, left, right + 1, currTime + distToRight);
    }
    if (!leftResult && !rightResult)
        return {};
    if (leftResult && rightResult) {
        return std::min(*leftResult, *rightResult);
    } else if (leftResult) {
        return leftResult;
    }
    return rightResult;
}

std::optional<int64_t> solveTaskBase(std::vector<TCoin>& coins) {
    if (coins.size() == 1)
        return 0;

    std::sort(coins.begin(), coins.end(),
              [](const TCoin& lhs, const TCoin& rhs) { return lhs.Pos < rhs.Pos; });

    std::optional<int64_t> best;
    for (size_t i = 0; i < coins.size(); ++i) {
        const auto currResult = solveForPosBase(coins, i, i - 1, i + 1, 0);
        if (currResult) {
            if (!best || *currResult < *best)
                best = currResult;
        }
    }

    return best;
}

std::optional<int64_t> solveTaskDP(std::vector<TCoin>& coins) {
    auto dist = [&coins](size_t lhs, size_t rhs) {
        return std::abs(coins[lhs].Pos - coins[rhs].Pos);
    };

    const auto coinSize = coins.size();
    std::sort(coins.begin(), coins.end(),
              [](const TCoin& lhs, const TCoin& rhs) { return lhs.Pos < rhs.Pos; });

    std::vector<std::vector<int>> dpDist(coinSize, std::vector<int>(coinSize, 0));
    std::vector<std::vector<int>> dpTimes(coinSize, std::vector<int>(coinSize, 0));

    for (size_t nextCoinShift = 1; nextCoinShift < coinSize; ++nextCoinShift) {
        for (size_t ind = 0; ind < coinSize - nextCoinShift; ++ind) {
            dpDist[ind][ind + nextCoinShift] = infin;
            dpTimes[ind][ind + nextCoinShift] = infin;
            const auto leftDist = dpDist[ind + 1][ind + nextCoinShift] + dist(ind, ind + 1);
            const auto rightDist =
                dpTimes[ind + 1][ind + nextCoinShift] + dist(ind, ind + nextCoinShift);

            if (leftDist <= coins[ind].Deadline || rightDist <= coins[ind].Deadline) {
                dpDist[ind][ind + nextCoinShift] = std::min(leftDist, rightDist);
            }

            const auto leftTime =
                dpDist[ind][ind + nextCoinShift - 1] + dist(ind, ind + nextCoinShift);
            const auto rightTime = dpTimes[ind][ind + nextCoinShift - 1] +
                                   dist(ind + nextCoinShift - 1, ind + nextCoinShift);

            if (leftTime <= coins[ind + nextCoinShift].Deadline ||
                rightTime <= coins[ind + nextCoinShift].Deadline) {
                dpTimes[ind][ind + nextCoinShift] = std::min(leftTime, rightTime);
            }
        }
    }
    auto ans = std::min(dpTimes[0][coinSize - 1], dpDist[0][coinSize - 1]);
    if (ans == infin)
        return {};
    return ans;
}

bool RandomTest() {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distribGroupsN(1, 100);
    int nCoins = distribGroupsN(gen);

    std::vector<TCoin> coins;
    for (int i = 0; i < nCoins; ++i) {
        std::uniform_int_distribution<> distribPos(1, 1000);
        std::uniform_int_distribution<> distribTime(1, 1000);
        int pos = distribPos(gen);
        int time = distribTime(gen);

        coins.emplace_back(TCoin{pos, time});
    }

    auto testAnswer = solveTaskDP(coins);

    auto baseAnswer = solveTaskBase(coins);

    return baseAnswer == testAnswer;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    //
    //    for (int i = 0; i < 1000; i++) {
    //        if (RandomTest()) {
    //            std::cout << "OK" << std::endl;
    //        } else {
    //            std::cout << "FAIL" << std::endl;
    //            break;
    //        }
    //    }

    int coinsCnt;
    std::cin >> coinsCnt;

    std::vector<TCoin> coins;
    coins.reserve(coinsCnt);

    for (int i = 0; i < coinsCnt; ++i) {
        int pos;
        int time;
        std::cin >> pos;
        std::cin >> time;
        coins.emplace_back(TCoin{pos, time});
    }

    if (auto result = solveTaskDP(coins); result)
        std::cout << *result << std::endl;
    else
        std::cout << "No solution" << std::endl;

    return 0;
}
