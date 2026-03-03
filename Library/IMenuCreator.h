#pragma once

#include "IMenu.h"
#include <string>

/**
 * \interface IMenuCreator
 * \brief Інтерфейс фабрики меню.
 *
 * Визначає метод створення об'єкта меню відповідно до ролі користувача.
 */
class IMenuCreator {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Забезпечує коректне знищення об'єктів похідних класів через вказівник на базовий клас.
     */
    virtual ~IMenuCreator() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Створює об'єкт меню для конкретної ролі користувача.
     *
     * Це фабричний метод, який реалізується для створення меню, що відповідає ролі користувача (наприклад, адміністратор або бібліотекар).
     *
     * \param role Роль користувача (наприклад, "admin" або "librarian").
     * \return Вказівник на об'єкт меню, що відповідає ролі.
     */
    virtual IMenu* createMenu(const std::string& role) const = 0;
#pragma endregion
};
