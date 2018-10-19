#include <string>
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include "zip_adaptor.h"

int main() {
	const std::vector<int> test_1 = { 1, 2, 3, 4, 5 };
	std::vector<int> test_2 = { 1, 2, 3, 4, 5 };
	for(auto [a, b] : make_zip_adaptor(test_1, test_2)) {
		std::cout << a << "; " << b << std::endl;
	}

	constexpr std::array test_3 = { 1, 2, 3, 4, 5 };
	constexpr std::array test_4 = { 1, 2, 3, 4, 5 };
	for(auto [a, b, c] : make_zip_adaptor(test_3, test_4, test_1)) {
		std::cout << a << "; " << b << std::endl;
	}

	const std::unordered_set<int> test_5 = { 1, 2, 3, 4, 5 };
	std::unordered_map<int, std::string> test_6 = { { 1, "Hello" }, { 2, "I" }, { 3, "am" }, { 4, "a" }, { 5, "map" } };
	for(auto [a, b] : make_zip_adaptor(test_5, test_6)) {
		auto& [c, d] = b;
		d += " f*ed up";
		std::cout << a << "; " << c << " == " << d << std::endl;
	}
	for(auto [a, b] : make_zip_adaptor(test_5, test_6)) {
		auto& [c, d] = b;
		std::cout << a << "; " << c << " == " << d << std::endl;
	}
}