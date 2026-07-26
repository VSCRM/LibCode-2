#include "MainFeatures.h"
#include "ResultAdapter.h"
#include "Logger.h"
#include "PostgresConnect.h"
#include <stdexcept>
#include <utility>
#include <cctype>
#include <algorithm>

#pragma region Private Methods
void MainFeatures::logSqlError(const std::string& methodName, const pqxx::sql_error& e) {
    Logger::getInstance().log("[MainFeatures] [ERROR] SQL error (" + methodName + ")");
    Logger::getInstance().log("what(): " + std::string(e.what()));
    Logger::getInstance().log("query(): " + std::string(e.query()));
}

std::optional<std::string> MainFeatures::nullIfEmpty(const std::string& value) {
    if (value.empty()) return std::nullopt;
    return value;
}

std::optional<int> MainFeatures::parseOptionalInt(const std::string& value, const std::string& fieldName) {
    if (value.empty()) return std::nullopt;

    bool isNumeric = !value.empty() &&
        std::all_of(value.begin(), value.end(), [](unsigned char c) { return std::isdigit(c); });
    if (!isNumeric) {
        throw std::invalid_argument(fieldName + " must be a whole number, got: \"" + value + "\"");
    }

    try {
        return std::stoi(value);
    }
    catch (const std::exception&) {
        throw std::invalid_argument(fieldName + " is not a valid number: \"" + value + "\"");
    }
}
#pragma endregion

#pragma region Constructor
MainFeatures::MainFeatures(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
MainFeatures::~MainFeatures() {
    Logger::getInstance().log("MainFeatures destructor: releasing resources.");
}
#pragma endregion

#pragma region Methods
void MainFeatures::showInfoOfBooks() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in showInfoOfBooks");

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

        PostgresOutput output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfBooks", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] showInfoOfBooks: " + std::string(e.what()));
    }
}

void MainFeatures::showInfoOfReaders() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in showInfoOfReaders");

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

        PostgresOutput output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfReaders", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] showInfoOfReaders: " + std::string(e.what()));
    }
}

void MainFeatures::showInfoOfVisits() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in showInfoOfVisits");

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

        PostgresOutput output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfVisits", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] showInfoOfVisits: " + std::string(e.what()));
    }
}

void MainFeatures::showMostPopularBooks() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in showMostPopularBooks");

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

        PostgresOutput output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showMostPopularBooks", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] showMostPopularBooks: " + std::string(e.what()));
    }
}

// Adds a new book, creating the author if they don't already exist, and
// links the two through book_authors. genre/publisher/isbn are optional
// (stored as NULL); year is optional free text, parsed into an integer and
// rejected via an exception if it isn't a whole number.
void MainFeatures::addBook(const std::string& title, const std::string& author,
    const std::string& genre, const std::string& publisher,
    const std::string& year, const std::string& isbn) {
    try {
        if (title.empty() || author.empty()) {
            throw std::invalid_argument("Title and author must not be empty");
        }
        std::optional<int> parsedYear = parseOptionalInt(year, "Year");

        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in addBook");

        pqxx::work txn(*pgConn->getConnection());

        pqxx::result existingAuthor = txn.exec_params("SELECT id FROM authors WHERE name = $1", author);
        int authorId;
        if (!existingAuthor.empty()) {
            authorId = existingAuthor[0][0].as<int>();
        }
        else {
            pqxx::result inserted = txn.exec_params(
                "INSERT INTO authors (name) VALUES ($1) RETURNING id", author);
            authorId = inserted[0][0].as<int>();
        }

        pqxx::result insertedBook = txn.exec_params(
            "INSERT INTO books (title) VALUES ($1) RETURNING id", title);
        int bookId = insertedBook[0][0].as<int>();

        txn.exec_params(
            "INSERT INTO book_authors (book_id, author_id) VALUES ($1, $2)", bookId, authorId);

        txn.exec_params(
            "INSERT INTO book_details (book_id, genre, publisher, year, isbn) VALUES ($1, $2, $3, $4, $5)",
            bookId, nullIfEmpty(genre), nullIfEmpty(publisher), parsedYear, nullIfEmpty(isbn));

        txn.commit();
        Logger::getInstance().log("[MainFeatures] New book added: " + title);
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("addBook", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] addBook: " + std::string(e.what()));
    }
}

