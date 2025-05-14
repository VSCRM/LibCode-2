#pragma once

#include "IAddOperations.h"
#include "ResultAdapter.h"
#include <memory> 
#include <iostream>
#include "IDBConnect.h"

/**
 * \class ExtendFeatures
 * \brief Клас, який реалізує додаткові операції для додавання користувачів та перегляду статистики.
 */
class ExtendFeatures : public IAddOperations {
private:
#pragma region Fields
    /**
     * \brief З'єднання з базою даних.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

#pragma region Private Methods
    /**
     * \brief Логування помилок SQL.
     * \param methodName Назва методу, де виникла помилка.
     * \param e Об'єкт винятку SQL.
     */
    void logSqlError(const std::string& methodName, const pqxx::sql_error& e);
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор класу ExtendFeatures.
     * \param dbConnect Підключення до бази даних.
     */
    ExtendFeatures(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     */
    virtual ~ExtendFeatures();
#pragma endregion

#pragma region Methods
    /**
     * \brief Показати інформацію про бібліотеки.
     */
    virtual void showInfoOfLibraries() override;
    /**
     * \brief Показати інформацію про бібліотекарів.
     */
    virtual void showInfoOfLibrarians() override;
    /**
     * \brief Додати нового користувача.
     */
    virtual void addUser() override;
    /**
     * \brief Показати статистику роботи системи.
     */
    virtual void showSystemStatistics() override;
#pragma endregion
};