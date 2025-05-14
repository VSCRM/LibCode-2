#pragma once

#include "IMenu.h"
#include "IDBConnect.h"
#include <memory>

/**
 * \class LibrarianMenu
 * \brief Меню бібліотекаря.
 *
 * Реалізує логіку роботи з функціями бібліотекаря.
 */
class LibrarianMenu : public IMenu {
private:
#pragma region Fields
    /**
     * \brief Вказівник на об'єкт для підключення до бази даних.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор з параметром.
     * \param dbConnect Вказівник на підключення до бази даних.
     */
    explicit LibrarianMenu(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     *
     * Звільняє ресурси, пов’язані з меню бібліотекаря.
     */
    virtual ~LibrarianMenu();
#pragma endregion

#pragma region Methods
    /**
     * \brief Повертає список доступних пунктів меню для бібліотекаря.
     * \return Вектор рядків з назвами пунктів меню.
     */
    virtual std::vector<std::string> getEntries() const override;
    /**
     * \brief Обробляє вибір користувача.
     * \param selection Номер вибраного пункту.
     * \return true, якщо слід повторити меню; false — для виходу.
     */
    virtual bool handleSelection(int selection) override;
#pragma endregion
};