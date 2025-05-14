#pragma once

#include "IDBConnect.h"
#include "IDBOutput.h"
#include <memory>

/**
 * \interface IDBFactory
 * \brief Абстрактна фабрика для створення компонентів бази даних.
 */
class IDBFactory {
public:

#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Забезпечує коректне знищення об'єктів похідних класів.
     */
    virtual ~IDBFactory() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Створити об'єкт підключення до бази даних.
     *
     * \return Спільний вказівник на об'єкт, що реалізує інтерфейс IDBConnect.
     */
    virtual std::shared_ptr<IDBConnect> createConnection() = 0;
    /**
     * \brief Створити об'єкт для виведення з бази даних.
     *
     * \return Спільний вказівник на об'єкт, що реалізує інтерфейс IDBOutput.
     */
    virtual std::shared_ptr<IDBOutput> createOutput() = 0;
#pragma endregion
};
