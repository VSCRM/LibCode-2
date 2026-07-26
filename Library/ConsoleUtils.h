#pragma once

#include "IConsoleUtils.h"
#include <string>
#include <vector>

/**
 * \class ConsoleUtils
 * \brief Renders the console UI (ftxui-based implementation).
 *
 * Provides console rendering of the login window, the menu window and the
 * data-entry forms.
 */
class ConsoleUtils : public IConsoleUtils {
public:
#pragma region Constructor
    /**
     * \brief Default constructor.
     */
    ConsoleUtils();
#pragma endregion

#pragma region Destructor
    /**
     * \brief Default destructor.
     */
    virtual ~ConsoleUtils();
#pragma endregion

#pragma region Methods
    /**
     * \brief Displays the login window in the console.
     * \param exitRequested Set to true if the user asked to exit the app.
     * \return The login/password pair entered by the user.
     */
    virtual std::pair<std::string, std::string> showLoginWindow(bool& exitRequested) override;
    /**
     * \brief Displays a selectable menu in the console.
     * \param entries List of menu entries.
     * \return Index of the chosen entry.
     */
    virtual int showMenuWindow(const std::vector<std::string>& entries) override;
    /**
     * \brief Displays the "add new user" input form.
     * \param libraryNames Existing library names to choose from (librarian role only).
     * \return The entered new-user data, or a cancelled result.
     */
    virtual NewUserInput showAddUserWindow(const std::vector<std::string>& libraryNames) override;
    /**
     * \brief Displays the "add new book" input form.
     * \return The entered new-book data, or a cancelled result.
     */
    virtual NewBookInput showAddBookWindow() override;
    /**
     * \brief Displays the "add new reader" input form.
     * \return The entered new-reader data, or a cancelled result.
     */
    virtual NewReaderInput showAddReaderWindow() override;
    /**
     * \brief Displays the "add new visit" input form.
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
