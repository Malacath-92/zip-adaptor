#pragma once

#include <tuple>

#include "detail.h"

template<class... T>
class zip_adaptor;
template<class... T>
class zip_iterator;

template<class... T>
constexpr zip_adaptor<T...> make_zip_adaptor(T&& ...iteratables);

template<class... T>
class zip_adaptor {
public:
	template<class... IterT>
	class zip_iterator {
	public:
		using value_type = std::tuple<decltype(*std::declval<IterT>())...>;
		using const_value_type = std::tuple<decltype(*std::declval<const IterT>())...>;

		constexpr zip_iterator(IterT... iters);

		zip_iterator() = delete;
		constexpr zip_iterator(const zip_iterator&) = default;
		constexpr zip_iterator(zip_iterator&&) = default;

		constexpr zip_iterator& operator=(const zip_iterator&) = default;
		constexpr zip_iterator& operator=(zip_iterator&&) = default;

		constexpr zip_iterator& operator++();
		constexpr zip_iterator operator++(int);

		constexpr zip_iterator& operator--();
		constexpr zip_iterator operator--(int);

		constexpr bool operator!=(const zip_iterator& rhs) const;

		constexpr value_type operator*();
		constexpr const_value_type operator*() const;

	private:
		constexpr void increment();
		constexpr void decrement();

		using iterator_tuple = std::tuple<IterT...>;
		iterator_tuple mIters;
	};
	using iterator = zip_iterator<decltype(std::declval<T>().begin())...>;
	using const_iterator = zip_iterator<decltype(std::declval<const T>().begin())...>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// Throws std::length_error if sizes of iterables don't match
	constexpr zip_adaptor(T&&... iteratables);

	zip_adaptor() = delete;
	constexpr zip_adaptor(const zip_adaptor&) = default;
	constexpr zip_adaptor(zip_adaptor&&) = default;

	constexpr zip_adaptor& operator=(const zip_adaptor&) = default;
	constexpr zip_adaptor& operator=(zip_adaptor&&) = default;

	~zip_adaptor() = default;

	constexpr iterator begin() noexcept;
	constexpr const_iterator begin() const noexcept;
	constexpr iterator end() noexcept;
	constexpr const_iterator end() const noexcept;

	constexpr reverse_iterator rbegin() noexcept;
	constexpr const_reverse_iterator rbegin() const noexcept;
	constexpr reverse_iterator rend() noexcept;
	constexpr const_reverse_iterator rend() const noexcept;

	constexpr const_iterator cbegin() const noexcept;
	constexpr const_iterator cend() const noexcept;
	constexpr const_reverse_iterator crbegin() const noexcept;
	constexpr const_reverse_iterator crend() const noexcept;

	constexpr std::size_t size() const noexcept;
	constexpr bool empty() const noexcept;

private:
	std::tuple<T&...> mIterables;
};

#include "zip_adaptor.inl"
