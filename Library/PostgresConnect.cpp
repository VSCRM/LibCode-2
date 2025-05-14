#pragma endregion

#include "PostgresConnect.h"
#include "Logger.h"

#pragma region Private method
void PostgresConnect::logToFile(const std::string& message) {
    Logger::getInstance().log(message);
}
#pragma endregion

#pragma region Constructor
PostgresConnect::PostgresConnect(const std::string& connectionString)
    : conn(nullptr), connected(false), connStr(connectionString) {
    Logger::getInstance().log("Конструктор ApplicationCoordinator: координація та запуск підмодулів системи.");
    logToFile("[PostgresConnect] Конструктор викликано через фабрику. Параметри: " + connStr);
    connect();
}
#pragma endregion

#pragma region Destructor
PostgresConnect::~PostgresConnect() {
    if (connected) {
        disconnect();
    }
    logToFile("[PostgresConnect] Деструктор викликано. Об'єкт знищено.");
}
#pragma endregion

#pragma region Metods
bool PostgresConnect::connect() {
    try {
        conn = new pqxx::connection(connStr);
        if (conn->is_open()) {
            connected = true;
            logToFile("[PostgresConnect] Успішно підключено до БД");
            return true;
        }
        else {
            logToFile("[PostgresConnect] Не вдалося підключитися до БД");
            return false;
        }
    }
    catch (const std::exception& e) {
        logToFile(std::string("[PostgresConnect] Помилка при підключенні: ") + e.what());
        return false;
    }
}

void PostgresConnect::disconnect() {
    if (conn) {
        delete conn;
        conn = nullptr;
        connected = false;
        logToFile("[PostgresConnect] З'єднання завершено");
    }
    else {
        logToFile("[PostgresConnect] З'єднання вже завершено");
    }
}
#pragma endregion