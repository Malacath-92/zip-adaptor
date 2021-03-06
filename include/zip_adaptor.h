#pragma once

#include <tuple>
#include <type_traits>

#ifndef ZIP_NAMESPACE
#define ZIP_NAMESPACE zip
#endif

namespace ZIP_NAMESPACE {
	template<class... T>
	class zip_adaptor;

#ifndef ZIP_VERBOSE
	template<class... T>
	constexpr auto zip(T&&... iteratables) {
		using result_zip_adaptor = zip_adaptor<decltype(std::forward<decltype(iteratables)>(iteratables))...>;
		return result_zip_adaptor(std::forward<decltype(iteratables)>(iteratables)...);
	}
#else
	template<class... T>
	constexpr auto make_zip_adaptor(T&&... iteratables) {
		using result_zip_adaptor = zip_adaptor<decltype(std::forward<decltype(iteratables)>(iteratables))...>;
		return result_zip_adaptor(std::forward<decltype(iteratables)>(iteratables)...);
	}
#endif

	template<class... T>
	class zip_adaptor {
	public:
		template<class... IterT>
		class zip_iterator {
			friend class zip_adaptor;
		public:
			using value_type = std::tuple<std::remove_reference_t<decltype(*std::declval<IterT>())>...>;
			using reference = std::tuple<decltype(*std::declval<IterT>())...>;

			constexpr zip_iterator(IterT... iters);

			zip_iterator() = delete;
			constexpr zip_iterator(const zip_iterator&) = default;
			constexpr zip_iterator(zip_iterator&&) = default;

			constexpr zip_iterator& operator=(const zip_iterator&) = default;
			constexpr zip_iterator& operator=(zip_iterator&&) = default;

			constexpr zip_iterator& operator+(int);
			constexpr zip_iterator& operator++();
			constexpr zip_iterator operator++(int);

			constexpr zip_iterator& operator-(int);
			constexpr zip_iterator& operator--();
			constexpr zip_iterator operator--(int);

			constexpr bool operator!=(const zip_iterator& rhs) const;

			constexpr reference operator*();

		private:
			constexpr void increment();
			constexpr void decrement();

			using iterator_tuple = std::tuple<IterT...>;
			iterator_tuple mIters;
		};

		using iterator = zip_iterator<decltype(std::declval<T>().begin())...>;
		using const_iterator = zip_iterator<decltype(std::declval<const T>().cbegin())...>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using value_type = typename iterator::value_type;
		using reference = typename iterator::reference;
		using const_reference = typename const_iterator::reference;

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

		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		template<class It = iterator, typename = std::enable_if_t<!std::is_same_v<It, const_iterator>>>
		const_iterator erase(const_iterator pos);
		template<class It = iterator, typename = std::enable_if_t<!std::is_same_v<It, const_iterator>>>
		const_iterator erase(const_iterator first, const_iterator last);

	private:
		template<class U, bool = std::is_rvalue_reference_v<U>>
		struct remove_rvalue_reference {
			using type = U;
		};
		template<class U>
		struct remove_rvalue_reference<U, true> {
			using type = std::remove_reference_t<U>;
		};
		template<class U>
		using remove_rvalue_reference_t = typename remove_rvalue_reference<U>::type;

		using iterable_tuple = std::tuple<remove_rvalue_reference_t<T>...>;
		iterable_tuple mIterables;
	};
}

#include "zip_adaptor.inl"