#pragma once

#include <array>
#include <format>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// clang-format off
constexpr std::array genreString = {
    std::pair{Genre::Fiction, "Fiction"},
    std::pair{Genre::NonFiction, "NonFiction"},
    std::pair{Genre::SciFi, "SciFi"},
    std::pair{Genre::Biography, "Biography"},
    std::pair{Genre::Mystery, "Mystery"},
    std::pair{Genre::Unknown, "Unknown"}
};
// clang-format on

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {

    auto it = std::find_if(genreString.begin(), genreString.end(), [&s](const auto &p) { return p.second == s; });

    if (it != genreString.end()) {
        return it->first;
    }

    std::string msg = "Unsupported genre ";
    msg += s;
    throw std::logic_error(msg);
}

constexpr std::string GenreToString(const Genre genre) {
    auto it =
        std::find_if(genreString.begin(), genreString.end(), [&genre](const auto &p) { return p.first == genre; });

    if (it != genreString.end()) {
        return it->second;
    }

    std::string msg = "Unsupported genre ";
    msg += std::to_string(static_cast<int>(genre));
    throw std::logic_error(msg);
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(const std::string &title, const std::string_view &author, const int year, const Genre &genre,
                   const double rating, int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {}

    constexpr Book(const std::string &title, const std::string_view &author, const int year,
                   const std::string_view &genre, const double rating, int read_count)
        : author(author), title(title), year(year), genre(Genre::Unknown), rating(rating), read_count(read_count) {
        this->genre = GenreFromString(genre);
    }

    bool operator==(const Book &other) const {
        return (author == other.author) && (title == other.title) && (year == other.year) && (genre == other.genre) &&
               (rating == other.rating) && (read_count == other.read_count);
    }
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str = GenreToString(g);
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        return format_to(fc.out(), "\"{}\" {}, {} ({}, rating {}), reads {}", b.title, b.author, b.year, b.genre,
                         b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
