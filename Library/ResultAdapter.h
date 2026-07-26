#pragma once

#include <pqxx/pqxx>
#include "PostgresOutput.h"

/**
 * \class ResultAdapter
 * \brief Adapter that converts a pqxx::result into a PostgresOutput.
 *
 * Used to fill a PostgresOutput buffer from the results of an SQL query.
 */
class ResultAdapter {
public:
#pragma region Constructor
    /**
     * \brief Default constructor.
     */
    ResultAdapter() = default;
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     *
     * Logs a message when the object is destroyed.
     */
    ~ResultAdapter();
#pragma endregion

#pragma region Methods
    /**
     * \brief Converts an SQL query result into a PostgresOutput object.
     *
     * \param result The SQL query result (pqxx::result).
     * \return A new PostgresOutput with the formatted data. Each call
     *         returns its own, isolated object (instead of a reference to
     *         a shared static buffer), which is safe for repeated and
     *         concurrent use.
     */
    static PostgresOutput convert(const pqxx::result& result);
#pragma endregion
};
