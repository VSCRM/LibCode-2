#pragma once

#include "IDBConnect.h"
#include <pqxx/pqxx>
#include "pqxx/connection.hxx"
#include <string>

/**
 * \class PostgresConnect
 * \brief Manages a PostgreSQL database connection.
 *
 * Implements IDBConnect to open the connection, check its status, and
 * close it. Also exposes the underlying pqxx::connection object.
 */
class PostgresConnect : public IDBConnect {
private:
#pragma region Fields
    /**
     * \brief Pointer to the underlying PostgreSQL connection.
     */
    pqxx::connection* conn;
    /**
     * \brief Whether the connection is currently open.
     */
    bool connected;
    /**
     * \brief PostgreSQL connection string.
     */
    std::string connStr;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs PostgresConnect.
     *
     * Stores the connection string without opening the connection itself.
     * \param connectionString PostgreSQL connection string.
     */
    explicit PostgresConnect(const std::string& connectionString);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Disconnects from the database and releases resources.
     */
    virtual ~PostgresConnect();
#pragma endregion

#pragma region Methods
    /**
     * \brief Opens the PostgreSQL database connection.
     * \return true if the connection succeeded, false on error.
     */
    virtual bool connect() override;
    /**
     * \brief Closes the PostgreSQL database connection.
     */
    virtual void disconnect() override;
    /**
     * \brief Checks whether the connection is currently active.
     * \return true if active, false otherwise.
     */
    virtual bool isConnected() const override { return connected; }
    /**
     * \brief Returns a pointer to the underlying PostgreSQL connection.
     * \return A pointer to pqxx::connection, or nullptr if not connected.
     */
    pqxx::connection* getConnection() { return conn; }
    /**
     * \brief Writes a message to the log file.
     * \param message The message to write.
     */
    void logToFile(const std::string& message);
#pragma endregion
};
