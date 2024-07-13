#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <utility>
#include <vector>

int BaseSolution(const std::vector<int>& nums, int left, int right) {
    int res = 0;
    for (size_t i = 0; i < nums.size(); i++) {
        for (size_t j = i; j < nums.size(); j++) {
            int sum = 0;
            for (size_t k = i; k <= j; k++) {
                sum += nums[k];
            }
            if (sum >= left && sum <= right)
                res++;
        }
    }
    return res;
}

// 1 4 6 7
// 2 5 7

// 1 2 4 5 6 7 7

void MergeSort(std::vector<int64_t>& arr, int left, int right,
               const std::function<void(int64_t, int64_t, int64_t, int64_t)>& callbackFoo) {
    if (left >= right) {
        // callbackFoo(left, left, left, left);
        return;
    }

    int mid = left + (right - left) / 2;

    MergeSort(arr, left, mid, callbackFoo);
    MergeSort(arr, mid + 1, right, callbackFoo);

    std::vector<int64_t> leftNums(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<int64_t> rightNums(arr.begin() + mid + 1, arr.begin() + right + 1);

    callbackFoo(left + 1, mid + 1, mid + 1, right);

    std::merge(leftNums.begin(), leftNums.end(), rightNums.begin(), rightNums.end(),
               arr.begin() + left);
}

int64_t CntMoreSubstr(const std::vector<int64_t>& nums, int64_t leftBegin, int64_t leftEnd,
                      int64_t rightBegin, int64_t rightEnd, int64_t target) {
    int64_t res = 0;

    auto maxR = rightBegin;
    for (auto left = leftBegin; left <= leftEnd; left++) {
        for (auto right = maxR; right <= rightEnd; right++) {
            auto currSum = nums[right] - nums[left - 1];
            if (currSum >= target) {
                res += rightEnd - right + 1;
                break;
            }
            maxR = right;
        }
    }

    return res;
}

// 0 1 2 3
// 7 3 5 10
// 0 7 10 15 25

int64_t Solution(std::vector<int>& nums, int64_t leftBoarder, int64_t rightBoarder) {
    if (nums.size() == 1) {
        if (nums[0] >= leftBoarder && nums[0] <= rightBoarder)
            return 1;
        return 0;
    }

    std::vector<int64_t> prefixs;
    prefixs.resize(nums.size() + 1, 0);

    prefixs[0] = 0;
    for (size_t i = 0; i < nums.size(); i++) {
        prefixs[i + 1] = prefixs[i] + static_cast<int64_t>(nums[i]);
    }

    int64_t res = 0;
    auto callback = [&](int64_t leftBegin, int64_t leftEnd, int64_t rightBegin, int64_t rightEnd) {
        auto LeftRes =
            CntMoreSubstr(prefixs, leftBegin, leftEnd, rightBegin, rightEnd, leftBoarder);
        auto RightRes =
            CntMoreSubstr(prefixs, leftBegin, leftEnd, rightBegin, rightEnd, rightBoarder + 1);
        res += (LeftRes - RightRes);
    };
    MergeSort(prefixs, 0, prefixs.size() - 1, callback);
    return res;
}

void test() {
    srand(time(NULL));
    auto cnt = 1 + rand() % 500;
    auto left = -100 + rand() % 100;
    auto right = 100 + rand() % 100;
    std::vector<int> nums;
    for (int i = 0; i < cnt; i++)
        nums.emplace_back(-500 + rand() % 1000);
    auto baseSol = BaseSolution(nums, left, right);
    auto solution = Solution(nums, left, right);
    if (baseSol != solution)
        std::cout << "fail" << std::endl;
    else
        std::cout << "ok" << std::endl;
    std::cout << cnt << std::endl;
}

int main() {
    //    for (int i = 0; i < 100; i++) {
    //        test();
    //        std::this_thread::sleep_for(std::chrono::seconds(1));
    //    }
    int cnt;
    std::cin >> cnt;
    std::vector<int> nums;
    nums.reserve(cnt);

    for (int i = 0; i < cnt; i++) {
        int val;
        std::cin >> val;
        nums.emplace_back(val);
    }

    int64_t left;
    int64_t right;
    std::cin >> left;
    std::cin >> right;

    std::cout << Solution(nums, left, right) << std::endl;

    return 0;
}
