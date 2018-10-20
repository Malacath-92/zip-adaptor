#pragma once

#include <tuple>
#include <utility>

namespace detail {
	template<class... T, std::size_t... Indices>
	inline constexpr bool not_equal_impl(const std::tuple<T...>& lhs, const std::tuple<T...>& rhs,
										 const std::integer_sequence<std::size_t, Indices...>&) {
		return ((std::get<Indices>(lhs) != std::get<Indices>(rhs)) && ...);
	}
}