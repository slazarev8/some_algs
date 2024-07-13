
#include <functional>
#include <iostream>
#include <random>
#include <string>

struct Item {
    int64_t weight;
    int64_t cost;
};

struct ItemsGroup {
    Item item;
    int itemsNumber;
};

int64_t ClassicalKnapSackDPSolver(const std::vector<Item>& items, int maxSize) {
    std::vector<std::vector<int64_t>> dp(items.size(), std::vector<int64_t>(maxSize + 1, 0));
    for (int i = 0; i <= maxSize; ++i) {
        if (items[0].weight <= i)
            dp[0][i] = items[0].cost;
    }

    for (size_t item = 1; item < items.size(); ++item) {
        for (int w = 0; w <= maxSize; w++) {
            if (items[item].weight <= w)
                dp[item][w] = std::max(dp[item - 1][w],
                                       dp[item - 1][w - items[item].weight] + items[item].cost);
            else
                dp[item][w] = dp[item - 1][w];
        }
    }

    return dp[items.size() - 1][maxSize];
}

int64_t ClassicalKnapSackDPShortSolver(const std::vector<Item>& items, int maxSize) {
    std::vector<int64_t> dpCurr(maxSize + 1, 0);
    std::vector<int64_t> dpPrev(maxSize + 1, 0);
    for (int i = 0; i <= maxSize; ++i) {
        if (items[0].weight <= i)
            dpPrev[i] = items[0].cost;
    }

    for (size_t item = 1; item < items.size(); ++item) {
        for (int w = 0; w <= maxSize; w++) {
            if (items[item].weight <= w)
                dpCurr[w] = std::max(dpPrev[w], dpPrev[w - items[item].weight] + items[item].cost);
            else
                dpCurr[w] = dpPrev[w];
        }
        std::swap(dpPrev, dpCurr);
    }

    return dpPrev[maxSize];
}

int64_t BaseStupidSolver(const std::vector<ItemsGroup>& groups, int maxSize) {
    std::vector<Item> items;
    for (size_t i = 0; i < groups.size(); i++) {
        for (int j = 0; j < groups[i].itemsNumber; j++) {
            items.emplace_back(groups[i].item);
        }
    }
    return ClassicalKnapSackDPSolver(items, maxSize);
}

std::vector<Item> Decompose(const std::vector<ItemsGroup>& groups) {
    std::vector<Item> items;
    for (size_t i = 0; i < groups.size(); ++i) {
        int64_t curK = groups[i].itemsNumber;
        int64_t curW = groups[i].item.weight;
        int64_t curC = groups[i].item.cost;

        for (int64_t power = 1;; power *= 2) {
            if (curK == 0)
                break;
            if (curK < power) {
                items.emplace_back(Item{curW * curK, curC * curK});
                break;
            }

            items.emplace_back(Item{curW * power, curC * power});

            curK -= power;
        }
    }
    return items;
}

int64_t SmartSolver(const std::vector<ItemsGroup>& groups, int maxSize) {
    return ClassicalKnapSackDPShortSolver(Decompose(groups), maxSize);
}

bool RandomTest(std::function<int(const std::vector<ItemsGroup>& groups, int maxSize)> fooBase,
                std::function<int(const std::vector<ItemsGroup>& groups, int maxSize)> foo) {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distribGroupsN(1, 300);
    int nGroups = distribGroupsN(gen);
    std::uniform_int_distribution<> distribMaxW(1, 1000);
    int maxSize = distribMaxW(gen);

    std::vector<ItemsGroup> groups;
    for (int i = 0; i < nGroups; ++i) {
        std::uniform_int_distribution<> distribW(1, 1000);
        std::uniform_int_distribution<> distribC(1, 1000);
        std::uniform_int_distribution<> distribK(1, 1000);
        int w = distribW(gen);
        int k = distribK(gen);
        int c = distribC(gen);
        groups.emplace_back(ItemsGroup{Item{w, c}, k});
    }
    auto baseAnswer = fooBase(groups, maxSize);
    auto testAnswer = foo(groups, maxSize);
    return baseAnswer == testAnswer;
}

int main() {
    //    for (int i = 0; i < 1000; i++) {
    //        if (RandomTest(SmartSolver, BaseStupidSolver)) {
    //            std::cout << "OK" << std::endl;
    //        } else {
    //            std::cout << "FAIL" << std::endl;
    //            break;
    //        }
    //    }

    int nOfGroups;
    std::cin >> nOfGroups;
    int maxSize;
    std::cin >> maxSize;
    std::vector<ItemsGroup> groups;
    groups.reserve(nOfGroups);

    for (int i = 0; i < nOfGroups; ++i) {
        int wi;
        int ci;
        int ki;
        std::cin >> wi;
        std::cin >> ci;
        std::cin >> ki;
        groups.emplace_back(ItemsGroup{Item{wi, ci}, ki});
    }

    std::cout << SmartSolver(groups, maxSize) << std::endl;
}
