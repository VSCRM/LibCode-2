#pragma once

/**
 * \interface IDBConnect
 * \brief Interface for a database connection.
 */
class  IDBConnect {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Ensures derived classes are destroyed correctly when used through a
     * pointer to this base class.
     */
    virtual ~IDBConnect() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Opens the database connection.
     *
     * \return true if the connection succeeded; false otherwise.
     */
    virtual bool connect() = 0;
    /**
     * \brief Closes the database connection.
     *
     * Closes the connection to avoid resource leaks.
     */
    virtual void disconnect() = 0;
    /**
     * \brief Checks the connection status.
     *
     * \return true if the connection is active; false otherwise.
     */
    virtual bool isConnected() const = 0;
#pragma endregion
};
