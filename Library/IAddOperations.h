#pragma once

/**
 * \interface IAddOperations
 * \brief Інтерфейс для додаткових операцій (додавання користувачів та перегляд статистики).
 *
 * Визначає методи для управління користувачами та перегляду статистики системи.
 */
class IAddOperations {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Дозволяє коректно знищувати об'єкти похідних класів через вказівник на базовий клас.
     */
    virtual ~IAddOperations() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Показує інформацію про бібліотеки.
     *
     * Виводить інформацію про бібліотеки в системі.
     */
    virtual void showInfoOfLibraries() = 0;
    /**
     * \brief Показує інформацію про бібліотекарів.
     *
     * Виводить інформацію про бібліотекарів, що працюють в системі.
     */
    virtual void showInfoOfLibrarians() = 0;
    /**
     * \brief Додає нового користувача в систему.
     *
     * Викликає процес введення даних користувача та збереження їх у базі даних.
     */
    virtual void addUser() = 0;
    /**
     * \brief Показує загальну статистику системи.
     *
     * Виводить статистичні дані, такі як кількість користувачів, бібліотек тощо.
     */
    virtual void showSystemStatistics() = 0;
#pragma endregion
};
