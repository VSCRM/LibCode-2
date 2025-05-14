#pragma once

#include "IDBConnect.h"
#include <pqxx/pqxx>
#include <string>

/**
 * \class PostgresConnect
 * \brief Клас для підключення до бази даних PostgreSQL.
 *
 * Реалізує інтерфейс IDBConnect для встановлення з'єднання, перевірки його статусу
 * та відключення. Додатково надає доступ до внутрішнього об'єкта з'єднання.
 */
class PostgresConnect : public IDBConnect {
private:
#pragma region Fields
    /**
     * \brief Вказівник на з'єднання з PostgreSQL.
     */
    pqxx::connection* conn;
    /**
     * \brief Статус підключення.
     */
    bool connected;
    /**
     * \brief Рядок підключення до бази даних PostgreSQL.
     */
    std::string connStr;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор класу PostgresConnect.
     *
     * Ініціалізує рядок підключення без встановлення самого з'єднання.
     * \param connectionString Рядок підключення до PostgreSQL.
     */
    explicit PostgresConnect(const std::string& connectionString);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Від'єднується від бази даних та очищає ресурси.
     */
    virtual ~PostgresConnect();
#pragma endregion

#pragma region Methods
    /**
     * \brief Встановлює з'єднання з базою даних PostgreSQL.
     * \return true, якщо з'єднання було успішним, false — у випадку помилки.
     */
    virtual bool connect() override;
    /**
     * \brief Відключається від бази даних PostgreSQL.
     */
    virtual void disconnect() override;
    /**
     * \brief Перевіряє, чи активне з'єднання з БД.
     * \return true, якщо активне, інакше false.
     */
    virtual bool isConnected() const override { return connected; }
    /**
     * \brief Повертає вказівник на внутрішнє з'єднання PostgreSQL.
     * \return Вказівник на pqxx::connection або nullptr, якщо не підключено.
     */
    pqxx::connection* getConnection() { return conn; }
    /**
     * \brief Записує повідомлення у лог-файл.
     * \param message Повідомлення, яке буде записане.
     */
    void logToFile(const std::string& message);
#pragma endregion
};