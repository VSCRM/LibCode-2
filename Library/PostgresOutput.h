#pragma once

#include "IDBOutput.h"
#include <vector>
#include <fstream>
#include <chrono>

/**
 * \class PostgresOutput
 * \brief Реалізація інтерфейсу IDBOutput для роботи з PostgreSQL та створення текстового файлу з даними.
 */
class PostgresOutput : public IDBOutput {
private:
#pragma region Private Fields
    /**
     * \brief Файл для логування.
     *
     * Використовується для запису логів про операції з базою даних.
     */
    std::ofstream logFile;
    /**
     * \brief Файл для виведення результатів.
     *
     * Використовується для збереження результатів операцій у форматі текстового файлу.
     */
    std::ofstream outputFile;
    /**
     * \brief Час початку виконання операцій.
     *
     * Відмічає час початку операцій для логування часу виконання.
     */
    std::chrono::steady_clock::time_point startTime;
#pragma endregion

public:
#pragma region Fields
    /**
     * \brief Вектор для заголовків стовпців.
     *
     * Містить заголовки стовпців, які будуть виведені в файл.
     */
    std::vector<std::string> headers;
    /**
     * \brief Вектор рядків даних.
     *
     * Містить дані, які будуть виведені в файл.
     */
    std::vector<std::vector<std::string>> rows;
    /**
     * \brief Буфер для тимчасового зберігання даних.
     *
     * Використовується для накопичення даних перед записом у файл.
     */
    std::vector<std::vector<std::string>> buffer;
#pragma endregion

#pragma region Constructor
    /**
     * \brief Конструктор за замовчуванням. Ініціалізує лог-файл та заміряє час.
     */
    PostgresOutput();
#pragma endregion

#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Закриває всі відкриті файли та звільняє ресурси.
     */
    virtual ~PostgresOutput();
#pragma endregion

#pragma region Copy Prohibition
    /**
     * \brief Заборона копіювання об'єкта.
     */
    PostgresOutput(const PostgresOutput&) = delete;
    /**
     * \brief Заборона копіювального присвоєння.
     */
    PostgresOutput& operator=(const PostgresOutput&) = delete;
#pragma endregion

#pragma region Methods
    /**
     * \brief Додає заголовок стовпця.
     * \param header Заголовок стовпця.
     */
    virtual void addColumnHeader(const std::string& header);
    /**
     * \brief Додає рядок даних у буфер.
     * \param row Вектор рядка.
     */
    virtual void addRow(const std::vector<std::string>& row);
    /**
     * \brief Очищає буфер та заголовки.
     */
    virtual void clear();
    /**
     * \brief Виводить буфер у файл з автовирівнюванням і логуванням часу.
     */
    virtual void writeToFile() override;
#pragma endregion
};