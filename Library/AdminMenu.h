#pragma once

#include "IMenu.h"
#include "IDBConnect.h"
#include <memory>

/**
 * \class AdminMenu
 * \brief Меню адміністратора.
 *
 * Реалізує розширене меню з додатковими можливостями для адміністратора.
 */
class AdminMenu : public IMenu {
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
    explicit AdminMenu(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     *
     * Використовується для завершення роботи меню адміністратора.
     */
    virtual ~AdminMenu();
#pragma endregion

#pragma region Methods
    /**
     * \brief Повертає список пунктів меню для адміністратора.
     * \return Вектор рядків з назвами пунктів меню.
     */
    virtual std::vector<std::string> getEntries() const override;
    /**
     * \brief Обробляє вибір користувача.
     * \param selected Номер обраного пункту меню.
     * \return true, якщо меню слід повторити; false — якщо вийти.
     */
    virtual bool handleSelection(int selected) override;
#pragma endregion
};