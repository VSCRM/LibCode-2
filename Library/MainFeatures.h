#pragma once

#include "IBaseOperations.h"
#include "ResultAdapter.h"
#include "IDBConnect.h"
#include <memory> 

/**
 * \class MainFeatures
 * \brief Основні функції перегляду даних для бібліотекаря та адміністратора.
 *
 * Клас надає функції для перегляду книг, читачів, візитів і найпопулярніших книг.
 */
class MainFeatures : public IBaseOperations {
private:
#pragma region Fields
    /**
     * \brief Підключення до бази даних.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

#pragma region Private Methods
    /**
     * \brief Логування помилки SQL-запиту.
     * \param methodName Назва методу, де сталася помилка.
     * \param e Об'єкт помилки SQL.
     */
    void logSqlError(const std::string& methodName, const pqxx::sql_error& e);
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор з передачею підключення до бази даних.
     * \param conn Вказівник на активне з'єднання з PostgreSQL.
     */
    MainFeatures(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     * Очищає ресурси після завершення роботи з об'єктом.
     */
    virtual ~MainFeatures();
#pragma endregion

#pragma region Methods
    /**
     * \brief Виводить інформацію про всі книги з деталями та авторами.
     */
    virtual void showInfoOfBooks() override;
    /**
     * \brief Виводить інформацію про всіх читачів бібліотеки.
     */
    virtual void showInfoOfReaders() override;
    /**
     * \brief Виводить інформацію про всі візити читачів.
     */
    virtual void showInfoOfVisits() override;
    /**
     * \brief Виводить інформацію про найпопулярніші книги на основі кількості візитів.
     */
    virtual void showMostPopularBooks() override;
#pragma endregion
};