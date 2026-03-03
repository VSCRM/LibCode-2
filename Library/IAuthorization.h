#pragma once

#include <string>

/**
 * \interface IAuthorization
 * \brief Інтерфейс для авторизації користувачів.
 *
 * Визначає методи для перевірки даних користувача та отримання ролі.
 */
class IAuthorization {
public:
#pragma region Destructor
    /**
     * \brief Деструктор.
     *
     * Віртуальний деструктор для забезпечення коректного знищення похідних класів через вказівник на базовий клас.
     */
    virtual ~IAuthorization() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Авторизація користувача.
     *
     * Перевіряє дані користувача та здійснює авторизацію, якщо введені правильні логін та пароль.
     *
     * \param username Логін користувача.
     * \param password Пароль користувача.
     * \return true, якщо авторизація успішна, false — якщо не вдалося авторизувати.
     */
    virtual bool authorize(const std::string& username, const std::string& password) = 0;
    /**
     * \brief Отримати тип ролі користувача.
     *
     * Повертає роль користувача, яка зберігається в базі даних.
     *
     * \param role Роль користувача (наприклад, "адміністратор" або "бібліотекар").
     * \return Роль користувача.
     */
    virtual std::string getUserRole(const std::string& role) = 0;
#pragma endregion
};
