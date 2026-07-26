#pragma once

#include "IConsoleUtils.h"
#include <memory>

/**
 * \class ConsoleUtilsDecorator
 * \brief Decorator that adds color styling to the wrapped console UI.
 *
 * Lets color styles be applied to the login/menu/add-user windows without
 * modifying their original implementation.
 */
class ConsoleUtilsDecorator : public IConsoleUtils {
private:
#pragma region Fields
    /**
     * \brief The original object being decorated.
     * \details Holds a pointer to the wrapped IConsoleUtils implementation,
     *          allowing behavior to be extended without touching the
     *          original code.
     */
    std::shared_ptr<IConsoleUtils> original_;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructor.
     * \param original The original object being decorated.
     */
    ConsoleUtilsDecorator(std::shared_ptr<IConsoleUtils> original);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     */
    virtual ~ConsoleUtilsDecorator();
#pragma endregion

#pragma region Methods
    /**
     * \brief Displays the login window with added color styling.
     * \return The login/password pair entered by the user.
     */
    virtual std::pair<std::string, std::string> showLoginWindow(bool& exitRequested) override;
    /**
     * \brief Displays the menu with added color styling.
     * \param entries List of menu entries.
     * \return Index of the chosen entry.
     */
    virtual int showMenuWindow(const std::vector<std::string>& entries) override;
    /**
     * \brief Displays the "add new user" form with added color styling.
     * \param libraryNames Existing library names to choose from (librarian role only).
     * \return The entered new-user data, or a cancelled result.
     */
    virtual NewUserInput showAddUserWindow(const std::vector<std::string>& libraryNames) override;
    /**
     * \brief Displays the "add new book" form with added color styling.
     * \return The entered new-book data, or a cancelled result.
     */
    virtual NewBookInput showAddBookWindow() override;
    /**
     * \brief Displays the "add new reader" form with added color styling.
     * \return The entered new-reader data, or a cancelled result.
     */
    virtual NewReaderInput showAddReaderWindow() override;
    /**
     * \brief Displays the "add new visit" form with added color styling.
     * \param readerNames Existing reader names to choose from.
     * \param libraryNames Existing library names to choose from.
     * \param bookTitles Existing book titles to choose from.
     * \return The entered new-visit data, or a cancelled result.
     */
    virtual NewVisitInput showAddVisitWindow(const std::vector<std::string>& readerNames,
        const std::vector<std::string>& libraryNames,
        const std::vector<std::string>& bookTitles) override;
#pragma endregion
};
