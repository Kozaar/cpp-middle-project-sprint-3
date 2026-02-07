#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.author < rhs.author; }

    bool operator()(const bookdb::Book &lhs, std::string_view rhs) const { return lhs.author < rhs; }

    bool operator()(std::string_view lhs, const bookdb::Book rhs) const { return lhs < rhs.author; }

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }
};

struct LessByTitle {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.title < rhs.title; }

    bool operator()(const bookdb::Book &lhs, std::string_view rhs) const { return lhs.title < rhs; }

    bool operator()(std::string_view lhs, const bookdb::Book rhs) const { return lhs < rhs.title; }

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }
};

struct LessByYear {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.year < rhs.year; }

    bool operator()(const bookdb::Book &lhs, const int rhs) const { return lhs.year < rhs; }

    bool operator()(const int lhs, const bookdb::Book rhs) const { return lhs < rhs.year; }

    bool operator()(const int lhs, const int rhs) const { return lhs < rhs; }
};

struct LessByPopularity {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.rating < rhs.rating; }

    bool operator()(const bookdb::Book &lhs, const double rhs) const { return lhs.rating < rhs; }

    bool operator()(const double lhs, const bookdb::Book rhs) const { return lhs < rhs.rating; }

    bool operator()(const double lhs, const double rhs) const { return lhs < rhs; }
};

using LessByRating = LessByPopularity;

}  // namespace bookdb::comp