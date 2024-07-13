#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>
#include <optional>

class HashFunction {
public:
    HashFunction() {};

    HashFunction(const int64_t aa, const int64_t bb)
            : aa_(aa), bb_(bb) {
    }

    int GetValue(const int key, const int size) const {
        int64_t result = (aa_ * key + bb_) % prime_;
        result = result % size;
        if (result < 0) {
            result += size;
        }
        return result;
    }

private:
    int64_t aa_;
    int64_t bb_;
    int64_t prime_ = 10000000019LL;
};

HashFunction GetRandomHashFunction(std::mt19937 &random_generator) {
    std::uniform_int_distribution<int64_t> a_distribution(1, 10000000019LL);
    std::uniform_int_distribution<int64_t> b_distribution(0, 10000000019LL);
    return {a_distribution(random_generator), b_distribution(random_generator)};
}

std::vector<size_t> FindKeyDistribution(const HashFunction &hash, const std::vector<int> &numbers) {
    size_t range = numbers.size();
    std::vector<size_t> distribution(numbers.size(), 0);
    for (auto num: numbers) {
        ++distribution[hash.GetValue(num, range)];
    }
    return distribution;
}

uint64_t CalculateSumOfSquares(const std::vector<size_t> &key_distribution) {
    uint64_t sum = 0;
    for (auto bucket_size: key_distribution) {
        sum += bucket_size * bucket_size;
    }

    return sum;
}

class Bucket {
public:
    void Initialize(const std::vector<int> &numbers, std::mt19937 &random_generator) {
        size_t bucket_size = numbers.size() * numbers.size();
        std::vector<size_t> distribution;
        do {
            boxes_.assign(bucket_size, {});
            hash_function_ = GetRandomHashFunction(random_generator);
            distribution = FindKeyDistribution(hash_function_, numbers);
        } while (CalculateSumOfSquares(distribution) != numbers.size());
        for (auto num: numbers) {
            size_t hash_index = hash_function_.GetValue(num, boxes_.size());
            boxes_[hash_index] = num;
        }
    }

    bool Contains(const int int_to_search) const {
        if (boxes_.empty()) {
            return false;
        }
        size_t hash_index = hash_function_.GetValue(int_to_search, boxes_.size());
        return (boxes_[hash_index].has_value() && boxes_[hash_index].value() == int_to_search);
    }

private:
    std::vector<std::optional<int>> boxes_;
    HashFunction hash_function_;
};

class FixedSet {
public:
    void Initialize(const std::vector<int> &numbers) {
        auto device = std::random_device{};
        std::mt19937 random_generator(device());
        size_ = numbers.size();
        buckets_.resize(size_);
        uint64_t memory_to_set = 3 * size_ + 1;
        while (memory_to_set > 3 * size_) {
            hash_function_ = GetRandomHashFunction(random_generator);
            std::vector<size_t> current_distribution = FindKeyDistribution(hash_function_, numbers);
            memory_to_set = CalculateSumOfSquares(current_distribution);
        }
        std::vector<std::vector<int>> bucket_ints;
        bucket_ints.resize(size_);
        for (const auto &number: numbers) {
            size_t hash_ind = hash_function_.GetValue(number, size_);
            bucket_ints[hash_ind].push_back(number);
        }
        for (size_t bucket_number = 0; bucket_number < size_; ++bucket_number) {
            buckets_[bucket_number].Initialize(bucket_ints[bucket_number], random_generator);
        }
    }

    bool Contains(int number) const {
        if (buckets_.empty()) {
            return false;
        }
        size_t hash_index = hash_function_.GetValue(number, size_);
        return buckets_[hash_index].Contains(number);
    }

private:
    std::vector<Bucket> buckets_;
    size_t size_;
    HashFunction hash_function_;
};


int rand() { // NOLINT
    throw std::runtime_error("Don't use rand");
}

std::vector<int> ReadSequence() {
    size_t size;
    std::cin >> size;
    std::vector<int> sequence(size);
    for (auto &current: sequence) {
        std::cin >> current;
    }
    return sequence;
}

std::vector<bool> PerformRequests(const std::vector<int> &requests, const FixedSet &set) {
    std::vector<bool> request_answers;
    request_answers.reserve(requests.size());
    for (int request: requests) {
        request_answers.push_back(set.Contains(request));
    }
    return request_answers;
}

void PrintRequestsResponse(const std::vector<bool> &request_answers) {
    for (bool answer: request_answers) {
        std::cout << (answer ? "Yes" : "No") << "\n";
    }
}

void RunTests();

int main(int argc, char **argv) {
    RunTests();
    if (argc > 1 && !strcmp(argv[1], "--testing")) {
        RunTests();
        return 0;
    }

    std::ios::sync_with_stdio(false);

    auto numbers = ReadSequence();
    auto requests = ReadSequence();
    FixedSet set;
    set.Initialize(numbers);
    PrintRequestsResponse(PerformRequests(requests, set));

    return 0;
}

// ========= TESTING ZONE =========

#define ASSERT_EQ(expected, actual)                                                               \
    do {                                                                                          \
        auto __expected = expected;                                                               \
        auto __actual = actual;                                                                   \
        if (!(__expected == __actual)) {                                                          \
            std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl;         \
            std::cerr << "\texpected: " << __expected << " (= " << #expected << ")" << std::endl; \
            std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl;          \
            std::cerr << "=========== FAIL ===========\n";                                        \
            throw std::runtime_error("Check failed");                                             \
        }                                                                                         \
    } while (false)

void Empty() {
    FixedSet set;
    set.Initialize({});
    ASSERT_EQ(false, set.Contains(0));
}

void Simple() {
    FixedSet set;
    set.Initialize({-3, 5, 0, 3, 7, 1});
    ASSERT_EQ(true, set.Contains(0));
    ASSERT_EQ(true, set.Contains(-3));
    ASSERT_EQ(true, set.Contains(1));
    ASSERT_EQ(false, set.Contains(2));
    ASSERT_EQ(false, set.Contains(4));
}

void RepeatInitialize() {
    FixedSet set;
    const int shift = 100;
    int element = 0;
    int last = -1;
    for (int elements_count = 0; elements_count < 10; ++elements_count) {
        std::vector<int> elements;
        for (int i = 0; i < elements_count; ++i) {
            elements.push_back(element++);
        }
        set.Initialize(elements);
        for (auto elem: elements) {
            ASSERT_EQ(true, set.Contains(elem));
        }
        ASSERT_EQ(false, set.Contains(last));
        last = element - 1;
        element += shift;
    }
}

void Magic() {
#ifdef MAGIC
    std::cerr << "You've been visited by Hash Police!\n";
    std::cerr << "Probably your hash table is not as good as you think.\n";
    std::cerr << "No ticket today, but you better be careful.\n\n";
    int first = -1'000'000'000;
    int second = first + MAGIC;
    FixedSet set;
    set.Initialize({first, second});
    ASSERT_EQ(true, set.Contains(first));
    ASSERT_EQ(true, set.Contains(second));
    ASSERT_EQ(false, set.Contains(0));
#endif
}

void RunTests() {
    std::cerr << "Running tests...\n";
    Empty();
    Simple();
    RepeatInitialize();
    Magic();
    std::cerr << "Tests are passed!\n";
}
