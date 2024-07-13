#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <string>
#include <vector>

class TreeCountSolver {
public:
    explicit TreeCountSolver(const std::vector<int>& numbers)
        : numbers_(numbers) {
        dp_.resize(numbers_.size(), std::vector<std::optional<int>>(numbers.size()));
    }
    size_t CountTrees(int left, int right) {
        if (left >= right || std::min(left, right) < 0 ||
            std::max(left, right) >= static_cast<int>(numbers_.size()))
            return 1;

        if (numbers_[left] == numbers_[right])
            return 1;

        if (dp_[left][right])
            return *dp_[left][right];

        size_t res = 0;
        for (int i = left; i <= right; ++i) {
            if (i > left && numbers_[i] == numbers_[i - 1])
                continue;
            res = (res + CountTrees(left, i - 1) * CountTrees(i + 1, right)) % mod_;
        }
        dp_[left][right] = res;

        return res;
    }

private:
    const std::vector<int>& numbers_;
    const int mod_ = 123456789;
    std::vector<std::vector<std::optional<int>>> dp_;
};

void test() {
    int cnt = 400;
    std::vector<int> nums;
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(-1, 2);

    for (int i = 0; i < cnt; ++i) {
        nums.emplace_back(distrib(gen));
    }

    TreeCountSolver solver(nums);
    std::cout << solver.CountTrees(0, cnt - 1) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int numsCnt;
    std::cin >> numsCnt;
    std::vector<int> numbers;
    numbers.reserve(numsCnt);

    for (int i = 0; i < numsCnt; ++i) {
        int number;
        std::cin >> number;
        numbers.emplace_back(number);
    }

    std::sort(numbers.begin(), numbers.end());

    TreeCountSolver solver(numbers);
    std::cout << solver.CountTrees(0, numsCnt - 1) << std::endl;
}
