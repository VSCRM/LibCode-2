#pragma once

#include "IAuthorization.h"
#include "Authorization.h"
#include <memory>
#include <chrono>
#include <string>

/**
 * \class AuthorizationProxy
 * \brief Проксі для класу Authorization з обмеженням доступу за часом.
 *
 * Дозволяє авторизацію лише у визначені години доби (з 07:00 до 16:00),
 * делегуючи виклики реальному об’єкту Authorization.
 */
class AuthorizationProxy : public IAuthorization {
private:
#pragma region Field
   /**
    * \brief Вказівник на реальний об’єкт авторизації.
    */
    std::unique_ptr<Authorization> realAuthorization;
#pragma endregion

public:
#pragma region Constructor
   /**
    * \brief Конструктор з передачею підключення до БД.
    * \param dbConnect Підключення до бази даних.
    */
    explicit AuthorizationProxy(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
   /**
    * \brief Віртуальний деструктор.
    */
    virtual ~AuthorizationProxy();
#pragma endregion

#pragma region Metods
   /**
    * \brief Авторизація з перевіркою дозволеного часу.
    * \param username Логін користувача.
    * \param password Пароль користувача.
    * \return true, якщо авторизація успішна та в дозволений час, інакше false.
    */
    virtual bool authorize(const std::string& username, const std::string& password) override;
   /**
    * \brief Отримати роль користувача.
    * \param username Логін користувача.
    * \return Роль користувача.
    */
    virtual std::string getUserRole(const std::string& username) override;
#pragma endregion
};