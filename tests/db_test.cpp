#include "book.hpp"
#include "book_database.hpp"
#include <gtest/gtest.h>

template <typename Container>
bool IsEqual(const Container &lhs, const Container &rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    auto itLhs = lhs.begin();
    auto itRhs = rhs.begin();

    for (; itLhs != lhs.end(); ++itLhs, ++itRhs) {
        if (*itLhs != *itRhs) {
            return false;
        }
    }
    return true;
}

TEST(TestBookDatabase, ConstructorCheck) {
    std::vector<bookdb::Book> etalon = {{"author", "title", 2020, bookdb::Genre::Biography, 4.5, 1},
                                        {"author", "title2", 2020, bookdb::Genre::Biography, 4.5, 2},
                                        {"author3", "title", 2020, bookdb::Genre::Biography, 4.5, 3}};
    bookdb::BookDatabase<>::AuthorContainer authorsEtalon = {{"author", 2}, {"author3", 1}};

    bookdb::BookDatabase<> db(std::initializer_list<bookdb::Book>{etalon[0], etalon[1], etalon[2]});

    ASSERT_TRUE(IsEqual(db.GetBooks(), etalon));
    ASSERT_TRUE(IsEqual(db.GetAuthors(), authorsEtalon));
}

TEST(TestBookDatabase, PushBackCheck) {
    std::vector<bookdb::Book> etalon = {{"author", "title", 2020, bookdb::Genre::Biography, 4.5, 1},
                                        {"author", "title2", 2020, bookdb::Genre::Biography, 4.5, 2},
                                        {"author3", "title", 2020, bookdb::Genre::Biography, 4.5, 3}};
    bookdb::BookDatabase<>::AuthorContainer authorsEtalon = {{"author", 2}, {"author3", 1}};
    bookdb::BookDatabase<> db;
    for (const auto &item : etalon) {
        db.PushBack(item);
    }

    ASSERT_TRUE(IsEqual(db.GetBooks(), etalon));
    ASSERT_TRUE(IsEqual(db.GetAuthors(), authorsEtalon));
}

TEST(TestBookDatabase, EmplaceBackCheck) {
    std::vector<bookdb::Book> etalon = {{"author", "title", 2020, bookdb::Genre::Biography, 4.5, 1},
                                        {"author", "title2", 2020, bookdb::Genre::Biography, 4.5, 2},
                                        {"author3", "title", 2020, bookdb::Genre::Biography, 4.5, 3}};
    bookdb::BookDatabase<>::AuthorContainer authorsEtalon = {{"author", 2}, {"author3", 1}};
    bookdb::BookDatabase<> db;
    for (const auto &item : etalon) {
        db.EmplaceBack(item.author, item.title, item.year, item.genre, item.rating, item.read_count);
    }

    ASSERT_TRUE(IsEqual(db.GetBooks(), etalon));
    ASSERT_TRUE(IsEqual(db.GetAuthors(), authorsEtalon));
}
