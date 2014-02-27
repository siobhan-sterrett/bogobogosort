#include <iostream>
#include <vector>
#include <random>
#include <cstdint>
#include <chrono>

#define BYTE_SIZE 8

template <typename InputIterator>
constexpr bool isSorted(InputIterator begin, InputIterator end) {
    return (begin + 1 == end) ? true :
           (*begin <= *(begin + 1)) ? isSorted(begin + 1, end) :
           false;
}

template <typename InputIterator>
constexpr size_t elemCount(InputIterator begin, InputIterator end,
                       typename InputIterator::value_type val) {
    return (begin == end) ? 0 :
           ((*begin == val) ? 1 : 0) + elemCount(begin + 1, end, val);
}

template <typename InputIterator>
bool containsSame(InputIterator begin_a, InputIterator end_a,
                  InputIterator begin_b, InputIterator end_b) {
    size_t count_a, count_b;
    for (InputIterator it = begin_a; it != end_a; ++it) {
        count_a = elemCount(begin_a, end_a, *it);
        count_b = elemCount(begin_b, end_b, *it);
        if (count_a != count_b) {
            return false;
        }
    }
    return true;
}

template <typename InputIterator>
void bogobogosort(InputIterator begin, InputIterator end) {
    const unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    const size_t ELEMENT_SIZE = BYTE_SIZE * sizeof(*begin);
    const size_t BIT_LENGTH = ELEMENT_SIZE * (end - begin);
    std::default_random_engine generator(SEED);
    std::uniform_int_distribution< size_t > distribution(0, BIT_LENGTH - 1);
    size_t pos, index, bit;
    uintmax_t mask;
    std::vector<typename InputIterator::value_type> original(begin, end);
    do {
        pos = distribution(generator);
        index = pos / ELEMENT_SIZE;
        bit = pos % ELEMENT_SIZE;
        mask = 1 << bit;
        begin[index] ^= mask;
    } while (!isSorted(begin, end) or
             !containsSame(begin, end, original.begin(), original.end()));
}

int main(int argc, char **argv) {
    size_t n = 0;
    if (argc > 1) {
        n = atoi(argv[1]);
        if (!n) return 0;
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<char> distribution('a', 'z');
    std::vector<char> vec;
    for (size_t i = 0; i < n; ++i) vec.push_back(distribution(generator));
    for (char c : vec) std::cout << c << ' ';
    std::cout << std::endl;
    bogobogosort(vec.begin(), vec.end());
    for (char c : vec) std::cout << c << ' ';
    std::cout << std::endl;
    return 0;
}
