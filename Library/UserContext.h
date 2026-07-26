#pragma once

#include "IUserContext.h"
#include <string>

/**
 * \class UserContext
 * \brief Implements IUserContext and stores data about the current user.
 *
 * Holds the user's name and role, and exposes them through the
 * IUserContext interface contract.
 */
class UserContext : public IUserContext {
private:
#pragma region Fields
    /**
     * \brief The user's name.
     */
    std::string username;
    /**
     * \brief The user's role.
     */
    std::string role;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs UserContext.
     *
     * Initializes the user's name and role.
     *
     * \param username The user's name.
     * \param role The user's role.
     */
    UserContext(const std::string& username, const std::string& role)
        : username(username), role(role) {
    }
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     *
     * The class holds no dynamic memory or special resources, so the
     * compiler-generated destructor is used as-is.
     */
    virtual ~UserContext() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Gets the user's name.
     *
     * \return The user's name.
     */
    virtual std::string getUsername() const override { return username; }
    /**
     * \brief Gets the user's role.
     *
     * \return The user's role.
     */
    virtual std::string getRole() const override { return role; }
#pragma endregion
};
