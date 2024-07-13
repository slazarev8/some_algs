#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int Dijkstra(const std::vector<std::vector<std::pair<int, int>>>& gr, int from, int to,
             std::vector<std::vector<int>>& cache) {
    if (!cache[from].empty()) {
        if (cache[from][to] == std::numeric_limits<int>::max())
            return -1;
        return cache[from][to];
    }

    const int infinity = std::numeric_limits<int>::max();
    std::vector<int> dist(gr.size(), infinity);

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        pq;

    dist[from] = 0;
    pq.emplace(0, from);

    while (!pq.empty()) {
        int current = pq.top().second;
        int current_cost = pq.top().first;
        pq.pop();

        if (current_cost > dist[current]) {
            continue;
        }

        for (const auto& edge : gr[current]) {
            int next = edge.first;
            int weight = edge.second;
            int new_cost = current_cost + weight;

            if (new_cost < dist[next]) {
                dist[next] = new_cost;
                pq.emplace(new_cost, next);
            }
        }
    }

    cache[from] = std::move(dist);

    if (cache[from][to] == std::numeric_limits<int>::max())
        return -1;

    return cache[from][to];
}

int BiDijkstra(const std::vector<std::vector<std::pair<int, int>>>& gr,
               const std::vector<std::vector<std::pair<int, int>>>& gr_rev, int from, int to) {
    if (from == to)
        return 0;
    //    if (cache[from].empty())
    //        cache[from] = std::vector<std::optional<int>>(gr.size());
    //    if (cache[from][to])
    //        return *cache[from][to];
    const int infinity = std::numeric_limits<int>::max();
    static std::vector<int> dist_forward(gr.size(), infinity), dist_backward(gr.size(), infinity);
    std::fill(dist_forward.begin(), dist_forward.end(), infinity);
    std::fill(dist_backward.begin(), dist_backward.end(), infinity);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        pq_forward, pq_backward;

    dist_forward[from] = 0;
    dist_backward[to] = 0;
    pq_forward.emplace(0, from);
    pq_backward.emplace(0, to);

    static std::vector<bool> visited_forward(gr.size(), false), visited_backward(gr.size(), false);
    std::fill(visited_forward.begin(), visited_forward.end(), false);
    std::fill(visited_backward.begin(), visited_backward.end(), false);
    int shortest_path = infinity;

    while (!pq_forward.empty() && !pq_backward.empty()) {
        int u = pq_forward.top().second;
        pq_forward.pop();

        int v = pq_backward.top().second;
        pq_backward.pop();

        visited_forward[u] = true;

        for (const auto& edge : gr[u]) {
            int x = edge.first;
            int w = edge.second;

            if (!visited_forward[x] && dist_forward[u] + w <= dist_forward[x]) {
                dist_forward[x] = dist_forward[u] + w;
                pq_forward.emplace(dist_forward[x], x);
            }

            if (visited_backward[x] && dist_forward[u] + w + dist_backward[x] < shortest_path) {
                shortest_path = dist_forward[u] + w + dist_backward[x];
            }
        }

        visited_backward[v] = true;
        for (const auto& edge : gr_rev[v]) {
            int x = edge.first;
            int w = edge.second;

            if (!visited_backward[x] && dist_backward[v] + w <= dist_backward[x]) {
                dist_backward[x] = dist_backward[v] + w;
                pq_backward.emplace(dist_backward[x], x);
            }

            if (visited_forward[x] && dist_backward[v] + w + dist_forward[x] < shortest_path) {
                shortest_path = dist_backward[v] + w + dist_forward[x];
            }
        }

        if (dist_forward[u] + dist_backward[v] >= shortest_path) {
            break;
        }
    }

    auto res = shortest_path == infinity ? -1 : shortest_path;
    // cache[from][to] = res;
    return res;
}

void test() {
    int oasCnt = 20000000;
    int roadsCnt = 20000;

    std::vector<std::vector<std::pair<int, int>>> gr, gr_rev;

    gr.resize(oasCnt);
    gr_rev.resize(oasCnt);

    for (int i = 0; i < roadsCnt; ++i) {
        int from = rand() % oasCnt;
        int to = rand() % oasCnt;
        int cost = rand() % 3;
        gr[from].emplace_back(std::make_pair(to, cost));
        gr_rev[to].emplace_back(std::make_pair(from, cost));
    }

    int requestsCnt = 1;

    std::vector<int> toReq;
    std::vector<int> fromReq;
    for (int i = 0; i < requestsCnt; ++i) {
        int from = rand() % oasCnt;
        int to = rand() % oasCnt;
        toReq.emplace_back(to);
        fromReq.emplace_back(from);
    }
    std::vector<int> resD;
    std::vector<int> resBD;

    //  std::vector<std::vector<std::optional<int>>> cacheBD(gr.size());
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < requestsCnt; ++i) {
        resBD.emplace_back(BiDijkstra(gr, gr_rev, fromReq[i], toReq[i]));
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto durationBiDijkstra =
        std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::vector<std::vector<int>> cache;
    cache.resize(oasCnt);

    start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < requestsCnt; ++i) {
        resD.emplace_back(Dijkstra(gr, fromReq[i], toReq[i], cache));
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto durationDijkstra =
        std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "BiDijkstra took " << durationBiDijkstra / 1000 << " mlseconds." << std::endl;
    std::cout << "Dijkstra took " << durationDijkstra / 1000 << " mlseconds." << std::endl;

    if (resD != resBD) {
        std::cout << "fail" << std::endl;
        abort();
        return;
    }
}

int main() {
    //    for (int i = 0; i < 100; i++) {
    //        srand(time(0));
    //        test();
    //    }
    // std::cout << "ok";
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int oasCnt, roadsCnt;
    std::cin >> oasCnt;
    std::cin >> roadsCnt;

    std::vector<std::vector<std::pair<int, int>>> gr, gr_rev;

    gr.resize(oasCnt);
    gr_rev.resize(oasCnt);

    for (int i = 0; i < roadsCnt; ++i) {
        int from, to, cost;
        std::cin >> from;
        std::cin >> to;
        std::cin >> cost;
        gr[from - 1].emplace_back(std::make_pair(to - 1, cost));
        gr_rev[to - 1].emplace_back(std::make_pair(from - 1, cost));
    }

    int requestsCnt;
    std::cin >> requestsCnt;

    for (int i = 0; i < requestsCnt; ++i) {
        int from, to;
        std::cin >> from;
        std::cin >> to;
        std::cout << BiDijkstra(gr, gr_rev, from - 1, to - 1) << "\n";
    }

    //    if (requestsCnt < 10000) {
    //        for (int i = 0; i < requestsCnt; ++i) {
    //            int from, to;
    //            std::cin >> from;
    //            std::cin >> to;
    //            std::cout << BiDijkstra(gr, gr_rev, from - 1, to - 1) << "\n";
    //        }
    //    } else {
    //        std::vector<std::vector<int>> cache;
    //        cache.resize(oasCnt);
    //
    //        for (int i = 0; i < requestsCnt; ++i) {
    //            int from, to;
    //            std::cin >> from;
    //            std::cin >> to;
    //            std::cout << Dijkstra(gr, from - 1, to - 1, cache) << "\n";
    //        }
    //    }

    return 0;
}
