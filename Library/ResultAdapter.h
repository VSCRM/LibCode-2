#pragma once

#include <pqxx/pqxx>
#include "PostgresOutput.h"

/**
 * \class ResultAdapter
 * \brief Адаптер для перетворення pqxx::result у PostgresOutput.
 *
 * Використовується для заповнення буфера PostgresOutput на основі результатів SQL-запиту.
 */
class ResultAdapter {
public:
#pragma region Constructor
    /**
     * \brief Конструктор за замовчуванням.
     */
    ResultAdapter() = default;
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     *
     * Виводить повідомлення в лог під час знищення об'єкта.
     */
    ~ResultAdapter();
#pragma endregion

#pragma region Methods
    /**
     * \brief Перетворює результат SQL-запиту на об'єкт PostgresOutput.
     *
     * \param result Результат SQL-запиту (pqxx::result).
     * \return Посилання на об'єкт PostgresOutput з відформатованими даними.
     */
    static PostgresOutput& convert(const pqxx::result& result);
#pragma endregion
};