#pragma once

#include <string>

/**
 * \interface IAuthorization
 * \brief Interface for user authorization.
 *
 * Defines the operations used to verify user credentials and get a role.
 */
class IAuthorization {
public:
#pragma region Destructor
    /**
     * \brief Destructor.
     *
     * Virtual, to ensure derived classes are destroyed correctly through a
     * pointer to this base class.
     */
    virtual ~IAuthorization() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Authorizes a user.
     *
     * Verifies the user's credentials and authorizes them if the login and
     * password are correct.
     *
     * \param username User's login.
     * \param password User's password.
     * \return true if authorization succeeded, false otherwise.
     */
    virtual bool authorize(const std::string& username, const std::string& password) = 0;
    /**
     * \brief Gets the user's role.
     *
     * Returns the role stored for the user in the database.
     *
     * \param role User's login (parameter name kept for source compatibility).
     * \return The user's role.
     */
    virtual std::string getUserRole(const std::string& role) = 0;
#pragma endregion
};
