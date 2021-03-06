#include "runtime.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>

#define ZIP_NAMESPACE
#include "zip_adaptor.h"

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
	std::vector<int> first = []() {
		std::vector<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	std::vector<int> second = []() {
		std::vector<int> second(50);
		std::generate(second.begin(), second.end(), [i = 0, j = 1]() mutable { return (++i) * (++j); });
		return second;
	}();
	std::vector<int> third = [&]() {
		std::vector<int> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return second[i++] / (first[j++] + 1); });
		return third;
	}();

	for(auto[a, b, c] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(a) != int&");
		static_assert(std::is_same_v<decltype(b), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(b) != int&");
		static_assert(std::is_same_v<decltype(c), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(c) != int&");

		if(b != (a + 1) * (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != (a + 1) * (a + 2)");
		if(c != b / (a + 1))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != a / b");

		a = 0;
		b = 1;
		c = -1;
	}

	for(auto[a, b, c] : zip(first, second, third)) {
		if(a != 0)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": a != 0");
		if(b != 1)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != 1");
		if(c != -1)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != -1");
	}
}
void homogeneous_containers_heterogeneous_types_non_const_iteration() {
	std::vector<int> first = []() {
		std::vector<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	std::vector<float> second = []() {
		std::vector<float> second(50);
		std::generate(second.begin(), second.end(), [i = 0.0f, j = 1]() mutable { return (++i) / (++j); });
		return second;
	}();
	std::vector<std::string> third = [&]() {
		std::vector<std::string> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return std::to_string(std::ceil(second[i++] * first[j++] + 1)); });
		return third;
	}();

	for(auto[a, b, c] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(a) != int&");
		static_assert(std::is_same_v<decltype(b), float&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(b) != float&");
		static_assert(std::is_same_v<decltype(c), std::string&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(c) != std::string&");

		if(b != (float(a) + 1.0f) / (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != (float(a) + 1.0f) * (a + 2)");
		if(c != std::to_string(std::ceil(b * (a + 1))))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != std::to_string(std::floor(b) / (a + 1))");

		a = 111;
		b = std::numeric_limits<float>::quiet_NaN();
		c = "zero";
	}

	for(auto[a, b, c] : zip(first, second, third)) {
		if(a != 111)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": a != 111");
		if(b == b)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b == b");
		if(c != "zero")
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != \"zero\"");
	}
}
void heterogeneous_containers_homogeneous_types_non_const_iteration() {
	std::deque<int> first = []() {
		std::deque<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	std::map<int, int> second = []() {
		std::map<int, int> second;
		auto gen = [i = 0, j = 1]() mutable { return (++i) / (++j); };
		for(int i = 0; i < 50; i++) second.insert(std::make_pair(i, gen()));
		return second;
	}();
	std::list<int> third = [&]() {
		std::list<int> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return second.at(i++) * (first[j++] + 1); });
		return third;
	}();

	for(auto[a, bc, d] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: decltype(a) != int&");
		static_assert(std::is_same_v<decltype(bc), std::pair<const int, int>&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: decltype(bc) != std::pair<const int, int>&");
		static_assert(std::is_same_v<decltype(d), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: decltype(d) != int&");

		auto&[b, c] = bc;
		if(b != a)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != a");
		if(c != (a + 1) / (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != (a + 1) / (a + 2)");
		if(d != (c * (a + 1)))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": d != (c * (a + 1))");

		a = -666;
		c = 0;
		d = 666;
	}

	for(auto[a, bc, d] : zip(first, second, third)) {
		auto&[b, c] = bc;
		if(a != -666)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": a != -666");
		if(c != 0)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != 0");
		if(d != 666)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": d != 666");
	}
}
void heterogeneous_containers_heterogeneous_types_non_const_iteration() {
	std::deque<int> first = []() {
		std::deque<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	std::map<int, float> second = []() {
		std::map<int, float> second;
		auto gen = [i = 0.0f, j = 1]() mutable { return (++i) / (++j); };
		for(int i = 0; i < 50; i++) second.insert(std::make_pair(i, gen()));
		return second;
	}();
	std::list<std::string> third = [&]() {
		std::list<std::string> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return std::to_string(std::ceil(second.at(i++) * first[j++] + 1)); });
		return third;
	}();

	for(auto[a, bc, d] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: decltype(a) != int&");
		static_assert(std::is_same_v<decltype(bc), std::pair<const int, float>&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: decltype(bc) != std::pair<const int, float>&");
		static_assert(std::is_same_v<decltype(d), std::string&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: decltype(d) != std::string&");

		auto&[b, c] = bc;
		if(b != a)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != a");
		if(c != (float(a) + 1.0f) / (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != (float(a) + 1.0f) / (a + 2)");
		if(d != std::to_string(std::ceil(c * (a + 1))))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != std::to_string(std::floor(b) / (a + 1))");

		a = 123;
		c = std::numeric_limits<float>::infinity();
		d = "not infinity";
	}

	for(auto[a, bc, d] : zip(first, second, third)) {
		auto&[b, c] = bc;
		if(a != 123)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": a != 123");
		if(c != std::numeric_limits<float>::infinity())
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != std::numeric_limits<float>::inifinity()");
		if(d != "not infinity")
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": d != \"not infinity\"");
	}
}

void rvalue_iteration() {
	auto zipper = []() {
		std::vector<int> first = []() {
			std::vector<int> first(50);
			std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
			return first;
		}();
		std::vector<int> second = []() {
			std::vector<int> second(50);
			std::generate(second.begin(), second.end(), [i = 0, j = 1]() mutable { return (++i) * (++j); });
			return second;
		}();
		std::vector<int> third = [&]() {
			std::vector<int> third(50);
			std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return second[i++] / (first[j++] + 1); });
			return third;
		}();

		return zip(std::move(first), std::move(second), std::move(third));
	}();

	if (zipper.size() != 50)
		throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": zipper.size() != 50");

	for(auto[a, b, c] : zipper) {
		static_assert(std::is_same_v<decltype(a), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(a) != int&");
		static_assert(std::is_same_v<decltype(b), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(b) != int&");
		static_assert(std::is_same_v<decltype(c), int&>, "homogeneous_containers_homogeneous_types_non_const_iteration failed: typeof(c) != int&");

		if(b != (a + 1) * (a + 2))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != (a + 1) * (a + 2)");
		if(c != b / (a + 1))
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != a / b");

		a = 0;
		b = 1;
		c = -1;
	}

	for(auto[a, b, c] : zipper) {
		if(a != 0)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": a != 0");
		if(b != 1)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": b != 1");
		if(c != -1)
			throw std::logic_error("homogeneous_containers_homogeneous_types_non_const_iteration " + failed + ": c != -1");
	}
}

void no_copies_iteration() {
	struct ExpensiveToCopy {
		ExpensiveToCopy() = delete;
		constexpr ExpensiveToCopy(const ExpensiveToCopy&) = delete;
		constexpr ExpensiveToCopy(ExpensiveToCopy&&) noexcept = default;
		constexpr ExpensiveToCopy& operator=(const ExpensiveToCopy&) = delete;
		constexpr ExpensiveToCopy& operator=(ExpensiveToCopy&&) noexcept = default;
		constexpr ExpensiveToCopy(int _v) : v(_v) {}
		int v;
	};

	auto zipped_range = zip(
		[]() {
			std::array<ExpensiveToCopy, 5> first = { 0, 1, 2, 3, 4 };
			return std::vector(std::make_move_iterator(first.begin()), std::make_move_iterator(first.end()));
		}(),
		[]() {
			std::array<ExpensiveToCopy, 5> first = { 3, 6, 9, 12, 15 };
			return std::vector(std::make_move_iterator(first.begin()), std::make_move_iterator(first.end()));
		}(),
		[]() {
			std::array<ExpensiveToCopy, 5> first = { 0, 6, 18, 36, 60 };
			return std::vector(std::make_move_iterator(first.begin()), std::make_move_iterator(first.end()));
		}()
	);

	for(auto[a, b, c] : zipped_range) {
		static_assert(std::is_same_v<decltype(a), ExpensiveToCopy&>, "no_copies_iteration failed: decltpye(a) != ExpensiveToCopy&");
		static_assert(std::is_same_v<decltype(b), ExpensiveToCopy&>, "no_copies_iteration failed: decltpye(b) != ExpensiveToCopy&");
		static_assert(std::is_same_v<decltype(c), ExpensiveToCopy&>, "no_copies_iteration failed: decltpye(c) != ExpensiveToCopy&");
		if(b.v != (a.v + 1) * 3)
			throw std::logic_error("no_copies_iteration " + failed + ": b.v != (a.v + 1) * 3");
		if(c.v != a.v * b.v)
			throw std::logic_error("no_copies_iteration " + failed + ": c.v != a.v * b.v");
	}
}
void no_copies_no_moves_iteration() {
	struct ExpensiveToCopyOrMove {
		ExpensiveToCopyOrMove() = delete;
		constexpr ExpensiveToCopyOrMove(const ExpensiveToCopyOrMove&) = delete;
		constexpr ExpensiveToCopyOrMove(ExpensiveToCopyOrMove&&) = delete;
		constexpr ExpensiveToCopyOrMove& operator=(const ExpensiveToCopyOrMove&) = delete;
		constexpr ExpensiveToCopyOrMove& operator=(ExpensiveToCopyOrMove&&) = delete;
		constexpr ExpensiveToCopyOrMove(int _v) : v(_v) {}
		int v;
	};

	const std::array<ExpensiveToCopyOrMove, 5> first = { 0, 1, 2, 3, 4 };
	const std::array<ExpensiveToCopyOrMove, 5> second = { 3, 6, 9, 12, 15 };
	const std::array<ExpensiveToCopyOrMove, 5> third = { 0, 6, 18, 36, 60 };

	for(auto[a, b, c] : zip(first, second, third)) {
		static_assert(std::is_same_v<decltype(a), const ExpensiveToCopyOrMove&>, "no_copies_no_moves_iteration failed: decltpye(a) != const ExpensiveToCopyOrMove&");
		static_assert(std::is_same_v<decltype(b), const ExpensiveToCopyOrMove&>, "no_copies_no_moves_iteration failed: decltpye(b) != const ExpensiveToCopyOrMove&");
		static_assert(std::is_same_v<decltype(c), const ExpensiveToCopyOrMove&>, "no_copies_no_moves_iteration failed: decltpye(c) != const ExpensiveToCopyOrMove&");
		if(b.v != (a.v + 1) * 3)
			throw std::logic_error("no_copies_no_moves_iteration " + failed + ": b.v != (a.v + 1) * 3");
		if(c.v != a.v * b.v)
			throw std::logic_error("no_copies_no_moves_iteration " + failed + ": c.v != a.v * b.v");
	}
}

void exceptions() {
	try {
		const std::vector<int> small(10, 0);
		const std::vector<int> large(100, 0);
		[[maybe_unused]] const auto zipper = zip(small, large);
		throw std::logic_error("exceptions " + failed + ": std::length_error was not thrown");
	}
	catch(std::length_error& e) {
		std::cout << "std::length_error caught: " << e.what() << std::endl;
	}
}

void erase_const() {
	std::vector<int> first(100);
	std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
	std::list<int> second(100);
	std::generate(second.begin(), second.end(), []() { return rand(); });
	std::map<int, int> third;
	for(int i = 0; i < 100; i++) third.insert(std::make_pair(i, rand()));
	std::set<int> fourth;
	for(int i = 0; i < 100; i++) fourth.insert(rand());

	auto zipper = zip(first, second, third, fourth);
	for(auto it = zipper.cbegin(); it != zipper.cend();) {
		auto[a, b, c, d] = *it;
		if(a % 2 == 0) {
			it = zipper.erase(it);
		}
		else {
			it++;
		}
	}
	if(zipper.size() != 50)
		throw std::logic_error("erase_const " + failed + ": zipper.size() != 50");

	zipper.erase(zipper.cbegin() + 10, zipper.cend());
	if(zipper.size() != 10)
		throw std::logic_error("erase_const " + failed + ": zipper.size() != 10");
}
void erase_non_const() {
	std::vector<int> first(100);
	std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
	std::list<int> second(100);
	std::generate(second.begin(), second.end(), []() { return rand(); });
	std::map<int, int> third;
	for(int i = 0; i < 100; i++) third.insert(std::make_pair(i, rand()));
	std::set<int> fourth;
	for(int i = 0; i < 100; i++) fourth.insert(rand());

	auto zipper = zip(first, second, third, fourth);
	for(auto it = zipper.begin(); it != zipper.end();) {
		auto[a, b, c, d] = *it;
		if(a % 2 == 0) {
			it = zipper.erase(it);
		}
		else {
			it++;
		}
	}
	if(zipper.size() != 50)
		throw std::logic_error("erase_non_const " + failed + ": zipper.size() != 50");

	zipper.erase(zipper.cbegin() + 10, zipper.cend());
	if(zipper.size() != 10)
		throw std::logic_error("erase_non_const " + failed + ": zipper.size() != 10");
}

void join() {
    auto join = [](auto&&... containers) {
        auto zipped_containers = zip(std::forward<decltype(containers)>(containers)...);

        using zipped_type = decltype(zipped_containers);
        using zipped_value_type = typename zipped_type::value_type;
        std::vector<zipped_value_type> joined_containers;
        joined_containers.reserve(zipped_containers.size());
        for (auto zipped : zipped_containers)
            joined_containers.push_back(zipped);
        return joined_containers; 
    };
	std::vector<int> first = []() {
		std::vector<int> first(50);
		std::generate(first.begin(), first.end(), [i = 0]() mutable { return i++; });
		return first;
	}();
	std::vector<int> second = []() {
		std::vector<int> second(50);
		std::generate(second.begin(), second.end(), [i = 0, j = 1]() mutable { return (++i) * (++j); });
		return second;
	}();
	std::vector<int> third = [&]() {
		std::vector<int> third(50);
		std::generate(third.begin(), third.end(), [&, i = 0, j = 0]() mutable { return second[i++] / (first[j++] + 1); });
		return third;
	}();
	auto joined = join(first, second, third);
	for (auto [a, b, c, abc] : zip(first, second, third, joined)) {
		if (std::make_tuple(a, b, c) != abc)
			throw std::logic_error("join " + failed + ": std::make_tuple(a, b, c) != abc");
	}
}

#endif