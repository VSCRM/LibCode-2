#pragma once

#include "IDBFactory.h"
#include "PostgresConnect.h"
#include "PostgresOutput.h"

/**
 * \class PostgresFactory
 * \brief Concrete factory that creates PostgreSQL connections and output objects.
 *
 * Implements IDBFactory to build instances of the PostgreSQL-backed classes.
 */
class PostgresFactory : public IDBFactory {
#pragma region Fields
private:
    /**
     * \brief PostgreSQL connection string.
     */
    std::string connectionString;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructor.
     * \param connStr PostgreSQL connection string.
     */
    explicit PostgresFactory(const std::string& connStr) : connectionString(connStr) {}
#pragma endregion

#pragma region Destructor
    /**
     * \brief Virtual destructor.
     */
    virtual ~PostgresFactory() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Creates a PostgreSQL database connection.
     * \return A smart pointer to an object implementing IDBConnect.
     */
    virtual std::shared_ptr<IDBConnect> createConnection() override {
        auto connection = std::make_shared<PostgresConnect>(connectionString);
        return connection;
    }
    /**
     * \brief Creates an object for writing output to a file.
     * \return A smart pointer to an object implementing IDBOutput.
     */
    virtual std::shared_ptr<IDBOutput> createOutput() override {
        return std::make_shared<PostgresOutput>();
    }
#pragma endregion
};
