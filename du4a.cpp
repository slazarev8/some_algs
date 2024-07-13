#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

double DPCalcPropability(int cnt, int depth) {
    const auto DpSize = cnt + 1;

    std::vector<std::vector<long double>> dp(DpSize, std::vector<long double>(DpSize, 0.0));
    if (depth >= cnt) {
        return 0;
    }

    for (int i = 0; i <= cnt; ++i) {
        dp[i][0] = i == 0 ? 1.0 : 0.0;
    }

    dp[1][0] = 1.0;
    dp[1][1] = 0.0;

    long double currP = 0.0;
    for (int currN = 2; currN <= cnt; ++currN) {
        for (int currH = 1; currH <= depth; ++currH) {
            currP = 0.0;
            for (int pivot = 0; pivot <= currN - 1; ++pivot) {
                for (int leftH = 0; leftH <= currH - 1; ++leftH) {
                    if (leftH != currH - 1)
                        currP += 2.0 * dp[pivot][currH - 1] * dp[currN - pivot - 1][leftH];
                    else
                        currP += dp[pivot][currH - 1] * dp[currN - pivot - 1][currH - 1];
                }
            }
            dp[currN][currH] = currP / currN;
        }
    }
    return dp[cnt][depth];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int cnt;
    int depth;
    std::cin >> cnt;
    std::cin >> depth;

    std::cout << std::setprecision(10) << DPCalcPropability(cnt, depth) << std::endl;

    return 0;
}
