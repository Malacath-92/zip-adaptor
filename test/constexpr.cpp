#include <array>

#include "zip_adaptor.h"

#ifndef SKIP_CONSTEXPR_TESTS

constexpr bool constexpr_const_iteration() {
	const std::array<int, 5> first	= { 0, 1, 2, 3, 4 };
	const std::array<int, 5> second = { 3, 6, 9, 12, 15 };
	const std::array<int, 5> third	= { 0, 6, 18, 36, 60 };
	// Note: Circumventing an issue in MSVC that causes the range-based-for-loop to not be constexpr
#ifdef _MSVC_LANG
	auto zipper = zip(first, second, third);
	auto begin = zipper.begin();
	auto end = zipper.end();
	for(; begin != end; begin++) {
		auto val = *begin;
		auto&[a, b, c] = val;
#else
	for(auto[a, b, c] : zip(first, second, third)) {
#endif
		static_assert(std::is_same_v<decltype(a), const int&>, "Iterating through wrong type, should be const int&");
		static_assert(std::is_same_v<decltype(b), const int&>, "Iterating through wrong type, should be const int&");
		static_assert(std::is_same_v<decltype(c), const int&>, "Iterating through wrong type, should be const int&");
		if(b != (a + 1) * 3)
			return false;
		if(c != a * b)
			return false;
	}
	return true;
}
static_assert(constexpr_const_iteration(), "Test constexpr_const_iteration failed");

constexpr bool constexpr_non_const_iteration() {
	auto swap = [](auto& lhs, auto& rhs) {
		auto tmp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(tmp);
	};

	const std::array<int, 5> first = { 0, 1, 2, 3, 4 };
	std::array<int, 5> second = { 3, 6, 9, 12, 15 };
	std::array<int, 5> third = { 0, 6, 18, 36, 60 };
	// Note: Circumventing an issue in MSVC that causes the range-based-for-loop to not be constexpr
#ifdef _MSVC_LANG
	auto zipper = zip(first, second, third);
	auto begin = zipper.begin();
	auto end = zipper.end();
	for(; begin != end; begin++) {
		auto val = *begin;
		auto&[a, b, c] = val;
#else
	for(auto[a, b, c] : zip(first, second, third)) {
#endif
		static_assert(std::is_same_v<decltype(a), const int&>, "Iterating through wrong type, should be const int&");
		static_assert(std::is_same_v<decltype(b), int&>, "Iterating through wrong type, should be int&");
		static_assert(std::is_same_v<decltype(c), int&>, "Iterating through wrong type, should be int&");
		if(b != (a + 1) * 3)
			return false;
		if(c != a * b)
			return false;
		swap(b, c);
		b *= 2;
		c /= 3;
		swap(b, c);
		if(b != a + 1)
			return false;
		if(c != a * b * 3 * 2)
			return false;
	}
	return true;
}
static_assert(constexpr_non_const_iteration(), "Test constexpr_non_const_iteration failed");

constexpr bool constexpr_no_copies_no_moves() {
	auto swap = [](auto& lhs, auto& rhs) {
		auto tmp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(tmp);
	};

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
	// Note: Circumventing an issue in MSVC that causes the range-based-for-loop to not be constexpr
#ifdef _MSVC_LANG
	auto zipper = zip(first, second, third);
	auto begin = zipper.begin();
	auto end = zipper.end();
	for(; begin != end; begin++) {
		auto val = *begin;
		auto&[a, b, c] = val;
#else
	for(auto[a, b, c] : zip(first, second, third)) {
#endif
		static_assert(std::is_same_v<decltype(a), const ExpensiveToCopyOrMove&>, "Iterating through wrong type, should be const ExpensiveToCopyOrMove&");
		static_assert(std::is_same_v<decltype(b), const ExpensiveToCopyOrMove&>, "Iterating through wrong type, should be const ExpensiveToCopyOrMove&");
		static_assert(std::is_same_v<decltype(c), const ExpensiveToCopyOrMove&>, "Iterating through wrong type, should be const ExpensiveToCopyOrMove&");
		if(b.v != (a.v + 1) * 3)
			return false;
		if(c.v != a.v * b.v)
			return false;
	}
	return true;
}
static_assert(constexpr_no_copies_no_moves(), "Test constexpr_non_const_iteration failed");

#endif