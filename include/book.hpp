#pragma once

#include <format>
#include <iomanip>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    // Ваш код здесь
    return Genre::Unknown;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(const std::string_view title, const std::string &author, const int year, const Genre &genre,
                   const double rating, int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {}

    constexpr Book(const std::string_view title, const std::string &author, const int year,
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
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
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
