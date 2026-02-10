#pragma once

#include <algorithm>
#include <flat_map>
#include <functional>
#include <iterator>
#include <random>
#include <sstream>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, size_t, Comparator> authors(comp);

    std::for_each(cont.cbegin(), cont.cend(), [&authors](const Book &book) {
        auto it = authors.find(book.author);
        if (it != authors.end()) {
            ++it->second;
        } else {
            authors.emplace(book.author, 1);
        }
    });

    return authors;
}

template <BookIterator Iterator>
auto calculateGenreRatings(Iterator first, Iterator last) {
    struct Stats {
        double rating = 0.0;
        int count = 0;
    };
    std::flat_map<Genre, Stats> tmp;

    std::for_each(first, last, [&tmp](const Book &book) {
        auto [it, is_emplaced] = tmp.try_emplace(book.genre);
        it->second.rating += book.rating;
        ++it->second.count;
    });

    std::flat_map<Genre, double> ratings;
    std::for_each(tmp.begin(), tmp.end(), [&ratings](const auto &pair) {
        const auto &[genre, stats] = pair;
        if (stats.count > 0) {
            ratings.emplace(genre, stats.rating / stats.count);
        } else {
            ratings.emplace(genre, 0);
        }
    });

    return ratings;
}

template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.size() == 0) {
        return 0.;
    }

    double sum = std::transform_reduce(cont.cbegin(), cont.cend(), 0.0, std::plus<>(),
                                       [](const Book &book) { return book.rating; });
    return sum / cont.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t count) {
    std::vector<std::reference_wrapper<const Book>> books;

    if (count >= cont.size()) {
        books.reserve(cont.size());
        std::transform(cont.cbegin(), cont.cend(), std::back_inserter(books),
                       [](const Book &book) { return std::cref(book); });
    } else if (count > 0) {
        std::sample(cont.cbegin(), cont.cend(), std::back_inserter(books), count, std::mt19937{std::random_device{}()});
    }

    return books;
}

template <BookContainerLike T, BookComparator Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t count, Comparator comp) {
    std::vector<std::reference_wrapper<const Book>> books;

    if (count == 0) {
        return books;
    }

    if (count > cont.size()) {
        count = cont.size();
    }

    books.reserve(count);

    std::partial_sort(cont.begin(), cont.begin() + count, cont.end(), comp);

    books.assign(cont.begin(), cont.begin() + count);

    return books;
}

}  // namespace bookdb

namespace std {

template <>
struct formatter<std::flat_map<std::string, size_t, bookdb::TransparentStringLess>> {
    template <typename FormatContext>
    auto format(const std::flat_map<std::string, size_t, bookdb::TransparentStringLess> &authors,
                FormatContext &fc) const {
        std::stringstream ss;
        const int left_shift = 20;
        const int right_shift = 5;
        ss << "\n";
        ss << std::setw(left_shift) << "Author" << " | books\n";
        std::for_each(authors.begin(), authors.end(), [&ss](const auto &pair) {
            ss << std::setw(left_shift) << pair.first << " - " << std::setw(right_shift) << pair.second << "\n";
        });
        return format_to(fc.out(), "{}", ss.str());
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<std::flat_map<bookdb::Genre, double>> {
    template <typename FormatContext>
    auto format(const std::flat_map<bookdb::Genre, double> &raitings, FormatContext &fc) const {
        std::stringstream ss;
        const int left_shift = 20;
        const int right_shift = 5;
        ss << "\n";
        ss << std::setw(left_shift) << "Genre" << " | Rating\n";
        std::for_each(raitings.begin(), raitings.end(), [&ss](const auto &pair) {
            ss << std::setw(left_shift) << std::format("{}", pair.first) << " - " << std::setw(right_shift)
               << pair.second << "\n";
        });
        return format_to(fc.out(), "{}", ss.str());
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
