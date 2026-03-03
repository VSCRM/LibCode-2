#pragma once

#include <string>

/**
 * \interface IUserContext
 * \brief Інтерфейс, що зберігає інформацію про авторизованого користувача.
 *
 * Забезпечує доступ до основних даних про користувача, таких як ім’я та роль, які визначаються після авторизації.
 */
class IUserContext {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Забезпечує коректне знищення об'єктів похідних класів.
     */
    virtual ~IUserContext() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Отримує ім’я користувача.
     *
     * \return Ім’я авторизованого користувача.
     */
    virtual std::string getUsername() const = 0;
    /**
     * \brief Отримує роль користувача.
     *
     * \return Роль авторизованого користувача (наприклад, "admin", "librarian").
     */
    virtual std::string getRole() const = 0;
#pragma endregion
};
