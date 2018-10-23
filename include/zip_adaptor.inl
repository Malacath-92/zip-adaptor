#include "zip_adaptor.h"

#include <utility>

// Note: 
#ifdef _MSVC_LANG
#define dependent_template
#else
#define dependent_template template
#endif

namespace ZIP_NAMESPACE {
	namespace detail {
		template<class... T, std::size_t... Indices>
		inline constexpr bool not_equal_impl(const std::tuple<T...>& lhs, const std::tuple<T...>& rhs,
											 const std::integer_sequence<std::size_t, Indices...>&) {
			return ((std::get<Indices>(lhs) != std::get<Indices>(rhs)) && ...);
		}
	}

	/// Adaptor implementation
#ifndef ZIP_VERBOSE
	template<class... T>
	inline constexpr zip_adaptor<T...> zip(T&&... iteratables) {
		return zip_adaptor<T...>(std::forward<T>(iteratables)...);
	}
#else
	template<class... T>
	inline constexpr zip_adaptor<T...> make_zip_adaptor(T&&... iteratables) {
		return zip_adaptor<T...>(std::forward<T>(iteratables)...);
	}
#endif

	template<class... T>
	inline constexpr zip_adaptor<T...>::zip_adaptor(T&&... iteratables) : mIterables(std::forward_as_tuple(std::forward<T>(iteratables)...)) {
		const size_t firstSize = std::get<0>(mIterables).size();
		bool sameSizes = ((iteratables.size() == firstSize) && ...);
		if(!sameSizes)
			throw std::length_error("Zipping containers of different length!");
	}

	template<class... T>
	inline constexpr std::size_t zip_adaptor<T...>::size() const noexcept {
		return std::get<0>(mIterables).size();
	}
	template<class... T>
	inline constexpr bool zip_adaptor<T...>::empty() const noexcept {
		return size() != 0;
	}

	template<class... T>
	inline constexpr typename zip_adaptor<T...>::iterator zip_adaptor<T...>::begin() noexcept {
		auto begin_impl = [](auto&... iteratables) {
			return iterator(iteratables.begin()...);
		};
		return std::apply(begin_impl, mIterables);
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_iterator zip_adaptor<T...>::begin() const noexcept {
		auto begin_impl = [](auto&... iteratables) {
			return const_iterator(iteratables.begin()...);
		};
		return std::apply(begin_impl, mIterables);
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::iterator zip_adaptor<T...>::end() noexcept {
		auto end_impl = [](auto&... iteratables) {
			return iterator(iteratables.end()...);
		};
		return std::apply(end_impl, mIterables);
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_iterator zip_adaptor<T...>::end() const noexcept {
		auto end_impl = [](auto&... iteratables) {
			return const_iterator(iteratables.end()...);
		};
		return std::apply(end_impl, mIterables);
	}

	template<class... T>
	inline constexpr typename zip_adaptor<T...>::reverse_iterator zip_adaptor<T...>::rbegin() noexcept {
		std::make_reverse_iterator(begin());
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_reverse_iterator zip_adaptor<T...>::rbegin() const noexcept {
		std::make_reverse_iterator(begin());
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::reverse_iterator zip_adaptor<T...>::rend() noexcept {
		std::make_reverse_iterator(end());
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_reverse_iterator zip_adaptor<T...>::rend() const noexcept {
		std::make_reverse_iterator(end());
	}

	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_iterator zip_adaptor<T...>::cbegin() const noexcept {
		return begin();
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_iterator zip_adaptor<T...>::cend() const noexcept {
		return end();
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_reverse_iterator zip_adaptor<T...>::crbegin() const noexcept {
		return rbegin();
	}
	template<class... T>
	inline constexpr typename zip_adaptor<T...>::const_reverse_iterator zip_adaptor<T...>::crend() const noexcept {
		return rend();
	}


	/// Iterator implementation
	template<class... T>
	template<class... IterT>
	inline constexpr zip_adaptor<T...>::zip_iterator<IterT...>::zip_iterator(IterT... iters) :
		mIters(std::forward_as_tuple(iters...)) {}

	template<class... T>
	template<class... IterT>
	inline constexpr typename zip_adaptor<T...>::dependent_template zip_iterator<IterT...>& zip_adaptor<T...>::zip_iterator<IterT...>::operator++() {
		increment();
		return *this;
	}
	template<class... T>
	template<class... IterT>
	inline constexpr typename zip_adaptor<T...>::dependent_template zip_iterator<IterT...> zip_adaptor<T...>::zip_iterator<IterT...>::operator++(int) {
		auto a = *this;
		increment();
		return a;
	}
	template<class... T>
	template<class... IterT>
	inline constexpr typename zip_adaptor<T...>::dependent_template zip_iterator<IterT...>& zip_adaptor<T...>::zip_iterator<IterT...>::operator--() {
		decrement();
		return *this;
	}
	template<class... T>
	template<class... IterT>
	inline constexpr typename zip_adaptor<T...>::dependent_template zip_iterator<IterT...> zip_adaptor<T...>::zip_iterator<IterT...>::operator--(int) {
		auto a = *this;
		decrement();
		return a;
	}

	template<class... T>
	template<class... IterT>
	inline constexpr bool zip_adaptor<T...>::zip_iterator<IterT...>::operator!=(const zip_iterator& rhs) const {
		using IterTuple = decltype(mIters);
		auto not_equal = [](const IterTuple& lhs, const IterTuple& rhs) {
			constexpr std::size_t tuple_size = std::tuple_size_v<std::remove_reference_t<decltype(lhs)>>;
			static_assert(tuple_size == std::tuple_size_v<std::remove_reference_t<decltype(rhs)>>, "Tuples need to be of same size");
			constexpr auto indices = std::make_index_sequence<tuple_size>();
			return detail::not_equal_impl(lhs, rhs, indices);
		};
		return not_equal(mIters, rhs.mIters);
	}

	template<class... T>
	template<class... IterT>
	inline constexpr typename zip_adaptor<T...>::dependent_template zip_iterator<IterT...>::value_type zip_adaptor<T...>::zip_iterator<IterT...>::operator*() {
		auto dereference_impl = [](IterT... iters) {
			return std::forward_as_tuple(*iters...);
		};
		return std::apply(dereference_impl, mIters);
	}

	template<class... T>
	template<class... IterT>
	inline constexpr void zip_adaptor<T...>::zip_iterator<IterT...>::increment() {
		auto increment_impl = [](auto&... iters) {
			(++iters, ...);
		};
		std::apply(increment_impl, mIters);
	}
	template<class... T>
	template<class... IterT>
	inline constexpr void zip_adaptor<T...>::zip_iterator<IterT...>::decrement() {
		auto decrement_impl = [](auto&... iters) {
			(--iters, ...);
		};
		std::apply(decrement_impl, mIters);
	}
}