#pragma once

#include "IAuthorization.h"
#include "Authorization.h"
#include <memory>
#include <chrono>
#include <string>

/**
 * \class AuthorizationProxy
 * \brief Proxy for Authorization that adds a time-of-day access restriction.
 *
 * Only allows authorization during a specific window (07:00-16:00),
 * delegating the actual work to the wrapped Authorization object.
 */
class AuthorizationProxy : public IAuthorization {
private:
#pragma region Field
   /**
    * \brief The wrapped real authorization object.
    */
    std::unique_ptr<Authorization> realAuthorization;
#pragma endregion

public:
#pragma region Constructor
   /**
    * \brief Constructs AuthorizationProxy.
    * \param dbConnect Database connection.
    */
    explicit AuthorizationProxy(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
   /**
    * \brief Virtual destructor.
    */
    virtual ~AuthorizationProxy();
#pragma endregion

#pragma region Metods
   /**
    * \brief Authorizes a user, gated by the allowed time window.
    * \param username User's login.
    * \param password User's password.
    * \return true if authorization succeeded and the current time is
    *         within the allowed window; false otherwise.
    */
    virtual bool authorize(const std::string& username, const std::string& password) override;
   /**
    * \brief Gets a user's role.
    * \param username User's login.
    * \return The user's role.
    */
    virtual std::string getUserRole(const std::string& username) override;
#pragma endregion
};
