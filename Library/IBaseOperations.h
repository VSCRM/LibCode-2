#pragma once

/**
 * \interface IBaseOperations
 * \brief Інтерфейс для виконання основних операцій (перегляд статистики).
 *
 * Визначає методи для перегляду різних статистичних даних в системі, таких як інформація про книги, читачів та візити.
 */
class IBaseOperations {
public:
#pragma region Destructor
    /**
     * \brief Віртуальний деструктор.
     *
     * Дозволяє коректно знищувати об'єкти похідних класів через вказівник на базовий клас.
     */
    virtual ~IBaseOperations() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Показує інформацію про книги.
     *
     * Виводить детальну інформацію про всі книги в системі.
     */
    virtual void showInfoOfBooks() = 0;
    /**
     * \brief Показує інформацію про читачів.
     *
     * Виводить детальну інформацію про всіх читачів системи.
     */
    virtual void showInfoOfReaders() = 0;
    /**
     * \brief Показує інформацію про візити.
     *
     * Виводить статистику про всі візити до бібліотеки.
     */
    virtual void showInfoOfVisits() = 0;
    /**
     * \brief Показує найпопулярніші книги.
     *
     * Виводить список найбільш популярних книг за певний період або за іншими критеріями.
     */
    virtual void showMostPopularBooks() = 0;
#pragma endregion
};
