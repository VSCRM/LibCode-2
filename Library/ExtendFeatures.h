#pragma once

#include "IAddOperations.h"
#include "ResultAdapter.h"
#include <memory> 
#include <iostream>
#include <string>
#include <vector>
#include "IDBConnect.h"

/**
 * \class ExtendFeatures
 * \brief Implements the admin-only operations: managing users and viewing
 *        system statistics.
 */
class ExtendFeatures : public IAddOperations {
private:
#pragma region Fields
    /**
     * \brief Database connection.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

#pragma region Private Methods
    /**
     * \brief Logs an SQL error.
     * \param methodName Name of the method where the error occurred.
     * \param e The SQL exception object.
     */
    void logSqlError(const std::string& methodName, const pqxx::sql_error& e);
    /**
     * \brief Parses a work-week day number ("1".."7") from free text.
     *
     * Throws std::invalid_argument if the value is empty or not a valid
     * integer in the 1-7 range, so the caller can reject bad input instead
     * of silently storing garbage or crashing on an uncaught exception.
     *
     * \param value The text to parse.
     * \param fieldName Human-readable field name, used in the error message.
     */
    static int parseDayOfWeek(const std::string& value, const std::string& fieldName);
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs ExtendFeatures.
     * \param dbConnect Database connection.
     */
    ExtendFeatures(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Virtual destructor.
     */
    virtual ~ExtendFeatures();
#pragma endregion

#pragma region Methods
    /**
     * \brief Shows information about libraries.
     */
    virtual void showInfoOfLibraries() override;
    /**
     * \brief Shows information about librarians.
     */
    virtual void showInfoOfLibrarians() override;
    /**
     * \brief Adds a new user to the database. For the "librarian" role,
     *        also creates a librarian record and a work_schedule row.
     */
    virtual void addUser(const std::string& login, const std::string& password, const std::string& role,
        const std::string& librarianFullName, const std::string& libraryName,
        const std::string& startDay, const std::string& endDay,
        const std::string& startTime, const std::string& endTime) override;
    /**
     * \brief Shows overall system statistics.
     */
    virtual void showSystemStatistics() override;
    /**
     * \brief Lists the names of all existing libraries, for use in a
     *        selection list.
     */
    virtual std::vector<std::string> getLibraryNames() override;
#pragma endregion
};
