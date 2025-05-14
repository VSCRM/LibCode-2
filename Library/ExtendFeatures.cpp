#include "ExtendFeatures.h"
#include "ResultAdapter.h"
#include "Logger.h"
#include "PostgresConnect.h"
#include <iostream>
#include <windows.h>
#include <thread>

#pragma region Private Methods
void ExtendFeatures::logSqlError(const std::string& methodName, const pqxx::sql_error& e) {
    Logger::getInstance().log("[ExtendFeatures] [ERROR] SQL помилка (" + methodName + ")");
    Logger::getInstance().log("? what(): " + std::string(e.what()));
    Logger::getInstance().log("? query(): " + std::string(e.query()));
}
#pragma endregion

#pragma region Constructor
ExtendFeatures::ExtendFeatures(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
ExtendFeatures::~ExtendFeatures() {
    Logger::getInstance().log("Деструктор ExtendFeatures: очищення ресурсів.");
}
#pragma endregion

#pragma region Methods
void ExtendFeatures::showInfoOfLibraries() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в showInfoOfLibraries");

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

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfLibraries", e);
    }
}

void ExtendFeatures::showInfoOfLibrarians() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в showInfoOfLibrarians");

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

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("showInfoOfLibrarians", e);
    }
}

void ExtendFeatures::addUser() {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в addUser");

        pqxx::work txn(*pgConn->getConnection());
        pqxx::result res = txn.exec(R"(
            SELECT * 
            FROM users
            ORDER BY id ASC
        )");

        if (res.empty()) {
            return;
        }

        auto& output = ResultAdapter::convert(res);
        output.writeToFile();
    }
    catch (const pqxx::sql_error& e) {
        logSqlError("addUser", e);
    }
}

void ExtendFeatures::showSystemStatistics() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring command = L"notepad.exe log.txt";

    if (!CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        Logger::getInstance().log("[ExtendFeatures] [ERROR] Не вдалося запустити Notepad у методі showSystemStatistics");
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
#pragma endregion