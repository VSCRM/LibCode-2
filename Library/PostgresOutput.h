#pragma once

#include "IDBOutput.h"
#include <vector>
#include <fstream>
#include <chrono>

/**
 * \class PostgresOutput
 * \brief Implements IDBOutput: formats PostgreSQL query results and writes
 *        them to a text file.
 */
class PostgresOutput : public IDBOutput {
private:
#pragma region Private Fields
    /**
     * \brief Start time of the current operation.
     *
     * Used to measure and log how long an operation took.
     */
    std::chrono::steady_clock::time_point startTime;
#pragma endregion

public:
#pragma region Fields
    /**
     * \brief Column headers.
     *
     * Holds the column headers that will be written to the file.
     */
    std::vector<std::string> headers;
    /**
     * \brief Buffer holding the row data.
     *
     * Accumulates rows before they are written to the file.
     */
    std::vector<std::vector<std::string>> buffer;
#pragma endregion

#pragma region Constructor
    /**
     * \brief Default constructor. Starts the timing measurement.
     */
    PostgresOutput();
#pragma endregion

#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Closes any open files and releases resources.
     */
    virtual ~PostgresOutput();
#pragma endregion

#pragma region Copy Prohibition
    /**
     * \brief Copying is disallowed.
     */
    PostgresOutput(const PostgresOutput&) = delete;
    /**
     * \brief Copy assignment is disallowed.
     */
    PostgresOutput& operator=(const PostgresOutput&) = delete;
#pragma endregion

#pragma region Move
    /**
     * \brief Move support (needed because ResultAdapter::convert returns
     *        PostgresOutput by value instead of a reference to a shared
     *        static object).
     */
    PostgresOutput(PostgresOutput&&) noexcept = default;
    PostgresOutput& operator=(PostgresOutput&&) noexcept = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Adds a column header.
     * \param header The column header.
     */
    virtual void addColumnHeader(const std::string& header);
    /**
     * \brief Adds a data row to the buffer.
     * \param row The row of values.
     */
    virtual void addRow(const std::vector<std::string>& row);
    /**
     * \brief Clears the buffer and headers.
     */
    virtual void clear();
    /**
     * \brief Writes the buffer to a file with automatic column alignment,
     *        and logs how long it took.
     */
    virtual void writeToFile() override;
#pragma endregion
};
