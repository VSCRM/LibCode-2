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
    logToFile("[PostgresConnect] Constructor called via the factory. Parameters: " + connStr);
    connect();
}
#pragma endregion

#pragma region Destructor
PostgresConnect::~PostgresConnect() {
    // Call disconnect() unconditionally (not only when connected == true),
    // because connect() may have left conn != nullptr with connected ==
    // false (e.g. if the connection object was created but never opened).
    // disconnect() is itself idempotent and safe to call repeatedly.
    disconnect();
    logToFile("[PostgresConnect] Destructor called. Object destroyed.");
}
#pragma endregion

#pragma region Metods
bool PostgresConnect::connect() {
    // If connect() is called again (e.g. after losing the connection), the
    // previous connection must be closed first, otherwise the conn pointer
    // gets overwritten and the old pqxx::connection leaks.
    if (conn) {
        disconnect();
    }

    try {
        conn = new pqxx::connection(connStr);
        if (conn->is_open()) {
            connected = true;
            logToFile("[PostgresConnect] Successfully connected to the database");
            return true;
        }
        else {
            // The connection object was created but never opened - delete
            // it immediately, otherwise conn stays "half-alive" (not
            // nullptr, but connected == false) and the destructor would
            // never free it.
            delete conn;
            conn = nullptr;
            logToFile("[PostgresConnect] Failed to connect to the database");
            return false;
        }
    }
    catch (const std::exception& e) {
        // pqxx::connection may have allocated resources before throwing;
        // if the object was still created (conn != nullptr), release it.
        delete conn;
        conn = nullptr;
        connected = false;
        logToFile(std::string("[PostgresConnect] Connection error: ") + e.what());
        return false;
    }
}

void PostgresConnect::disconnect() {
    if (conn) {
        delete conn;
        conn = nullptr;
        connected = false;
        logToFile("[PostgresConnect] Connection closed");
    }
    else {
        logToFile("[PostgresConnect] Connection was already closed");
    }
}
#pragma endregion