// Adds a new reader along with a category_details row. Only name and
// categoryType are required; address/phone and all the category-specific
// fields are optional and stored as NULL when left blank. schoolNumber is
// free text, parsed into an integer and rejected via an exception if it
// isn't a whole number.
void MainFeatures::addReader(const std::string& name, const std::string& address,
    const std::string& phone, const std::string& categoryType,
    const std::string& institutionName, const std::string& major,
    const std::string& schoolNumber, const std::string& kindergartenName,
    const std::string& researchField) {
    try {
        if (name.empty() || categoryType.empty()) {
            throw std::invalid_argument("Name and category must not be empty");
        }
        std::optional<int> parsedSchoolNumber = parseOptionalInt(schoolNumber, "School number");

        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in addReader");

        pqxx::work txn(*pgConn->getConnection());

        pqxx::result existingCategory = txn.exec_params(
            "SELECT id FROM categories WHERE category_type = $1", categoryType);
        int categoryId;
        if (!existingCategory.empty()) {
            categoryId = existingCategory[0][0].as<int>();
        }
        else {
            pqxx::result inserted = txn.exec_params(
                "INSERT INTO categories (category_type) VALUES ($1) RETURNING id", categoryType);
            categoryId = inserted[0][0].as<int>();
        }

        pqxx::result insertedReader = txn.exec_params(
            "INSERT INTO readers (name, address, phone) VALUES ($1, $2, $3) RETURNING id",
            name, nullIfEmpty(address), nullIfEmpty(phone));
        int readerId = insertedReader[0][0].as<int>();

        txn.exec_params(
            "INSERT INTO category_details "
            "(category_id, reader_id, institution_name, major, school_number, kindergarten_name, research_field) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)",
            categoryId, readerId, nullIfEmpty(institutionName), nullIfEmpty(major),
            parsedSchoolNumber, nullIfEmpty(kindergartenName), nullIfEmpty(researchField));

        txn.commit();
        Logger::getInstance().log("[MainFeatures] New reader added: " + name);
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("addReader", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] addReader: " + std::string(e.what()));
    }
}

// Records a new visit for an existing reader and library. readerName and
// libraryName must match an existing row (looked up by name); if either is
// not found, the visit is rejected via an exception instead of being
// silently recorded with a bogus reference. bookTitle is optional: an
// empty title records a visit without a borrowed book, but a non-empty
// title that doesn't match any book is treated as an error rather than
// silently ignored.
void MainFeatures::addVisit(const std::string& readerName, const std::string& libraryName,
    const std::string& bookTitle) {
    try {
        if (readerName.empty() || libraryName.empty()) {
            throw std::invalid_argument("Reader name and library name must not be empty");
        }

        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in addVisit");

        pqxx::work txn(*pgConn->getConnection());

        pqxx::result readerRes = txn.exec_params("SELECT id FROM readers WHERE name = $1", readerName);
        if (readerRes.empty()) {
            throw std::invalid_argument("No reader found with name: " + readerName);
        }
        int readerId = readerRes[0][0].as<int>();

        pqxx::result libraryRes = txn.exec_params("SELECT id FROM libraries WHERE name = $1", libraryName);
        if (libraryRes.empty()) {
            throw std::invalid_argument("No library found with name: " + libraryName);
        }
        int libraryId = libraryRes[0][0].as<int>();

        std::optional<int> bookId = std::nullopt;
        if (!bookTitle.empty()) {
            pqxx::result bookRes = txn.exec_params("SELECT id FROM books WHERE title = $1", bookTitle);
            if (bookRes.empty()) {
                throw std::invalid_argument("No book found with title: " + bookTitle);
            }
            bookId = bookRes[0][0].as<int>();
        }

        txn.exec_params(
            "INSERT INTO visits (reader_id, library_id, book_id, visit_time) "
            "VALUES ($1, $2, $3, date_trunc('second', NOW()))",
            readerId, libraryId, bookId);

        txn.commit();
        Logger::getInstance().log("[MainFeatures] New visit recorded for reader: " + readerName);
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("addVisit", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] addVisit: " + std::string(e.what()));
    }
}

std::vector<std::string> MainFeatures::getReaderNames() {
    std::vector<std::string> names;
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in getReaderNames");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec("SELECT name FROM readers ORDER BY name");
        for (const auto& row : res) {
            names.push_back(row[0].as<std::string>());
        }
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("getReaderNames", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] getReaderNames: " + std::string(e.what()));
    }
    return names;
}

std::vector<std::string> MainFeatures::getLibraryNames() {
    std::vector<std::string> names;
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in getLibraryNames");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec("SELECT name FROM libraries ORDER BY name");
        for (const auto& row : res) {
            names.push_back(row[0].as<std::string>());
        }
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("getLibraryNames", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] getLibraryNames: " + std::string(e.what()));
    }
    return names;
}

std::vector<std::string> MainFeatures::getBookTitles() {
    std::vector<std::string> titles;
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in getBookTitles");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec("SELECT title FROM books ORDER BY title");
        for (const auto& row : res) {
            titles.push_back(row[0].as<std::string>());
        }
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("getBookTitles", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[MainFeatures] [ERROR] getBookTitles: " + std::string(e.what()));
    }
    return titles;
}
#pragma endregion
