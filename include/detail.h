#pragma once

#include <tuple>
#include <utility>

namespace detail {
	template<class... T>
	inline constexpr bool not_equal(const std::tuple<T...>& lhs, const std::tuple<T...>& rhs);


	template<class... T, std::size_t... Indices>
	inline constexpr bool not_equal_impl(const std::tuple<T...>& lhs, const std::tuple<T...>& rhs,
										 const std::integer_sequence<std::size_t, Indices...>&) {
		return ((std::get<Indices>(lhs) != std::get<Indices>(rhs)) && ...);
	}
	template<class... T>
	inline constexpr bool not_equal(const std::tuple<T...>& lhs, const std::tuple<T...>& rhs) {
		constexpr std::size_t tuple_size = std::tuple_size_v<std::remove_reference_t<decltype(lhs)>>;
		static_assert(tuple_size == std::tuple_size_v<std::remove_reference_t<decltype(rhs)>>, "Tuples need to be of same size");
		constexpr auto indices = std::make_index_sequence<tuple_size>();
		return not_equal_impl(lhs, rhs, indices);
	}
}