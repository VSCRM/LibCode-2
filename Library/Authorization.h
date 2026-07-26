#pragma once

#include "IAuthorization.h"
#include "IDBConnect.h" 
#include <string>
#include <memory> 

/**
 * \class Authorization
 * \brief Handles user authorization.
 */
class Authorization : public IAuthorization {
private:
#pragma region Fields
    /**
     * \brief Database connection used to verify user credentials.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs Authorization.
     * \param dbConnect Database connection.
     */
    Authorization(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     *
     * Releases resources associated with authorization.
     */
    virtual ~Authorization();
#pragma endregion

#pragma region Methods
    /**
     * \brief Authorizes a user by login and password.
     * \param username User's login.
     * \param password User's password.
     * \return true if authorization succeeded, false otherwise.
     */
    virtual bool authorize(const std::string& username, const std::string& password) override;
    /**
     * \brief Gets a user's role based on their login.
     * \param username User's login.
     * \return The user's role (e.g. "librarian", "admin").
     */
    virtual std::string getUserRole(const std::string& username) override;
#pragma endregion
};
