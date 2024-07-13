#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <chrono>
#include <random>

unsigned int cur = 0; // беззнаковое 32-битное число

unsigned int nextRand24(int aa, int bb) {
    cur = cur * aa + bb; // вычисляется с переполнениями
    return cur >> 8;     // число от 0 до 2**24-1.
}

unsigned int nextRand32(int aa, int bb) {
    unsigned int a24bit = nextRand24(aa, bb);
    unsigned int b24bit = nextRand24(aa, bb);
    return (a24bit << 8) ^ b24bit;
}

unsigned int Partition(std::vector<unsigned int>& nums, unsigned int left, unsigned int right,
                       unsigned int kInd) {
    const auto pivotVal = nums[kInd];
    std::swap(nums[kInd], nums[right]);
    unsigned int currInd = left;
    int randCnt = 0;
    for (unsigned i = left; i < right; ++i) {
        if (nums[i] <= pivotVal) {
            if (nums[i] == pivotVal && randCnt++ % 2 == 0)
                continue;
            std::swap(nums[i], nums[currInd]);
            ++currInd;
        }
    }
    std::swap(nums[currInd], nums[right]);
    return currInd;
}

unsigned int QuickSelect(std::vector<unsigned int>& nums, unsigned int left, unsigned int right,
                         unsigned int kInd) {
    if (right == left)
        return nums[left];

    auto pivotInd = left + rand() % (right - left + 1);

    auto partitionInd = Partition(nums, left, right, pivotInd);
    if (partitionInd == kInd)
        return nums[kInd];
    if (partitionInd <= kInd)
        return QuickSelect(nums, partitionInd + 1, right, kInd);
    else
        return QuickSelect(nums, left, partitionInd - 1, kInd);
}

uint64_t smartSolution(std::vector<unsigned int>& nums) {
    const auto median = QuickSelect(nums, 0, nums.size() - 1, nums.size() / 2);

    size_t result = 0;
    for (size_t i = 0; i < nums.size(); ++i) {
        int64_t diff = static_cast<int64_t>(nums[i]) - static_cast<int64_t>(median);
        result += std::abs(diff);
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int citizensCnt;
    std::cin >> citizensCnt;

    int aVal;
    int bVal;
    std::cin >> aVal;
    std::cin >> bVal;

    std::vector<unsigned int> nums;
    nums.reserve(citizensCnt);
    for (int i = 0; i < citizensCnt; ++i) {
        nums.emplace_back(nextRand32(aVal, bVal));
    }

    std::cout << smartSolution(nums) << std::endl;

    return 0;
}
