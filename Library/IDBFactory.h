#pragma once

#include "IDBConnect.h"
#include "IDBOutput.h"
#include <memory>

/**
 * \interface IDBFactory
 * \brief Abstract factory for creating database-related components.
 */
class IDBFactory {
public:

#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Ensures derived-class objects are destroyed correctly.
     */
    virtual ~IDBFactory() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Creates a database connection object.
     *
     * \return A shared pointer to an object implementing IDBConnect.
     */
    virtual std::shared_ptr<IDBConnect> createConnection() = 0;
    /**
     * \brief Creates a database output object.
     *
     * \return A shared pointer to an object implementing IDBOutput.
     */
    virtual std::shared_ptr<IDBOutput> createOutput() = 0;
#pragma endregion
};
