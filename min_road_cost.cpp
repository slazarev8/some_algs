#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <random>
#include <stack>
#include <stdexcept>
#include <vector>

void dfs(int v, int p, int& timer, std::vector<bool>& visited, std::vector<int>& tin,
         std::vector<int>& low, const std::vector<std::vector<std::pair<int, int>>>& edges,
         std::vector<std::tuple<int, int, int>>& bridges) {
    visited[v] = true;
    tin[v] = low[v] = timer++;

    for (const auto& edge : edges[v]) {
        int to = edge.first;
        int weight = edge.second;

        if (to == p)
            continue;
        if (visited[to]) {
            low[v] = std::min(low[v], tin[to]);
        } else {
            dfs(to, v, timer, visited, tin, low, edges, bridges);
            low[v] = std::min(low[v], low[to]);
            if (low[to] > tin[v]) {
                bridges.push_back({v, to, weight});
            }
        }
    }
}

int findCheapestBridge(const std::vector<std::vector<std::pair<int, int>>>& edges) {
    int timer = 0;
    std::vector<std::tuple<int, int, int>> bridges;
    std::vector<bool> visited(edges.size(), false);
    std::vector<int> tin(edges.size(), -1);
    std::vector<int> low(edges.size(), -1);

    for (size_t i = 0; i < edges.size(); ++i) {
        if (!visited[i]) {
            dfs(i, -1, timer, visited, tin, low, edges, bridges);
        }
    }

    int min_cost = std::numeric_limits<int>::max();
    // std::tuple<int, int, int> cheapest_bridge = {-1, -1, -1};
    for (const auto& bridge : bridges) {
        int v1, v2, weight;
        std::tie(v1, v2, weight) = bridge;
        if (weight < min_cost) {
            min_cost = weight;
        }
    }

    if (min_cost == std::numeric_limits<int>::max())
        return -1;
    return min_cost;
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int cityCnt, roadsCnt;
    std::cin >> cityCnt;
    std::cin >> roadsCnt;

    std::vector<std::vector<std::pair<int, int>>> gr;
    gr.resize(cityCnt);

    for (int i = 0; i < roadsCnt; ++i) {
        int from, to, cost;
        std::cin >> from;
        std::cin >> to;
        std::cin >> cost;

        gr[from - 1].emplace_back(std::make_pair((to - 1), cost));
        gr[to - 1].emplace_back(std::make_pair((from - 1), cost));
    }

    auto res = findCheapestBridge(gr);

    std::cout << res << std::endl;

    return 0;
}
