#pragma once

#include <format>
#include <iomanip>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    // clang-format off
    if (s == "Fiction") return Genre::Fiction;
    if (s == "NonFiction") return Genre::NonFiction;
    if (s == "SciFi") return Genre::SciFi;
    if (s == "Biography") return Genre::Biography;
    if (s == "Mystery") return Genre::Mystery;
    if (s == "Unknown") return Genre::Unknown;
    // clang-format on

    std::string msg = "Unsupported genre ";
    msg += s;
    throw std::logic_error(msg);
}

constexpr std::string GenreToString(const Genre genre) {
    // clang-format off
    std::string genreStr;
    using bookdb::Genre;
    switch (genre) {
        case Genre::Fiction:    genreStr = "Fiction"; break;
        case Genre::Mystery:    genreStr = "Mystery"; break;
        case Genre::NonFiction: genreStr = "NonFiction"; break;
        case Genre::SciFi:      genreStr = "SciFi"; break;
        case Genre::Biography:  genreStr = "Biography"; break;
        case Genre::Unknown:    genreStr = "Unknown"; break;
        default:
            throw std::logic_error{"Unsupported bookdb::Genre"};
        }
    // clang-format on
    return genreStr;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(const std::string &t, const std::string_view &a, const int y, const Genre &g, const double r,
                   int count)
        : author(a), title(t), year(y), genre(g), rating(r), read_count(count) {}

    constexpr Book(const std::string &t, const std::string_view &a, const int y, const std::string_view &g,
                   const double r, int count)
        : author(a), title(t), year(y), genre(Genre::Unknown), rating(r), read_count(count) {
        genre = GenreFromString(g);
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
