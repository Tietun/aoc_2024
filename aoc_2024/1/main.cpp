#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <numeric>
#include <ranges>

inline auto constexpr input_file = "input.txt";

int main(void) {
    std::ifstream input(input_file);
    std::multiset<std::int_fast32_t> left_set, right_set;
    
    auto left = 0, right = 0;
    while(input >> left >> right) {
        left_set.insert(left);
        right_set.insert(right);
    }
    
    auto const list_delta = std::transform_reduce(left_set.cbegin(), left_set.cend(), right_set.cbegin(), 0, std::plus(), [](auto const _left, auto const _right){
        return std::abs(_left - _right);
    });
    
    std::cout << "Day 1 Part 1: " << list_delta << std::endl;
    
    auto const list_similarity = std::reduce(left_set.cbegin(), left_set.cend(), 0, [&](auto const _left, auto const _right){
        return _left + _right * std::count(right_set.cbegin(), right_set.cend(), _right);
    });
    
    std::cout << "Day 1 Part 2: " << list_similarity << std::endl;
    
    return EXIT_SUCCESS;
}
