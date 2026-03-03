#pragma once

/**
 * \interface IDBConnect
 * \brief Інтерфейс для підключення до бази даних.
 */
class  IDBConnect {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Забезпечує коректне знищення похідних класів при використанні через вказівник на базовий клас.
     */
    virtual ~IDBConnect() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Встановити підключення до бази даних.
     *
     * \return true, якщо підключення успішне; інакше — false.
     */
    virtual bool connect() = 0;
    /**
     * \brief Закрити підключення до бази даних.
     *
     * Закриває з'єднання, щоб уникнути витоків ресурсів.
     */
    virtual void disconnect() = 0;
    /**
     * \brief Перевірити статус підключення.
     *
     * \return true, якщо з'єднання активне; інакше — false.
     */
    virtual bool isConnected() const = 0;
#pragma endregion
};
