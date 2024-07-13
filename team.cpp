#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

bool operator<(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
    return lhs.first < rhs.first;
}

// using iter = std::vector<std::pair<int, int>>::iterator;

template <typename T>
void Merge(typename T::iterator l, typename T::iterator m, typename T::iterator r) {
    T first(l, m);
    T second(m, r);

    auto begin_l = first.begin();
    auto begin_r = second.begin();
    while (begin_l != first.end() && begin_r != second.end()) {
        if (*begin_l < *begin_r) {
            *l = *begin_l;
            begin_l++;
        } else {
            *l = *begin_r;
            begin_r++;
        }
        l++;
    }

    std::copy(begin_l, first.end(), l);
}

template <typename T>
void MergeSort(typename T::iterator l, typename T::iterator r) {
    std::size_t distance = std::distance(l, r);
    if (distance > 1) {
        typename T::iterator m = l + (distance / 2);
        MergeSort<T>(l, m);
        MergeSort<T>(m, r);
        Merge<T>(l, m, r);
    }
}

inline bool GoodTeam(const std::vector<std::pair<int, int>>& team, int left, int right) {
    if ((right - left) <= 1) {
        return true;
    }
    int64_t sum_of_two = team[left].first;
    sum_of_two += team[left + 1].first;
    return sum_of_two >= team[right].first;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int team_size;
    std::cin >> team_size;

    std::vector<std::pair<int, int>> team;
    team.reserve(team_size);

    for (int i = 0; i < team_size; ++i) {
        int val;
        std::cin >> val;
        team.emplace_back(std::make_pair(val, i + 1));
    }

    if (team.size() == 1) {
        std::cout << team[0].first << std::endl;
        std::cout << 1 << std::endl;
        return 0;
    }

    MergeSort<std::vector<std::pair<int, int>>>(team.begin(), team.end());

    std::vector<int> best;
    int64_t best_sum = 0;

    int left = 0;
    int64_t curr_sum = 0;
    int best_left = 0;
    int best_right = 0;

    for (int right = 0; right < team_size; ++right) {
        curr_sum += team[right].first;
        while (!GoodTeam(team, left, right)) {
            curr_sum -= team[left].first;
            left++;
        }
        if (curr_sum > best_sum) {
            best_left = left;
            best_right = right;
            best_sum = curr_sum;
        }
    }

    std::cout << best_sum << std::endl;
    best.reserve(best_right - best_left + 1);
    for (int team_ind = best_left; team_ind <= best_right; ++team_ind) {
        best.emplace_back(team[team_ind].second);
    }

    MergeSort<std::vector<int>>(best.begin(), best.end());

    for (auto player : best) {
        std::cout << player << " ";
    }
    std::cout << std::endl;

    return 0;
}
