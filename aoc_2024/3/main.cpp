#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <ranges>

inline auto constexpr input_file = "input.txt";

int main(void) {
    std::ifstream input(input_file);
    std::string const file(std::istreambuf_iterator(input), {});
    
    std::regex const mul_regex(R"(mul\(\d+,\d+\))");
    
    auto const multiply = [](std::string const& _string)
    {
        std::regex const number_regex(R"(\d+)");
        
        return std::transform_reduce(std::sregex_token_iterator(_string.cbegin(), _string.cend(), number_regex),
                                     std::sregex_token_iterator(), 1, std::multiplies(), [](auto const _smatch) {
            return std::stoi(_smatch.str());
        });
    };
    
    auto const multiplications = std::transform_reduce(std::sregex_iterator(file.cbegin(), file.cend(), mul_regex), std::sregex_iterator(), 0, std::plus(), [&](auto const _mul){
        return multiply(_mul.str());
    });
    
    std::cout << "Day 3 Part 1: " << multiplications << std::endl;
    
    std::regex const dont_do_regex(R"(don't\(\)([\S\s]*?)do\(\))");
    auto const enabled_mul_file = std::regex_replace(file, dont_do_regex, "");
    
    std::cout << enabled_mul_file << std::endl;
    
    auto const enabled_multiplications = std::transform_reduce(std::sregex_iterator(enabled_mul_file.cbegin(), enabled_mul_file.cend(), mul_regex), std::sregex_iterator(), 0, std::plus(), [&](auto const _mul) {
        return multiply(_mul.str());
    });
    
    std::cout << "Day 3 Part 2: " << enabled_multiplications << std::endl;
    
    return EXIT_SUCCESS;
}
