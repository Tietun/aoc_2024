#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ranges>

inline auto constexpr input_file = "input.txt";

int main(void) {
    std::ifstream input(input_file);
    std::vector<std::vector<std::int_fast32_t>> reports = {};
    
    std::string line;
    while(std::getline(input, line)) {
        auto& report = reports.emplace_back(decltype(reports)::value_type {});
        
        std::istringstream line_stream(line);
        std::string token;
        while(std::getline(line_stream, token, ' ')) {
            report.push_back(std::stoi(token));
        }
    }
    
    auto const sorted = [](auto const& _range) {
        return std::ranges::is_sorted(_range) or std::ranges::is_sorted(_range, std::greater());
    };
    
    auto const gradual = [](auto const& _range) {
        return std::ranges::adjacent_find(_range, [](auto const _left, auto const _right){
            auto const delta = std::abs(_left - _right);
            return delta > 3 or delta < 1;
        }) == _range.end();
    };
    
    auto const safe = [&](auto const& _report) {
        auto const is_sorted = sorted(_report);
                                                        
        if(not is_sorted)
            return false;
                                                        
        return gradual(_report);
    };
    
    auto const safe_reports = std::transform_reduce(reports.cbegin(), reports.cend(), 0, std::plus(), safe);
    
    std::cout << "Day 2 Part 1: " << safe_reports << std::endl;

    auto const dampened_safe_reports = std::transform_reduce(reports.cbegin(), reports.cend(), 0, std::plus(),
    [&](decltype(reports)::value_type const& _report) {
        if(safe(_report))
            return true;
        
        auto const indices = std::views::iota(0) | std::views::take(_report.size());
        auto const alternative_safe = std::ranges::find_if(indices,
                             [&](auto const _element){
            auto alternative_report = _report;
            alternative_report.erase(alternative_report.cbegin() + _element);
            return safe(alternative_report);
        }) != std::ranges::end(indices);
        
        return alternative_safe;
    });
    
    std::cout << "Day 2 Part 2: " << dampened_safe_reports << std::endl;
    
    return EXIT_SUCCESS;
}
