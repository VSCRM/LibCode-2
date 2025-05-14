#include "MainFeatures.h"
#include "ResultAdapter.h"
#include "Logger.h"
#include "PostgresConnect.h"

#pragma region Private Methods
void MainFeatures::logSqlError(const std::string& methodName, const pqxx::sql_error& e) {
    Logger::getInstance().log("[MainFeatures] [ERROR] SQL помилка (" + methodName + ")");
    Logger::getInstance().log("what(): " + std::string(e.what()));
    Logger::getInstance().log("query(): " + std::string(e.query()));
}
#pragma endregion

#pragma region Constructor
MainFeatures::MainFeatures(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
MainFeatures::~MainFeatures() {
    Logger::getInstance().log("Деструктор MainFeatures: очищення ресурсів.");
}
#pragma endregion

#pragma region Methods
void MainFeatures::showInfoOfBooks() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в showInfoOfBooks");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT
                books.id,
                books.title,
                authors.name AS author,
                book_details.genre,
                book_details.publisher,
                book_details.year,
                book_details.isbn
            FROM books
                JOIN book_details ON books.id = book_details.book_id
                JOIN book_authors ON books.id = book_authors.book_id
                JOIN authors ON book_authors.author_id = authors.id
            ORDER BY books.id
        )");

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfBooks", e);
    }
}

void MainFeatures::showInfoOfReaders() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в showInfoOfReaders");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT 
                readers.id,
                readers.name,
                readers.address,
                readers.phone,
                categories.category_type,
                category_details.institution_name,
                category_details.major,
                category_details.school_number,
                category_details.kindergarten_name,
                category_details.research_field
            FROM readers
                JOIN category_details ON readers.id = category_details.reader_id
                JOIN categories ON category_details.category_id = categories.id
            ORDER BY readers.id
        )");

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfReaders", e);
    }
}

void MainFeatures::showInfoOfVisits() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в showInfoOfVisits");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT 
                readers.name,
                books.title,
                visits.visit_time,
                libraries.name
            FROM visits
                JOIN readers ON visits.reader_id = readers.id
                JOIN libraries ON visits.library_id = libraries.id
                JOIN books ON visits.book_id = books.id
            ORDER BY visits.visit_time DESC
        )");

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfVisits", e);
    }
}

void MainFeatures::showMostPopularBooks() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в showMostPopularBooks");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT
                books.title,
                COUNT(*) AS given_count
            FROM visits
                JOIN books ON visits.book_id = books.id
            GROUP BY books.id, books.title
            ORDER BY given_count DESC
        )");

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showMostPopularBooks", e);
    }
}
#pragma endregion