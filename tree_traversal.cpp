#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <utility>
#include <vector>

struct TTree {
    TTree* left;
    TTree* right;
    int val;
};

struct TCntTree {
    TCntTree* left;
    TCntTree* right;
    TCntTree* parrent;
    int val;
    int cnt;
};

void Insert(TCntTree** root, TCntTree* parrent, int val) {
    if (!*root) {
        if (parrent && parrent->val == val) {
            parrent->cnt++;
            return;
        }
        *root = new TCntTree();
        (*root)->val = val;
        (*root)->cnt = 1;
        return;
    }
    if ((*root)->val > val)
        Insert(&(*root)->left, *root, val);
    else
        Insert(&(*root)->right, *root, val);
}

void Insert(TTree** root, int val) {
    if (!*root) {
        *root = new TTree();
        (*root)->val = val;
        return;
    }
    if ((*root)->val > val)
        Insert(&(*root)->left, val);
    else
        Insert(&(*root)->right, val);
}

void Inorder(TTree* root, std::vector<int>& res) {
    if (root->left)
        Inorder(root->left, res);
    std::cout << root->val << " ";
    res.emplace_back(root->val);
    if (root->right)
        Inorder(root->right, res);
}

void Inorder(TCntTree* root, std::vector<int>& res) {
    if (root->left)
        Inorder(root->left, res);
    for (int i = 0; i < root->cnt; ++i) {
        std::cout << root->val << " ";
        res.emplace_back(root->val);
    }
    if (root->right)
        Inorder(root->right, res);
}

void Postorder(TCntTree* root, std::vector<int>& res) {
    if (root->left)
        Postorder(root->left, res);
    if (root->right)
        Postorder(root->right, res);
    for (int i = 0; i < root->cnt; ++i) {
        std::cout << root->val << " ";
        res.emplace_back(root->val);
    }
}

void Postorder(TTree* root, std::vector<int>& res) {
    if (root->left)
        Postorder(root->left, res);
    if (root->right)
        Postorder(root->right, res);
    std::cout << root->val << " ";
    res.emplace_back(root->val);
}

void Clear(TTree** rootPtr) {
    TTree* root = *rootPtr;
    if (root->left != nullptr)
        Clear(&root->left);
    if (root->right != nullptr)
        Clear(&root->right);
    delete root;
    *rootPtr = nullptr;
}

void Clear(TCntTree** rootPtr) {
    auto* root = *rootPtr;
    if (root->left != nullptr)
        Clear(&root->left);
    if (root->right != nullptr)
        Clear(&root->right);
    delete root;
    *rootPtr = nullptr;
}

TTree* GenerateRandomTreeImpl(int min, int max, unsigned depth, std::mt19937& generator) {
    if (min > max || depth == 0) {
        return nullptr;
    }

    std::uniform_int_distribution<int> dist(min, max);
    int root_value = dist(generator);

    TTree* root = new TTree();
    root->val = root_value;

    root->left = GenerateRandomTreeImpl(min, root_value - 1, depth - 1, generator);
    root->right = GenerateRandomTreeImpl(root_value, max, depth - 1, generator);

    return root;
}

TTree* GenerateRandomTree(int min, int max, unsigned depth) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());

    return GenerateRandomTreeImpl(min, max, depth, generator);
}

void PreorderTraversal(TTree* root, std::vector<int>& values) {
    if (!root) {
        return;
    }

    values.push_back(root->val);
    PreorderTraversal(root->left, values);
    PreorderTraversal(root->right, values);
}

std::vector<int> GetPreorderVector(TTree* root) {
    std::vector<int> values;
    PreorderTraversal(root, values);
    return values;
}

void test() {
    srand(time(0));
    TCntTree* rootCnt = nullptr;
    TTree* root = nullptr;
    TTree* random_tree = GenerateRandomTree(-10, 10, 3);
    std::vector<int> preorder_vector = GetPreorderVector(random_tree);

    for (auto val : preorder_vector) {
        std::cout << val << " ";
        Insert(&root, val);
        Insert(&rootCnt, nullptr, val);
    }
    std::cout << std::endl;
    std::vector<int> cntRes;
    std::vector<int> res;
    Inorder(root, res);
    std::cout << std::endl;
    Inorder(rootCnt, cntRes);
    if (res != cntRes)
        std::cout << "FAIL" << std::endl;
    else
        std::cout << "OK" << std::endl;
}

void Postorder(const std::vector<int>& nums, int min, int max, size_t& ind) {
    if (ind >= nums.size() || nums[ind] < min || nums[ind] > max)
        return;

    int currVal = nums[ind];
    ++ind;
    Postorder(nums, min, currVal - 1, ind);
    Postorder(nums, currVal, max, ind);
    std::cout << currVal << " ";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int cnt;
    std::cin >> cnt;

    if (cnt == 0)
        return 0;

    std::vector<int> nums;
    nums.reserve(cnt);
    for (int i = 0; i < cnt; ++i) {
        int val;
        std::cin >> val;
        nums.emplace_back(val);
    }

    size_t ind = 0;
    Postorder(nums, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), ind);
    std::cout << std::endl;

    std::sort(nums.begin(), nums.end());

    for (const auto n : nums)
        std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
