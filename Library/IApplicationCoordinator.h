#pragma once

/**
 * \interface IApplicationCoordinator
 * \brief Інтерфейс координатора додатку.
 *
 * Керує запуском системи, авторизацією користувачів та навігацією між меню.
 */
class IApplicationCoordinator {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Забезпечує коректне знищення об'єктів похідних класів.
     */
    virtual ~IApplicationCoordinator() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Ініціалізує програму.
     *
     * Запускає основну логіку додатку, включаючи авторизацію користувача.
     */
    virtual void initialize() = 0;
    /**
     * \brief Керує процесом авторизації користувача.
     *
     * Якщо авторизація успішна, створює меню згідно з роллю користувача.
     */
    virtual void handleAuthorization() = 0;
    /**
     * \brief Показує меню користувача після авторизації.
     *
     * \return true, якщо меню було успішно показано; інакше — false.
     */
    virtual bool displayMenu() = 0;
#pragma endregion
};
