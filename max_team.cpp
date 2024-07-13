#include <algorithm>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <stdexcept>
#include <vector>

#include <chrono>
#include <stack>

// Функция для поиска в глубину
void Dfs(const std::vector<std::vector<int>>& graph, int v, std::vector<bool>& visited,
         std::stack<int>& order) {
    visited[v] = true;
    for (int neighbour : graph[v]) {
        if (!visited[neighbour]) {
            Dfs(graph, neighbour, visited, order);
        }
    }
    order.push(v);
}

// Функция для выделения сильно связанных компонент
void ExtractSCC(const std::vector<std::vector<int>>& transposed_graph, int v,
                std::vector<bool>& visited, std::vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int neighbour : transposed_graph[v]) {
        if (!visited[neighbour]) {
            ExtractSCC(transposed_graph, neighbour, visited, component);
        }
    }
}

// Функция транспонирования графа
std::vector<std::vector<int>> TransposeGraph(const std::vector<std::vector<int>>& graph) {
    std::vector<std::vector<int>> transposed_graph(graph.size());
    for (size_t v = 0; v < graph.size(); ++v) {
        for (auto neighbour : graph[v]) {
            transposed_graph[neighbour].push_back(v);
        }
    }
    return transposed_graph;
}

// Основная функция для поиска сильно связанных компонент
std::vector<std::vector<int>>
FindStrongConnected(const std::vector<std::vector<int>>& graph,
                    const std::vector<std::vector<int>>& transposed_graph) {
    std::vector<bool> visited(graph.size(), false);
    std::stack<int> order;

    // Первый обход в глубину для построения стека
    for (size_t v = 0; v < graph.size(); ++v) {
        if (!visited[v]) {
            Dfs(graph, v, visited, order);
        }
    }

    // Второй обход в глубину для выделения сильно связанных компонент
    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> result;
    while (!order.empty()) {
        int v = order.top();
        order.pop();
        if (!visited[v]) {
            std::vector<int> component;
            ExtractSCC(transposed_graph, v, visited, component);
            result.push_back(component);
        }
    }

    return result;
}

bool IsLooserGroup(const std::vector<std::vector<int>>& graph, const std::vector<int>& group) {
    for (auto player : group) {
        for (auto winner_for_this : graph[player]) {
            if (std::find(group.begin(), group.end(), winner_for_this) == group.end()) {
                return true;
            }
        }
    }
    return false;
}

int Solve(const std::vector<std::vector<int>>& graph) {
    // Транспонируем граф
    std::vector<std::vector<int>> transposed_graph = TransposeGraph(graph);
    auto strong_connected = FindStrongConnected(graph, transposed_graph);

    std::vector<bool> is_looser(strong_connected.size());

    for (size_t i = 0; i < strong_connected.size(); ++i) {
        is_looser[i] = IsLooserGroup(transposed_graph, strong_connected[i]);
    }

    int min_group_size = std::numeric_limits<int>::max();
    for (size_t i = 0; i < strong_connected.size(); ++i) {
        if (is_looser[i]) {
            continue;
        }
        min_group_size = std::min(min_group_size, static_cast<int>(strong_connected[i].size()));
    }

    auto res = std::max(1, static_cast<int>(graph.size()) - min_group_size + 1);
    return res;
}

int GenerateRandomNumber(int max_value) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, max_value - 1);
    return distribution(generator);
}

void Test() {
    const int players_cnt = 1 + GenerateRandomNumber(100);
    const int games_cnt = 1 + GenerateRandomNumber(1000);
    std::vector<std::vector<int>> graph;
    graph.resize(players_cnt);
    for (int i = 0; i < games_cnt; i++) {
        int first_player = 1 + GenerateRandomNumber(players_cnt);
        int second_player = 1 + GenerateRandomNumber(players_cnt);
        int result = 1 + GenerateRandomNumber(2);

        if (result == 1) {
            graph[first_player - 1].emplace_back(second_player - 1);
        }
        if (result == 2) {
            graph[second_player - 1].emplace_back(first_player - 1);
        }
    }
    auto start = std::chrono::high_resolution_clock::now();

    // Вызываем функцию
    std::cout << Solve(graph) << std::endl;

    // Засекаем время после выполнения функции
    auto end = std::chrono::high_resolution_clock::now();

    // Получаем разницу времени в миллисекундах
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Выводим время выполнения в миллисекундах
    std::cout << "Time taken: " << duration << " milliseconds" << std::endl;
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int cnt;
    int games_cnt;
    std::cin >> cnt;
    std::cin >> games_cnt;

    std::vector<std::vector<int>> graph;
    graph.resize(cnt);

    if (games_cnt == 0 || cnt <= 1) {
        std::cout << cnt << std::endl;
        return 0;
    }

    for (int i = 0; i < games_cnt; ++i) {
        int first_player;
        int second_player;
        int result;
        std::cin >> first_player;
        std::cin >> second_player;
        std::cin >> result;
        if (result == 3) {
            continue;
        }
        if (result == 1) {
            graph[first_player - 1].emplace_back(second_player - 1);
        }
        if (result == 2) {
            graph[second_player - 1].emplace_back(first_player - 1);
        }
    }

    std::cout << Solve(graph) << std::endl;

    return 0;
}
