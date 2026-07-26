#pragma once

#include <string>

/**
 * \interface IUserContext
 * \brief Interface that holds information about the authorized user.
 *
 * Provides access to the basic user data (name and role) that becomes
 * available after authorization.
 */
class IUserContext {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Ensures derived-class objects are destroyed correctly.
     */
    virtual ~IUserContext() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Gets the username.
     *
     * \return The authorized user's username.
     */
    virtual std::string getUsername() const = 0;
    /**
     * \brief Gets the user's role.
     *
     * \return The authorized user's role (e.g. "admin", "librarian").
     */
    virtual std::string getRole() const = 0;
#pragma endregion
};
