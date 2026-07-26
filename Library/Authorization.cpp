#include "Authorization.h"
#include "PostgresConnect.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <utility>

#pragma region Constructor
Authorization::Authorization(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
Authorization::~Authorization() {
    Logger::getInstance().log("[Authorization] Destructor called.");
}
#pragma endregion

#pragma region Methods
static void logToFile(const std::string& message) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        logFile << std::ctime(&timeT) << ": " << message << std::endl;
    }
}

bool Authorization::authorize(const std::string& username, const std::string& password) {
    // Reject an empty username up front instead of silently sending an
    // empty-string query to the database on every keystroke-less submit.
    if (username.empty()) {
        logToFile("Authorization attempt rejected: empty username.");
        return false;
    }

    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in authorize");

        auto start = std::chrono::high_resolution_clock::now();

        pqxx::work txn(*pgConn->getConnection());
        std::string query = "SELECT login, password FROM users WHERE login = " + txn.quote(username);
        pqxx::result res = txn.exec(query);

        if (res.size() == 1) {
            std::string dbPassword = res[0]["password"].as<std::string>();
            if (dbPassword == password) {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                logToFile("Authorization for user: " + username + ". Successful, time taken: " + std::to_string(duration.count()) + " seconds.");
                return true;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        logToFile("Authorization for user: " + username + ". NOT successful, time taken: " + std::to_string(duration.count()) + " seconds.");
    }
    catch (const std::exception& e) {
        logToFile("Authorization error: " + std::string(e.what()));
    }
    return false;
}

std::string Authorization::getUserRole(const std::string& username) {
    if (username.empty()) {
        logToFile("Get-role request rejected: empty username.");
        return "";
    }

    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Unsupported connection type in getUserRole");

        auto start = std::chrono::high_resolution_clock::now();

        pqxx::work txn(*pgConn->getConnection());
        std::string query = "SELECT role FROM users WHERE login = " + txn.quote(username);
        pqxx::result res = txn.exec(query);

        if (res.size() == 1) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            logToFile("Get role for user: " + username + ". Time taken: " + std::to_string(duration.count()) + " seconds.");
            return res[0]["role"].as<std::string>();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        logToFile("Get role for user: " + username + ". Time taken: " + std::to_string(duration.count()) + " seconds.");
    }
    catch (const std::exception& e) {
        logToFile("Get-role error: " + std::string(e.what()));
    }
    return "";
}
#pragma endregion
