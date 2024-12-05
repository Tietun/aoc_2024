#include <fstream>
#include <sstream>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <regex>
#include <numeric>

inline auto constexpr input_file = "input.txt";

int main(void) {
    std::ifstream input(input_file);
    
    std::vector<std::string> horizontal_lines = {};
    for(std::string line = {}; std::getline(input, line);)
        horizontal_lines.emplace_back(line);
    
    auto const line_length = std::ranges::begin(horizontal_lines)->size();
    auto const indices = std::views::iota(0) | std::views::take(line_length);
    auto const short_indices = std::views::iota(0) | std::views::take(line_length - 1);
    
    auto const vertical_lines = std::views::transform(indices, [&](auto const _index){
        return std::views::transform(horizontal_lines, [_index](auto const& _line){
            return _line.at(_index);
        }) | std::ranges::to<std::string>();
    }) | std::ranges::to<std::vector<std::string>>();
    
    std::vector<std::string> first_ascending_diagonal_lines = std::views::transform(indices, [&](auto const _index){
        auto const above_indices = std::views::iota(0) | std::views::take(_index + 1);
        return std::views::transform(above_indices, [&, _index](auto const _above_index) {
            return horizontal_lines.at(_index - _above_index).at(_above_index);
        }) | std::ranges::to<std::string>();
    }) | std::ranges::to<std::vector<std::string>>();
    
    auto const inverse_horizontal_lines = std::views::transform(std::views::reverse(horizontal_lines), [](auto const& _line){
        return std::views::reverse(_line) | std::ranges::to<std::string>();
    }) | std::ranges::to<std::vector<std::string>>();
    
    auto const reverse_horizontal_lines = std::views::transform(horizontal_lines, [](auto const& _line){
        return std::views::reverse(_line) | std::ranges::to<std::string>();
    }) | std::ranges::to<std::vector<std::string>>();
    
    auto const upside_down_horizontal_lines = std::views::reverse(horizontal_lines) | std::ranges::to<decltype(horizontal_lines)>();
    
    std::vector<std::string> const second_ascending_diagonal_lines = std::views::transform(short_indices, [&](auto const _index){
        auto const above_indices = std::views::iota(0) | std::views::take(_index + 1);
        return std::views::transform(above_indices, [&, _index](auto const _above_index) {
            return inverse_horizontal_lines.at(_index - _above_index).at(_above_index);
        }) | std::ranges::to<std::string>();
    }) | std::ranges::to<std::vector<std::string>>();
    
    first_ascending_diagonal_lines.append_range(std::views::reverse(second_ascending_diagonal_lines));
    
    std::vector<std::string> first_descending_diagonal_lines = std::views::transform(indices, [&](auto const _index){
        auto const above_indices = std::views::iota(0) | std::views::take(_index + 1);
        return std::views::transform(above_indices, [&, _index](auto const _above_index) {
            return reverse_horizontal_lines.at(_index - _above_index).at(_above_index);
        }) | std::ranges::to<std::string>();
    }) | std::ranges::to<std::vector<std::string>>();
    
    auto const second_descending_diagonal_lines = std::views::transform(short_indices, [&](auto const _index){
        auto const above_indices = std::views::iota(0) | std::views::take(_index + 1);
        return std::views::transform(above_indices, [&, _index](auto const _above_index) {
            return upside_down_horizontal_lines.at(_index - _above_index).at(_above_index);
        }) | std::ranges::to<std::string>();
    });
    
    first_descending_diagonal_lines.append_range(std::views::reverse(second_descending_diagonal_lines));
    
    std::regex const xmas_regex(R"(XMAS)");
    std::regex const samx_regex(R"(SAMX)");
    auto const glue = [](auto const& _left, auto const& _right){
        return _left + "\n" + _right;
    };
    
    auto const horizontal = std::accumulate(horizontal_lines.cbegin(), horizontal_lines.cend(), std::string{}, glue);
    auto const vertical = std::accumulate(vertical_lines.cbegin(), vertical_lines.cend(), std::string{}, glue);
    auto const ascending = std::accumulate(first_ascending_diagonal_lines.cbegin(), first_ascending_diagonal_lines.cend(), std::string{}, glue);
    auto const descending = std::accumulate(first_descending_diagonal_lines.cbegin(), first_descending_diagonal_lines.cend(), std::string{}, glue);
    auto const all = horizontal + vertical + ascending + descending;
    
    auto const first_xmas = std::distance(std::sregex_iterator(all.cbegin(), all.cend(), xmas_regex), std::sregex_iterator()) +
    std::distance(std::sregex_iterator(all.cbegin(), all.cend(), samx_regex), std::sregex_iterator());
    
    std::cout << "Day 4 Part 1: " << first_xmas << std::endl;
    
    std::regex const
    first_x("(?=(M.S([\\s\\S]){139}A([\\s\\S]){139}M.S))"),
    second_x("(?=(S.S([\\s\\S]){139}A([\\s\\S]){139}M.M))"),
    third_x("(?=(S.M([\\s\\S]){139}A([\\s\\S]){139}S.M))"),
    fourth_x("(?=(M.M([\\s\\S]){139}A([\\s\\S]){139}S.S))");
    
    auto const second_xmas = std::distance(std::sregex_iterator(horizontal.cbegin(), horizontal.cend(), first_x), std::sregex_iterator()) +
    std::distance(std::sregex_iterator(horizontal.cbegin(), horizontal.cend(), second_x), std::sregex_iterator()) +
    std::distance(std::sregex_iterator(horizontal.cbegin(), horizontal.cend(), third_x), std::sregex_iterator()) +
    std::distance(std::sregex_iterator(horizontal.cbegin(), horizontal.cend(), fourth_x), std::sregex_iterator());
    
    std::cout << "Day 4 Part 2: " << second_xmas << std::endl;
    
    return EXIT_SUCCESS;
}
