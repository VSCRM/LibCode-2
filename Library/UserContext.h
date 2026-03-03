#pragma once

#include "IUserContext.h"
#include <string>

/**
 * \class UserContext
 * \brief Клас, що реалізує інтерфейс IUserContext та зберігає дані про користувача.
 *
 * Цей клас відповідає за збереження інформації про користувача, зокрема його ім'я та роль.
 * Він реалізує методи доступу до цих даних згідно з інтерфейсним контрактом IUserContext.
 */
class UserContext : public IUserContext {
private:
#pragma region Fields
    /**
     * \brief Ім'я користувача.
     */
    std::string username;
    /**
     * \brief Роль користувача.
     */
    std::string role;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор класу UserContext.
     *
     * Ініціалізує ім'я та роль користувача.
     *
     * \param username Ім'я користувача.
     * \param role Роль користувача.
     */
    UserContext(const std::string& username, const std::string& role)
        : username(username), role(role) {
    }
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор класу UserContext.
     *
     * Оскільки клас не містить динамічної пам'яті або специфічних ресурсів,
     * автоматичний деструктор компілятора виконується без змін.
     */
    virtual ~UserContext() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Отримує ім'я користувача.
     *
     * \return Ім'я користувача.
     */
    virtual std::string getUsername() const override { return username; }
    /**
     * \brief Отримує роль користувача.
     *
     * \return Роль користувача.
     */
    virtual std::string getRole() const override { return role; }
#pragma endregion
};