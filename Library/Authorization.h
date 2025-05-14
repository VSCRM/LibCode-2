#pragma once

#include "IAuthorization.h"
#include "IDBConnect.h" 
#include <string>
#include <memory> 

/**
 * \class Authorization
 * \brief Клас для авторизації користувачів.
 */
class Authorization : public IAuthorization {
private:
#pragma region Fields
    /**
     * \brief Підключення до бази даних для перевірки облікових даних користувача.
     */
    std::shared_ptr<IDBConnect> dbConnect;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор класу Authorization.
     * \param dbConnect Вказівник на об'єкт з'єднання з базою даних.
     */
    Authorization(std::shared_ptr<IDBConnect> dbConnect);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор.
     *
     * Звільняє ресурси, пов'язані з авторизацією.
     */
    virtual ~Authorization();
#pragma endregion

#pragma region Methods
    /**
     * \brief Авторизація користувача за логіном і паролем.
     * \param username Логін користувача.
     * \param password Пароль користувача.
     * \return true, якщо авторизація пройшла успішно, false — інакше.
     */
    virtual bool authorize(const std::string& username, const std::string& password) override;
    /**
     * \brief Отримати роль користувача на основі логіну.
     * \param username Логін користувача.
     * \return Роль користувача (наприклад, "бібліотекар", "адміністратор").
     */
    virtual std::string getUserRole(const std::string& username) override;
#pragma endregion
};