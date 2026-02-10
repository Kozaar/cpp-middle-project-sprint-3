#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "statsistics.hpp"

#include <gtest/gtest.h>
#include <print>

const std::vector<bookdb::Book> etalonBook = {{"title", "author", 2020, bookdb::Genre::Biography, 4.5, 1},
                                              {"title2", "author", 1990, bookdb::Genre::Mystery, 4.6, 2},
                                              {"title", "author3", 2025, bookdb::Genre::Biography, 4., 3}};

const bookdb::BookDatabase<>::AuthorContainer etalonAuthors = {{"author"}, {"author3"}};

template <typename Container>
bool IsEqual(const Container &lhs, const Container &rhs) {
    if (lhs.size() != rhs.size()) {
        std::println("Size of containers {}, expected {}", lhs.size(), rhs.size());
        return false;
    }

    auto itLhs = lhs.begin();
    auto itRhs = rhs.begin();

    for (; itLhs != lhs.end(); ++itLhs, ++itRhs) {
        if (*itLhs != *itRhs) {
            std::println("Value {}, expected {}", *itLhs, *itRhs);
            return false;
        }
    }
    return true;
}

bookdb::BookDatabase<> BuildBookDb() {
    bookdb::BookDatabase<> db;
    for (const auto &item : etalonBook) {
        db.PushBack(item);
    }

    return db;
}

TEST(TestBookDatabase, ConstructorCheck) {
    bookdb::BookDatabase<> db(std::initializer_list<bookdb::Book>{etalonBook[0], etalonBook[1], etalonBook[2]});

    ASSERT_TRUE(IsEqual(db.GetBooks(), etalonBook));
    ASSERT_TRUE(IsEqual(db.GetAuthors(), etalonAuthors));
}

TEST(TestBookDatabase, PushBackCheck) {
    bookdb::BookDatabase<> db;
    for (const auto &item : etalonBook) {
        db.PushBack(item);
    }

    ASSERT_TRUE(IsEqual(db.GetBooks(), etalonBook));
    ASSERT_TRUE(IsEqual(db.GetAuthors(), etalonAuthors));
}

TEST(TestBookDatabase, EmplaceBackCheck) {
    bookdb::BookDatabase<> db;
    for (const auto &item : etalonBook) {
        db.EmplaceBack(item.title, item.author, item.year, item.genre, item.rating, item.read_count);
    }

    ASSERT_TRUE(IsEqual(db.GetBooks(), etalonBook));
    ASSERT_TRUE(IsEqual(db.GetAuthors(), etalonAuthors));
}

TEST(TestStatistics, AuthorHistogramFlatCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto authors = bookdb::buildAuthorHistogramFlat(db);

    ASSERT_EQ(authors.size(), 2);

    auto it = authors.find("author");
    ASSERT_TRUE(it != authors.end());
    ASSERT_EQ(it->second, 2);

    it = authors.find("author2");
    ASSERT_TRUE(it == authors.end());

    it = authors.find("author3");
    ASSERT_TRUE(it != authors.end());
    ASSERT_EQ(it->second, 1);
}

TEST(TestStatistics, EmptyAuthorHistogramFlatCheck) {
    bookdb::BookDatabase<> db;

    auto authors = bookdb::buildAuthorHistogramFlat(db);

    ASSERT_EQ(authors.size(), 0);

    auto it = authors.find("author");
    ASSERT_TRUE(it == authors.end());
}

TEST(TestStatistics, GenreRatingsCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto ratings = bookdb::calculateGenreRatings(db.begin(), db.end());

    ASSERT_EQ(ratings.size(), 2);

    auto it = ratings.find(bookdb::Genre::Biography);
    ASSERT_TRUE(it != ratings.end());
    ASSERT_EQ(it->second, 4.25);

    it = ratings.find(bookdb::Genre::Mystery);
    ASSERT_TRUE(it != ratings.end());
    ASSERT_EQ(it->second, 4.6);
}

TEST(TestStatistics, EmptyGenreRatingsCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto ratings = bookdb::calculateGenreRatings(db.end(), db.end());

    ASSERT_EQ(ratings.size(), 0);

    auto it = ratings.find(bookdb::Genre::Biography);
    ASSERT_TRUE(it == ratings.end());

    it = ratings.find(bookdb::Genre::Mystery);
    ASSERT_TRUE(it == ratings.end());
}

TEST(TestStatistics, AverageRatingCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto rating = bookdb::calculateAverageRating(db);

    ASSERT_NEAR(rating, 4.367, 0.001);
}

TEST(TestStatistics, RandomBooksCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto books = bookdb::sampleRandomBooks(db, 2);

    ASSERT_EQ(books.size(), 2);

    std::for_each(books.begin(), books.end(), [&db](const bookdb::Book &book) {
        ASSERT_TRUE(std::find(db.cbegin(), db.cend(), book) != db.end());
    });
}

TEST(TestStatistics, RandomBooksFullDbCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto books = bookdb::sampleRandomBooks(db, 20);
    ASSERT_EQ(books.size(), db.size());

    std::for_each(books.begin(), books.end(), [&db](const bookdb::Book &book) {
        ASSERT_TRUE(std::find(db.cbegin(), db.cend(), book) != db.end());
    });
}

TEST(TestStatistics, GetTopNCheck) {
    bookdb::BookDatabase<> db = BuildBookDb();

    auto books = bookdb::getTopNBy(db, 1, bookdb::comp::LessByYear{});
    ASSERT_EQ(books.size(), 1);

    ASSERT_TRUE(books.front() == etalonBook.back());
}
