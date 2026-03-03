#pragma once

/**
 * \interface IDBOutput
 * \brief Інтерфейс для виводу даних з бази даних у файл з автовирівнюванням.
 */
class IDBOutput {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Забезпечує коректне знищення об'єктів похідних класів.
     */
    virtual ~IDBOutput() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Вивести результати SQL-запиту у файл.
     *
     * \details Виводить раніше збережені результати запиту у файл з автовирівнюванням
     *          для зручного подальшого перегляду.
     */
    virtual void writeToFile() = 0;
#pragma endregion
};
