#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

template <class T>
class MyStack {
public:
    void Push(T value) {
        data_.push(value);
        if (maxValue_.empty() || maxValue_.top() < value)
            maxValue_.push(value);
        else
            maxValue_.push(maxValue_.top());
    }

    void Pop() {
        data_.pop();
        maxValue_.pop();
    }

    T Top() const {
        return data_.top();
    }

    T GetMax() const {
        return maxValue_.top();
    }

    bool Empty() const {
        return data_.empty();
    };

private:
    std::stack<T> data_;
    std::stack<T> maxValue_;
};

template <class T>
class MyQueue {
public:
    void Queue(T value) {
        inputStack_.Push(value);
    }

    T Enqueue() {
        if (outputStack_.Empty()) {
            while (!inputStack_.Empty()) {
                outputStack_.Push(inputStack_.Top());
                inputStack_.Pop();
            }
        }
        auto value = outputStack_.Top();
        outputStack_.Pop();
        return value;
    }

    T GetMax() {
        if (!inputStack_.Empty() && !outputStack_.Empty())
            return std::max(inputStack_.GetMax(), outputStack_.GetMax());
        else if (!inputStack_.Empty())
            return inputStack_.GetMax();
        return outputStack_.GetMax();
    }

private:
    MyStack<T> inputStack_;
    MyStack<T> outputStack_;
};

// 10
// 1 4 2 3 5 8 6 7 9 10
// 12
// R R L R R R L L L R L L

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int numbersCnt = 0;
    std::cin >> numbersCnt;
    std::vector<int> nums;
    nums.reserve(numbersCnt);

    for (int i = 0; i < numbersCnt; ++i) {
        int val;
        std::cin >> val;
        nums.emplace_back(val);
    }

    int commandsCnt = 0;
    std::cin >> commandsCnt;

    MyQueue<int> myQueue;
    myQueue.Queue(nums[0]);
    int rPos = 0;

    for (int i = 0; i < commandsCnt; ++i) {
        char command;
        std::cin >> command;
        if (command == 'R') {
            myQueue.Queue(nums[++rPos]);
            std::cout << myQueue.GetMax() << "\n";
        } else {
            myQueue.Enqueue();
            std::cout << myQueue.GetMax() << "\n";
        }
    }

    return 0;
}
