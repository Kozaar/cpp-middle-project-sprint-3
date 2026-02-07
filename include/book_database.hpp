#pragma once

#include <algorithm>
#include <initializer_list>
#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases

    using value_type = BookContainer::value_type;
    // using reference = BookContainer::reference;
    // using const_reference = BookContainer::const_reference;
    // using pointer = BookContainer::pointer;
    // using const_pointer = BookContainer::const_pointer;
    using size_type = BookContainer::size_type;

    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;

    using reverse_iterator = BookContainer::reverse_iterator;
    using const_reverse_iterator = BookContainer::const_reverse_iterator;

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books) : books_(books) {
        std::for_each(books_.begin(), books_.end(), [this](auto &book) {
            auto [it, is_emplaced] = authors_.emplace(book.author);
            book.author = *it;
        });
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods

    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }

    const_iterator cbegin() const { return books_.cbegin(); }
    const_iterator cend() const { return books_.cend(); }

    reverse_iterator rbegin() { return books_.rbegin(); }
    reverse_iterator rend() { return books_.rend(); }

    const_reverse_iterator crbegin() { return books_.crbegin(); }
    const_reverse_iterator crend() { return books_.crend(); }

    size_type size() const { return books_.size(); }

    void PushBack(const Book &b) {
        books_.push_back(b);
        auto [it, is_emplaced] = authors_.emplace(books_.back().author);
        books_.back().author = *it;
    }

    template <typename... Args>
    void EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        auto [it, is_emplaced] = authors_.emplace(books_.back().author);
        books_.back().author = *it;
    }

    const BookContainer &GetBooks() const { return books_; }

    const AuthorContainer &GetAuthors() const { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        /*
        Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        (size/begin/...)
        */

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
