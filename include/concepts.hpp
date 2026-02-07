#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires {
    // { typename T::value_type{} } -> std::convertible_to<Book>;
    // requires std::
    typename T::value_type;
    // requires std::is_same_v<decltype(T::value_type::author), std::string_view>;
    // requires std::is_same_v<decltype(T::value_type::title), std::string_view>;
    // requires std::is_same_v<decltype(T::value_type::year), int>;
    // requires std::is_same_v<decltype(T::value_type::genre), Genre>;
    // requires std::is_same_v<decltype(T::value_type::rating), double>;
    // requires std::is_same_v<decltype(T::value_type::read_count), int>;
    { T::value_type::author } -> std::convertible_to<std::string_view>;
    { T::value_type::title } -> std::convertible_to<std::string_view>;
    { T::value_type::year } -> std::convertible_to<int>;
    { T::value_type::genre } -> std::convertible_to<Genre>;
    { T::value_type::rating } -> std::convertible_to<double>;
    { T::value_type::read_count } -> std::convertible_to<int>;
};

template <typename T>
concept BookIterator = std::bidirectional_iterator<T> &&
                       (std::same_as<std::iter_value_t<T>, Book> || std::same_as<std::iter_reference_t<T>, Book &>);

template <typename S, typename I>
concept BookSentinel = requires(const I &iter, const S &sentinel) {
    requires BookIterator<I>;
    requires std::semiregular<S>;
    { iter == sentinel } -> std::convertible_to<bool>;
    { sentinel == iter } -> std::convertible_to<bool>;
};

template <typename P>
concept BookPredicate = requires(P predicate, const Book &book) {
    { predicate(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(const Book &lhs, const Book &rhs, C c) {
    { c(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb