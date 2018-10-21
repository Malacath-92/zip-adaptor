#include "runtime.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#ifndef SKIP_RUNTIME_TESTS

void homogeneous_containers_homogeneous_types_const_iteration() {
	const std::vector<int> first = []() {
		std::vector<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	const std::vector<int> second = []() {
		std::vector<int> second(50);
		std::generate(second.begin(), second.end(), [i = 0, j = 1]() mutable { return (++i) * (++j); });
		return second;
	}();
	const std::vector<int> third = [&]() {
		std::vector<int> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return second[i++] / (first[j++] + 1); });
		return third;
	}();

	for(auto[a, b, c] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: typeof(a) != const int&");
		static_assert(std::is_same_v<decltype(b), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: typeof(b) != const int&");
		static_assert(std::is_same_v<decltype(c), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: typeof(c) != const int&");
		if(b != (a + 1) * (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": b != (a + 1) * (a + 2)");
		if(c != b / (a + 1))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": c != a / b");
	}
}
void homogeneous_containers_heterogeneous_types_const_iteration() {
	const std::vector<int> first = []() {
		std::vector<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	const std::vector<float> second = []() {
		std::vector<float> second(50);
		std::generate(second.begin(), second.end(), [i = 0.0f, j = 1]() mutable { return (++i) / (++j); });
		return second;
	}();
	const std::vector<std::string> third = [&]() {
		std::vector<std::string> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return std::to_string(std::ceil(second[i++] * first[j++] + 1)); });
		return third;
	}();

	for(auto[a, b, c] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: typeof(a) != const int&");
		static_assert(std::is_same_v<decltype(b), const float&>, "homogeneous_containers_homogeneous_types_const_iteration failed: typeof(b) != const float&");
		static_assert(std::is_same_v<decltype(c), const std::string&>, "homogeneous_containers_homogeneous_types_const_iteration failed: typeof(c) != const std::string&");
		
		if(b != (float(a) + 1.0f) / (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": b != (float(a) + 1.0f) * (a + 2)");
		if(c != std::to_string(std::ceil(b * (a + 1))))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": c != std::to_string(std::floor(b) / (a + 1))");
	}
}
void heterogeneous_containers_homogeneous_types_const_iteration() {
	const std::deque<int> first = []() {
		std::deque<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	const std::map<int, int> second = []() {
		std::map<int, int> second;
		auto gen = [i = 0, j = 1]() mutable { return (++i) / (++j); };
		for(int i = 0; i < 50; i++) second.insert(std::make_pair(i, gen()));
		return second;
	}();
	const std::list<int> third = [&]() {
		std::list<int> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return second.at(i++) * (first[j++] + 1); });
		return third;
	}();

	for(auto[a, bc, d] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: decltype(a) != const int&");
		static_assert(std::is_same_v<decltype(bc), const std::pair<const int, int>&>, "homogeneous_containers_homogeneous_types_const_iteration failed: decltype(bc) != const std::pair<const int, int>&");
		static_assert(std::is_same_v<decltype(d), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: decltype(d) != const int&");
		
		auto&[b, c] = bc;
		if(b != a)
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": b != a");
		if(c != (a + 1) / (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": c != (a + 1) / (a + 2)");
		if(d != (c * (a + 1)))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": d != (c * (a + 1))");
	}
}
void heterogeneous_containers_heterogeneous_types_const_iteration() {
	const std::deque<int> first = []() {
		std::deque<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	const std::map<int, float> second = []() {
		std::map<int, float> second;
		auto gen = [i = 0.0f, j = 1]() mutable { return (++i) / (++j); };
		for(int i = 0; i < 50; i++) second.insert(std::make_pair(i, gen()));
		return second;
	}();
	const std::list<std::string> third = [&]() {
		std::list<std::string> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return std::to_string(std::ceil(second.at(i++) * first[j++] + 1)); });
		return third;
	}();

	for(auto[a, bc, d] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), const int&>, "homogeneous_containers_homogeneous_types_const_iteration failed: decltype(a) != const int&");
		static_assert(std::is_same_v<decltype(bc), const std::pair<const int, float>&>, "homogeneous_containers_homogeneous_types_const_iteration failed: decltype(bc) != const std::pair<const int, float>&");
		static_assert(std::is_same_v<decltype(d), const std::string&>, "homogeneous_containers_homogeneous_types_const_iteration failed: decltype(d) != const std::string&");
		
		auto&[b, c] = bc;
		if(b != a)
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": b != a");
		if(c != (float(a) + 1.0f) / (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": c != (float(a) + 1.0f) / (a + 2)");
		if(d != std::to_string(std::ceil(c * (a + 1))))
			throw std::logic_error("homogeneous_containers_homogeneous_types_const_iteration " + failed + ": c != std::to_string(std::floor(b) / (a + 1))");
	}
}

void homogeneous_containers_homogeneous_types_non_const_iteration() {
}
void homogeneous_containers_heterogeneous_types_non_const_iteration() {
}
void heterogeneous_containers_homogeneous_types_non_const_iteration() {
}
void heterogeneous_containers_heterogeneous_types_non_const_iteration() {
}

void no_copies_no_moves_iteration() {
}

// Wrong sizes
// vector<bool>
void exceptions() {
}

#endif