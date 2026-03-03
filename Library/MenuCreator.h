#pragma once

#include "IMenuCreator.h"
#include "AdminMenu.h"
#include "LibrarianMenu.h"
#include "IDBConnect.h"

/**
 * \class MenuCreator
 * \brief Конкретна фабрика для створення меню відповідно до ролі.
 */
class MenuCreator : public IMenuCreator {
private:
#pragma region Fields
    /**
     * \brief Вказівник на підключення до бази даних.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор фабрики меню.
     * \param dbConnect Підключення до бази даних.
     */
    explicit MenuCreator(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     *
     * Очищує ресурси, пов’язані з фабрикою меню.
     */
    virtual ~MenuCreator();
#pragma endregion

#pragma region Methods
    /**
     * \brief Створює об'єкт меню для конкретної ролі користувача.
     *
     * \param role Роль користувача (наприклад, "admin" або "librarian").
     * \return Вказівник на об'єкт меню, що відповідає ролі.
     */
    virtual IMenu* createMenu(const std::string& role) const override;
#pragma endregion
};