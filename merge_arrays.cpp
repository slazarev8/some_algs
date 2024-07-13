#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> MergeTwoArrays(const std::vector<int>& first, const std::vector<int>& second) {
    std::vector<int> result;
    result.reserve(first.size() + second.size());
    size_t firstInd = 0;
    size_t secondInd = 0;

    while (firstInd < first.size() || secondInd < second.size()) {
        if (firstInd < first.size() && secondInd < second.size()) {
            if (first[firstInd] < second[secondInd]) {
                result.emplace_back(first[firstInd++]);
            } else {
                result.emplace_back(second[secondInd++]);
            }
            continue;
        }
        while (firstInd < first.size()) {
            result.emplace_back(first[firstInd++]);
        }
        while (secondInd < second.size()) {
            result.emplace_back(second[secondInd++]);
        }
    }
    return result;
}

std::vector<int> MergeArrays(const std::vector<std::vector<int>>& nums, int left, int right) {
    if (right == left)
        return nums[right];
    if (right - left == 1)
        return MergeTwoArrays(nums[right], nums[left]);
    int middle = (left + right) / 2;
    return MergeTwoArrays(MergeArrays(nums, left, middle), MergeArrays(nums, middle + 1, right));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int numOfArrays;
    int arraySize;
    std::cin >> numOfArrays;
    std::cin >> arraySize;
    std::vector<std::vector<int>> nums;
    nums.resize(numOfArrays);

    for (int arrayN = 0; arrayN < numOfArrays; ++arrayN) {
        nums[arrayN].reserve(arraySize);
        for (int el = 0; el < arraySize; ++el) {
            int val;
            std::cin >> val;
            nums[arrayN].emplace_back(val);
        }
    }

    auto result = MergeArrays(nums, 0, nums.size() - 1);
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
