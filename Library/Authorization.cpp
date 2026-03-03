#include "Authorization.h"
#include "PostgresConnect.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#pragma region Constructor
Authorization::Authorization(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
Authorization::~Authorization() {
    Logger::getInstance().log("[Authorization] Деструктор викликано.");
}
#pragma endregion

#pragma region Methods
void logToFile(const std::string& message) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        logFile << std::ctime(&timeT) << ": " << message << std::endl;
    }
}

bool Authorization::authorize(const std::string& username, const std::string& password) {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в authorize");

        auto start = std::chrono::high_resolution_clock::now();

        pqxx::work txn(*pgConn->getConnection());
        std::string query = "SELECT login, password FROM users WHERE login = " + txn.quote(username);
        pqxx::result res = txn.exec(query);

        if (res.size() == 1) {
            std::string dbPassword = res[0]["password"].as<std::string>();
            if (dbPassword == password) {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                logToFile("Авторизація користувача:" + username + ". Успішна, затрачений час: " + std::to_string(duration.count()) + "секунд.");
                return true;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        logToFile("Авторизація користувача:" + username + ".  НЕ Успішна, затрачений час: " + std::to_string(duration.count()) + "секунд.");
    }
    catch (const std::exception& e) {
        logToFile("Помилка авторизації: " + std::string(e.what()));
    }
    return false;
}

std::string Authorization::getUserRole(const std::string& username) {
    try {
        auto pgConn = dynamic_cast<PostgresConnect*>(dbConnect.get());
        if (!pgConn) throw std::runtime_error("Невірний тип з'єднання в getUserRole");

        auto start = std::chrono::high_resolution_clock::now();

        pqxx::work txn(*pgConn->getConnection());
        std::string query = "SELECT role FROM users WHERE login = " + txn.quote(username);
        pqxx::result res = txn.exec(query);

        if (res.size() == 1) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            logToFile("Отримання ролі користувача:" + username + ". Затрачений час: " + std::to_string(duration.count()) + "секунд.");
            return res[0]["role"].as<std::string>();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        logToFile("Отримання ролі користувача:" + username + ". Затрачений час: " + std::to_string(duration.count()) + "секунд.");
    }
    catch (const std::exception& e) {
        logToFile("Помилка отримання ролі користувача: " + std::string(e.what()));
    }
    return "";
}
#pragma endregion