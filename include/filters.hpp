#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"

namespace bookdb {

auto YearBetween(const int min, const int max) {
    return [min, max](const Book &book) { return (book.year >= min) && (book.year <= max); };
}

auto RatingAbove(const double value) {
    return [value](const Book &book) { return book.rating > value; };
}

auto GenreIs(const Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <class... Args>
auto all_of(Args... args) {
    return [args...](const Book &book) { return (args(book) && ...); };
}

template <class... Args>
auto any_of(Args... args) {
    return [args...](const Book &book) { return (args(book) || ...); };
}

template <BookIterator Iterator, BookPredicate Predicate>
std::vector<std::reference_wrapper<const Book>> filterBooks(Iterator begin, Iterator end, Predicate predicate) {
    std::vector<std::reference_wrapper<const Book>> books;

    std::for_each(begin, end, [&books, &predicate](const Book &book) {
        if (predicate(book)) {
            books.push_back(book);
        }
    });

    return books;
}

}  // namespace bookdb