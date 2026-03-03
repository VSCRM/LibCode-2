#pragma once

#include "IApplicationCoordinator.h"
#include "IMenuCreator.h"
#include "IAuthorization.h"
#include "IUserContext.h"
#include "IDBFactory.h"
#include "IDBConnect.h"
#include "IConsoleUtils.h" 

/**
 * \class ApplicationCoordinator
 * \brief Клас, що реалізує інтерфейс IApplicationCoordinator і керує запуском системи, авторизацією та навігацією між меню.
 *
 * Клас координує основну логіку додатку, включаючи авторизацію та створення меню для користувача.
 */
class ApplicationCoordinator : public IApplicationCoordinator {
private:
#pragma region Fields
    /**
     * \brief Об'єкт для авторизації користувачів.
     */
    std::shared_ptr<IAuthorization> authorization;
    /**
     * \brief Об'єкт для зберігання даних про авторизованого користувача.
     */
    std::shared_ptr<IUserContext> userContext;
    /**
     * \brief Об'єкт для створення відповідного меню користувача.
     */
    std::shared_ptr<IMenuCreator> menuCreator;
    /**
     * \brief Об'єкт для створення підключення до БД.
     */
    std::shared_ptr<IDBFactory> dbFactory;
    /**
     * \brief Об'єкт для зберігання з'єднання з БД.
     */
    std::shared_ptr<IDBConnect> dbConnect;
    /**
     * \brief Об'єкт класу ConsoleUtils для роботи з консольним інтерфейсом.
     *
     * Використовується для візуального представлення системи.
     */
    std::shared_ptr<IConsoleUtils> consoleUtils;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Конструктор класу ApplicationCoordinator.
     * \param authorization Вказівник на об'єкт для авторизації користувачів.
     * \param userContext Вказівник на об'єкт для зберігання інформації про авторизованого користувача.
     * \param menuCreator Вказівник на об'єкт для створення меню користувача.
     * \param dbFactory Вказівник на об'єкт для створення підключення до БД.
     */
    ApplicationCoordinator(
        std::shared_ptr<IAuthorization> authorization,
        std::shared_ptr<IUserContext> userContext,
        std::shared_ptr<IMenuCreator> menuCreator,
        std::shared_ptr<IDBFactory> dbFactory,
        std::shared_ptr<IConsoleUtils> consoleUtils
    );
#pragma endregion

#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Закриває всі відкриті файли та звільняє ресурси.
     */
    virtual ~ApplicationCoordinator();
#pragma endregion

#pragma region Methods
    /**
     * \brief Ініціалізує програму.
     *
     * Стартує основну логіку додатку, наприклад, викликає авторизацію.
     */
    virtual void initialize() override;
    /**
     * \brief Керує процесом авторизації користувача.
     *
     * Якщо авторизація пройшла успішно, створює відповідне меню для користувача.
     */
    virtual void handleAuthorization() override;
    /**
     * \brief Показує меню користувача після авторизації.
     *
     * Створює та показує відповідне меню на основі ролі користувача.
     *
     * \return true, якщо меню успішно відображено, інакше false.
     */
    virtual bool displayMenu() override;
    /**
     * \brief Очищає консоль.
     *
     * Використовує спеціальні засоби для очищення вікна консолі.
     */
    virtual void clearConsole();
    /**
     * \brief Очищає лог-файл.
     *
     * Видаляє всі дані з файлу логу.
     */
    virtual void clearLogFile();
#pragma endregion
};