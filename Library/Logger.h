#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * \class Logger
 * \brief Синглтон-клас для логування повідомлень у файл.
 */
class Logger {
private:
#pragma region Fields
    /**
     * \brief Файл для запису логів.
     */
    std::ofstream logFile;
    /**
     * \brief Мютекс для забезпечення потокобезпечного логування.
     */
    std::mutex logMutex;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Приватний конструктор (для патерну Singleton).
     */
    Logger();
#pragma endregion

#pragma region Destructor
    /**
     * \brief Деструктор, який закриває лог-файл.
     */
    ~Logger();
#pragma endregion

#pragma region Prohibition
    /**
     * \brief Заборонено копіювання об'єкта Logger.
     * Це дозволяє забезпечити єдину точку доступу до логера.
     */
    Logger(const Logger&) = delete;
    /**
     * \brief Заборонено копіювальне присвоєння для об'єкта Logger.
     * Це дозволяє уникнути копіювання екземплярів логера.
     */
    Logger& operator=(const Logger&) = delete;
#pragma endregion

#pragma region Methods
    /**
     * \brief Отримати єдиний екземпляр логера.
     * \return Посилання на екземпляр Logger.
     */
    static Logger& getInstance();
    /**
     * \brief Записати повідомлення у лог-файл.
     * \param message Повідомлення для запису.
     */
    void log(const std::string& message);
#pragma endregion
};