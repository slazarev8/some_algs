
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

int FindMinMax(const std::vector<int>& first, const std::vector<int>& second) {
    assert(first.size() == second.size());
    int left = 0;
    int right = first.size() - 1;
    while (left < right) {
        int middle = (left + right) / 2;
        if (left == right)
            return left;
        if (left + 1 == right) {
            if (std::max(first[left], second[left]) < std::max(first[right], second[right]))
                return left;
            else
                return right;
        }
        if (first[middle] <= second[middle]) {
            left = middle;
        } else {
            right = middle;
        }
    }
    return left;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int aNum;
    int bNum;
    int len;
    std::cin >> aNum;
    std::cin >> bNum;
    std::cin >> len;

    std::vector<std::vector<int>> aNums;
    std::vector<std::vector<int>> bNums;

    aNums.resize(aNum);
    bNums.resize(bNum);

    auto FillNums = [&len](std::vector<std::vector<int>>& nums, int linesNumber) {
        for (int i = 0; i < linesNumber; ++i) {
            nums[i].reserve(len);
            for (int j = 0; j < len; ++j) {
                int curr;
                std::cin >> curr;
                nums[i].emplace_back(curr);
            }
        }
    };

    FillNums(aNums, aNum);
    FillNums(bNums, bNum);

    int requestsNumber;
    std::cin >> requestsNumber;

    for (int i = 0; i < requestsNumber; ++i) {
        int first;
        int second;
        std::cin >> first;
        std::cin >> second;
        std::cout << 1 + FindMinMax(aNums[first - 1], bNums[second - 1]) << "\n";
    }

    return 0;
}
