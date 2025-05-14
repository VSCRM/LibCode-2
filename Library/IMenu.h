#pragma once

#include <string>
#include <vector>

/**
 * \interface IMenu
 * \brief Інтерфейс для консольного меню користувача.
 *
 * Визначає базові дії для роботи з меню.
 */
class IMenu {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Дозволяє коректно знищувати об'єкти похідних класів через вказівник на базовий.
     */
    virtual ~IMenu() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Обробляє вибір користувача в меню.
     *
     * \param selected Індекс обраного пункту меню.
     * \return true, якщо вибір оброблено успішно; інакше — false.
     */
    virtual bool handleSelection(int selected) = 0;
    /**
     * \brief Повертає список пунктів меню.
     *
     * \return Вектор рядків, де кожен рядок — назва пункту меню.
     */
    virtual std::vector<std::string> getEntries() const = 0;
#pragma endregion
};
