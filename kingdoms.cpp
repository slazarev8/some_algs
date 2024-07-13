#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

struct TTree {
    TTree(){};
    std::vector<std::shared_ptr<TTree>> childs;
    int value;
    int code;
};

std::shared_ptr<TTree> BuildTree(int current, const std::vector<std::vector<int>>& edges,
                                 std::vector<bool>& visited) {
    visited[current] = true;

    auto current_node = std::make_shared<TTree>();
    current_node->value = current;

    for (int adjacent : edges[current]) {
        if (!visited[adjacent]) {
            std::shared_ptr<TTree> sub_tree = BuildTree(adjacent, edges, visited);
            current_node->childs.emplace_back(std::move(sub_tree));
        }
    }

    return current_node;
}

struct ChildHash {
    std::size_t operator()(const std::vector<std::shared_ptr<TTree>>& vec) const {
        std::hash<int> hasher;
        std::size_t seed = 0;
        for (auto ch : vec) {
            seed ^= hasher(ch->code) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

int GetTreeCode(const std::shared_ptr<TTree>& root, int& cnt) {
    if (!root) {
        return 0;
    }

    for (auto& child : root->childs) {
        GetTreeCode(child, cnt);
    }

    std::sort(root->childs.begin(), root->childs.end(),
              [](const std::shared_ptr<TTree>& lhs, const std::shared_ptr<TTree>& rhs) {
                  return lhs->code < rhs->code;
              });

    auto childHash = ChildHash{}(root->childs);
    root->code = static_cast<int>(childHash);

    return root->code;
}

bool AreTreesEqual(const std::shared_ptr<TTree>& treeOne, const std::shared_ptr<TTree>& treeTwo,
                   std::vector<int>& transitions) {
    if (treeOne == nullptr && treeTwo == nullptr) {
        return true;
    }

    if (treeOne == nullptr || treeTwo == nullptr) {
        return false;
    }

    if (treeOne->code != treeTwo->code) {
        return false;
    }

    if (treeOne->childs.size() != treeTwo->childs.size()) {
        return false;
    }

    for (size_t i = 0; i < treeOne->childs.size(); ++i) {
        if (!AreTreesEqual(treeOne->childs[i], treeTwo->childs[i], transitions)) {
            return false;
        }
    }

    transitions[treeOne->value - 1] = treeTwo->value;

    return true;
}

void dfs(int vertex, const std::vector<std::vector<int>>& edges, std::vector<int>& sizes) {
    sizes[vertex] = 1;
    for (int child : edges[vertex]) {
        if (sizes[child] != 0)
            continue;
        dfs(child, edges, sizes);
        sizes[vertex] += sizes[child];
    }
}

int getCentroid(int vertex, const std::vector<std::vector<int>>& edges,
                std::vector<int>& sizes) // v - any vertex of tree
{
    dfs(vertex, edges, sizes);
    int cnt = 0;
    while (cnt == 0) {
        int magicVertex = -1;
        for (int child : edges[vertex]) {
            if (sizes[child] > sizes[vertex])
                continue;
            if ((2 * sizes[child]) > static_cast<int>(sizes.size() - 1)) {
                magicVertex = child;
                break;
            }
        }
        if (magicVertex == -1)
            break;
        vertex = magicVertex;
    }
    return vertex;
}

std::vector<int> getCentroids(int vertex,
                              const std::vector<std::vector<int>>& edges) // v - any vertex of tree
{
    std::vector<int> sizes(edges.size(), 0);
    vertex = getCentroid(vertex, edges, sizes);
    std::vector<int> res;
    res.push_back(vertex);
    for (int u : edges[vertex]) {
        if (2 * sizes[u] == static_cast<int>(sizes.size() - 1))
            res.push_back(u);
    }
    return res;
}

std::vector<std::vector<int>> ReadKingdom(int size) {
    std::vector<std::vector<int>> kingdom;
    kingdom.resize(size + 1);

    for (int i = 0; i < size - 1; ++i) {
        int from, to;
        std::cin >> from;
        std::cin >> to;

        kingdom[from].emplace_back(to);
        kingdom[to].emplace_back(from);
    }
    return kingdom;
}

std::shared_ptr<TTree> BuildCenteredTrees(int decompInd,
                                          const std::vector<std::vector<int>>& kingdom, int size) {
    std::vector<bool> visited(size + 1, false);
    return BuildTree(decompInd, kingdom, visited);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int size = 0;
    std::cin >> size;

    if (size == 1) {
        std::cout << 1 << std::endl;
        return 0;
    }
    auto firstKingdom = ReadKingdom(size);
    auto secondKingdom = ReadKingdom(size);

    auto firstDecomp = getCentroids(1, firstKingdom);

    std::shared_ptr<TTree> firstKingdomTree =
        BuildCenteredTrees(firstDecomp[0], firstKingdom, size);
    firstKingdom.clear();
    firstDecomp.clear();

    auto secondDecomp = getCentroids(1, secondKingdom);
    std::shared_ptr<TTree> secondKingdomTree =
        BuildCenteredTrees(secondDecomp[0], secondKingdom, size);

    int cnt = 1;

    GetTreeCode(firstKingdomTree, cnt);
    GetTreeCode(secondKingdomTree, cnt);

    std::vector<int> transitions;
    transitions.resize(size);
    if ((AreTreesEqual(firstKingdomTree, secondKingdomTree, transitions))) {
        for (auto transition : transitions) {
            std::cout << transition << "\n";
        }
        return 0;
    }
    if (secondDecomp.size() > 1) {
        secondKingdomTree = BuildCenteredTrees(secondDecomp[1], secondKingdom, size);
        secondKingdom.clear();
        GetTreeCode(secondKingdomTree, cnt);
        if ((AreTreesEqual(firstKingdomTree, secondKingdomTree, transitions))) {
            for (auto transition : transitions) {
                std::cout << transition << "\n";
            }
            return 0;
        }
    }

    std::cout << -1;

    return 0;
}
