#include "ExtendFeatures.h"
#include "ResultAdapter.h"
#include "Logger.h"
#include "PostgresConnect.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <algorithm>
#include <cctype>
#ifdef _WIN32
#include <windows.h>
#endif
#include <thread>

#pragma region Private Methods
void ExtendFeatures::logSqlError(const std::string& methodName, const pqxx::sql_error& e) {
    Logger::getInstance().log("[ExtendFeatures] [ERROR] SQL error (" + methodName + ")");
    Logger::getInstance().log("  what(): " + std::string(e.what()));
    Logger::getInstance().log("  query(): " + std::string(e.query()));
}

int ExtendFeatures::parseDayOfWeek(const std::string& value, const std::string& fieldName) {
    if (value.empty()) {
        throw std::invalid_argument(fieldName + " must not be empty");
    }

    bool isNumeric = std::all_of(value.begin(), value.end(), [](unsigned char c) { return std::isdigit(c); });
    if (!isNumeric) {
        throw std::invalid_argument(fieldName + " must be a whole number, got: \"" + value + "\"");
    }

    int day;
    try {
        day = std::stoi(value);
    }
    catch (const std::exception&) {
        throw std::invalid_argument(fieldName + " is not a valid number: \"" + value + "\"");
    }

    if (day < 1 || day > 7) {
        throw std::invalid_argument(fieldName + " must be between 1 (Monday) and 7 (Sunday), got: " + value);
    }
    return day;
}
#pragma endregion

#pragma region Constructor
ExtendFeatures::ExtendFeatures(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
ExtendFeatures::~ExtendFeatures() {
    Logger::getInstance().log("ExtendFeatures destructor: releasing resources.");
}
#pragma endregion

#pragma region Methods
void ExtendFeatures::showInfoOfLibraries() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in showInfoOfLibraries");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT 
                id, 
                name, 
                location 
            FROM libraries 
            ORDER BY id
        )");

        if (res.empty()) {
            return;
        }

        PostgresOutput output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfLibraries", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[ExtendFeatures] [ERROR] showInfoOfLibraries: " + std::string(e.what()));
    }
}

void ExtendFeatures::showInfoOfLibrarians() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in showInfoOfLibrarians");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT 
                librarians.id,
                librarians.name,
                libraries.name AS work_place,
                libraries.location,
                work_schedule.start_day,
                work_schedule.end_day,
                work_schedule.start_time,
                work_schedule.end_time
            FROM librarians
                JOIN work_schedule ON librarians.id = work_schedule.worker_id
                JOIN libraries ON libraries.id = work_schedule.library_id
            ORDER BY librarians.id
        )");

        if (res.empty()) {
            return;
        }

        PostgresOutput output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfLibrarians", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[ExtendFeatures] [ERROR] showInfoOfLibrarians: " + std::string(e.what()));
    }
}

// Bug fix: the original implementation only ran a SELECT over the existing
// `users` table and never inserted anything, so "add user" silently did
// nothing. It now performs a real parameterized INSERT, and rejects empty
// login/password/role instead of silently accepting (or crashing on) them.
// For the "librarian" role, it additionally creates a librarians row and a
// work_schedule row (which library the librarian works at and their shift),
// since previously there was no way to set this at all when adding a
// librarian account.
void ExtendFeatures::addUser(const std::string& login, const std::string& password, const std::string& role,
    const std::string& librarianFullName, const std::string& libraryName,
    const std::string& startDay, const std::string& endDay,
    const std::string& startTime, const std::string& endTime) {
    try {
        if (login.empty() || password.empty() || role.empty()) {
            throw std::invalid_argument("Login, password and role must not be empty");
        }

        bool isLibrarian = (role == "librarian");
        int startDayNum = 0;
        int endDayNum = 0;
        if (isLibrarian) {
            if (librarianFullName.empty() || libraryName.empty() ||
                startTime.empty() || endTime.empty()) {
                throw std::invalid_argument(
                    "Full name, library, start time and end time must not be empty for a librarian");
            }
            startDayNum = parseDayOfWeek(startDay, "Start day");
            endDayNum = parseDayOfWeek(endDay, "End day");
        }

        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in addUser");

        pqxx::work txn(*pgConn->getConnection());

        std::optional<int> librarianId = std::nullopt;
        if (isLibrarian) {
            pqxx::result libraryRes = txn.exec_params(
                "SELECT id FROM libraries WHERE name = $1", libraryName);
            if (libraryRes.empty()) {
                throw std::invalid_argument("No library found with name: " + libraryName);
            }
            int libraryId = libraryRes[0][0].as<int>();

            pqxx::result insertedLibrarian = txn.exec_params(
                "INSERT INTO librarians (name) VALUES ($1) RETURNING id", librarianFullName);
            librarianId = insertedLibrarian[0][0].as<int>();

            txn.exec_params(
                "INSERT INTO work_schedule (worker_id, library_id, start_day, end_day, start_time, end_time) "
                "VALUES ($1, $2, $3, $4, $5, $6)",
                *librarianId, libraryId, startDayNum, endDayNum, startTime, endTime);
        }

        txn.exec_params(
            "INSERT INTO users (role, login, password, librarian_id) VALUES ($1, $2, $3, $4)",
            role, login, password, librarianId);
        txn.commit();

        Logger::getInstance().log("[ExtendFeatures] New user added: " + login);
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("addUser", e);
    }
    catch (const std::exception& e) {
        Logger::getInstance().log("[ExtendFeatures] [ERROR] addUser: " + std::string(e.what()));
    }
}

std::vector<std::string> ExtendFeatures::getLibraryNames() {
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
        Logger::getInstance().log("[ExtendFeatures] [ERROR] getLibraryNames: " + std::string(e.what()));
    }
    return names;
}

void ExtendFeatures::showSystemStatistics() {
#ifdef _WIN32
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring command = L"notepad.exe log.txt";

    if (!CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        Logger::getInstance().log("[ExtendFeatures] [ERROR] Failed to launch Notepad in showSystemStatistics");
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#else
    Logger::getInstance().log("[ExtendFeatures] showSystemStatistics is only supported on Windows");
#endif
}
#pragma endregion
