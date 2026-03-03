#pragma once

#include "IDBFactory.h"
#include "PostgresConnect.h"
#include "PostgresOutput.h"

/**
 * \class PostgresFactory
 * \brief Конкретна фабрика для створення PostgreSQL-підключень та об'єктів виводу.
 *
 * Реалізує інтерфейс IDBFactory для створення екземплярів класів, що працюють з PostgreSQL.
 */
class PostgresFactory : public IDBFactory {
#pragma region Fields
private:
    /**
     * \brief Рядок підключення до бази даних PostgreSQL.
     */
    std::string connectionString;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор з параметром.
     * \param connStr Рядок підключення до бази даних PostgreSQL.
     */
    explicit PostgresFactory(const std::string& connStr) : connectionString(connStr) {}
#pragma endregion

#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     */
    virtual ~PostgresFactory() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Створює з'єднання з базою даних PostgreSQL.
     * \return Розумний вказівник на об'єкт, що реалізує IDBConnect.
     */
    virtual std::shared_ptr<IDBConnect> createConnection() override {
        auto connection = std::make_shared<PostgresConnect>(connectionString);
        return connection;
    }
    /**
     * \brief Створює об'єкт для виводу даних у файл.
     * \return Розумний вказівник на об'єкт, що реалізує IDBOutput.
     */
    virtual std::shared_ptr<IDBOutput> createOutput() override {
        return std::make_shared<PostgresOutput>();
    }
#pragma endregion
};