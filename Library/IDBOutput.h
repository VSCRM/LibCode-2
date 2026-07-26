#pragma once

/**
 * \interface IDBOutput
 * \brief Interface for writing database results to an auto-aligned file.
 */
class IDBOutput {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Ensures derived-class objects are destroyed correctly.
     */
    virtual ~IDBOutput() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Writes the SQL query results to a file.
     *
     * \details Writes the previously stored query results to a file, with
     *          automatic column alignment for easy reading.
     */
    virtual void writeToFile() = 0;
#pragma endregion
};
