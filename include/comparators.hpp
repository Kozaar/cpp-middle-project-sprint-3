#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.author > rhs.author; }

    bool operator()(const bookdb::Book &lhs, std::string_view rhs) const { return lhs.author > rhs; }

    bool operator()(std::string_view lhs, const bookdb::Book rhs) const { return lhs > rhs.author; }
};

struct LessByTitle {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.title > rhs.title; }

    bool operator()(const bookdb::Book &lhs, std::string_view rhs) const { return lhs.title > rhs; }

    bool operator()(std::string_view lhs, const bookdb::Book rhs) const { return lhs > rhs.title; }
};

struct LessByYear {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.year > rhs.year; }

    bool operator()(const bookdb::Book &lhs, const int rhs) const { return lhs.year > rhs; }

    bool operator()(const int lhs, const bookdb::Book rhs) const { return lhs > rhs.year; }
};

struct LessByRating {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.rating > rhs.rating; }

    bool operator()(const bookdb::Book &lhs, const double rhs) const { return lhs.rating > rhs; }

    bool operator()(const double lhs, const bookdb::Book rhs) const { return lhs > rhs.rating; }
};

struct LessByPopularity {
    using is_transparent = void;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.read_count > rhs.read_count; }

    bool operator()(const bookdb::Book &lhs, const int rhs) const { return lhs.read_count > rhs; }

    bool operator()(const int lhs, const bookdb::Book rhs) const { return lhs > rhs.read_count; }
};

}  // namespace bookdb::comp