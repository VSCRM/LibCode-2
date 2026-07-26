#pragma once

#include "IBaseOperations.h"
#include "ResultAdapter.h"
#include "IDBConnect.h"
#include <memory> 
#include <optional>
#include <vector>

/**
 * \class MainFeatures
 * \brief Core operations available to librarians and admins: viewing
 *        statistics, and adding books, readers, and visits.
 */
class MainFeatures : public IBaseOperations {
private:
#pragma region Fields
    /**
     * \brief Database connection.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

#pragma region Private Methods
    /**
     * \brief Logs an SQL query error.
     * \param methodName Name of the method where the error occurred.
     * \param e The SQL exception object.
     */
    void logSqlError(const std::string& methodName, const pqxx::sql_error& e);
    /**
     * \brief Converts an empty string to std::nullopt, otherwise returns
     *        the string unchanged. Used to store optional text fields as
     *        SQL NULL instead of an empty string.
     */
    static std::optional<std::string> nullIfEmpty(const std::string& value);
    /**
     * \brief Parses free-text numeric input into an optional integer.
     *
     * An empty string maps to std::nullopt (the value is optional). A
     * non-empty string that isn't a valid integer throws
     * std::invalid_argument, so the caller can reject bad input instead of
     * silently storing garbage or crashing on an uncaught exception.
     *
     * \param value The text to parse.
     * \param fieldName Human-readable field name, used in the error message.
     */
    static std::optional<int> parseOptionalInt(const std::string& value, const std::string& fieldName);

public:
#pragma region Constructor
    /**
     * \brief Constructs MainFeatures.
     * \param dbConnect Database connection.
     */
    MainFeatures(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     */
    virtual ~MainFeatures();
#pragma endregion

#pragma region Methods
    /**
     * \brief Prints information about all books, including their authors.
     */
    virtual void showInfoOfBooks() override;
    /**
     * \brief Prints information about all library readers.
     */
    virtual void showInfoOfReaders() override;
    /**
     * \brief Prints information about all reader visits.
     */
    virtual void showInfoOfVisits() override;
    /**
     * \brief Prints the most popular books, ranked by visit count.
     */
    virtual void showMostPopularBooks() override;
    /**
     * \brief Adds a new book (and, if needed, a new author) to the catalog.
     */
    virtual void addBook(const std::string& title, const std::string& author,
        const std::string& genre, const std::string& publisher,
        const std::string& year, const std::string& isbn) override;
    /**
     * \brief Adds a new reader, along with their category details.
     */
    virtual void addReader(const std::string& name, const std::string& address,
        const std::string& phone, const std::string& categoryType,
        const std::string& institutionName, const std::string& major,
        const std::string& schoolNumber, const std::string& kindergartenName,
        const std::string& researchField) override;
    /**
     * \brief Records a new visit for an existing reader and library.
     */
    virtual void addVisit(const std::string& readerName, const std::string& libraryName,
        const std::string& bookTitle) override;
    /**
     * \brief Lists the names of all existing readers, for use in a
     *        selection list.
     */
    virtual std::vector<std::string> getReaderNames() override;
    /**
     * \brief Lists the names of all existing libraries, for use in a
     *        selection list.
     */
    virtual std::vector<std::string> getLibraryNames() override;
    /**
     * \brief Lists the titles of all existing books, for use in a
     *        selection list.
     */
    virtual std::vector<std::string> getBookTitles() override;
#pragma endregion
};
