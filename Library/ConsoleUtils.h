#pragma once

#include "IConsoleUtils.h"
#include <string>
#include <vector>

/**
 * \class ConsoleUtils
 * \brief Клас для відображення графічного інтерфейсу в консолі.
 *
 * Забезпечує консольне виведення вікна авторизації та меню.
 */
class ConsoleUtils : public IConsoleUtils {
public:
#pragma region Constructor
    /**
     * \brief Конструктор за замовчуванням.
     */
    ConsoleUtils();
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор за замовчуванням.
     */
    virtual ~ConsoleUtils();
#pragma endregion

#pragma region Methods
    /**
     * \brief Відображає вікно авторизації в консолі.
     * \return Пара з логіна і пароля, введених користувачем.
     */
    virtual std::pair<std::string, std::string> showLoginWindow() override;
    /**
     * \brief Відображає меню з варіантами вибору в консолі.
     * \param entries Список пунктів меню.
     * \return Індекс вибраного пункту (починаючи з 1).
     */
    virtual int showMenuWindow(const std::vector<std::string>& entries) override;
#pragma endregion
};