#pragma once

#include "IConsoleUtils.h"
#include <memory>

/**
 * \class ConsoleUtilsDecorator
 * \brief Декоратор для додавання кольору до компонентів.
 *
 * Цей клас дозволяє додавати кольорові стилі до вікон авторизації та меню,
 * не змінюючи їх первісну реалізацію.
 */
class ConsoleUtilsDecorator : public IConsoleUtils {
private:
#pragma region Fields
    /**
     * \brief Оригінальний об'єкт, який буде декоровано.
     * \details Це поле зберігає вказівник на оригінальний об'єкт, який реалізує інтерфейс IConsoleUtils.
     *           Завдяки цьому поле можна розширювати функціональність без зміни початкового коду.
     */
    std::shared_ptr<IConsoleUtils> original_;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор.
     * \param original Оригінальний об'єкт, який буде декоровано.
     * \details Декоратор ініціалізує оригінальний об'єкт, на якому буде реалізовуватись додатковий функціонал.
     */
    ConsoleUtilsDecorator(std::shared_ptr<IConsoleUtils> original);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     * \details Деструктор автоматично звільняє пам'ять і викликається при завершенні роботи об'єкта.
     */
    virtual ~ConsoleUtilsDecorator();
#pragma endregion

#pragma region Methods
    /**
     * \brief Відображає вікно авторизації з кольором.
     * \return Пара з логіна і пароля, введених користувачем.
     * \details Цей метод викликає оригінальний метод і потім додає до нього кольорові стилі.
     */
    virtual std::pair<std::string, std::string> showLoginWindow() override;
    /**
     * \brief Відображає меню з варіантами вибору з кольором.
     * \param entries Список пунктів меню.
     * \return Індекс вибраного пункту (починаючи з 1).
     * \details Цей метод викликає оригінальний метод і потім додає до нього кольорові стилі.
     */
    virtual int showMenuWindow(const std::vector<std::string>& entries) override;
#pragma endregion
};