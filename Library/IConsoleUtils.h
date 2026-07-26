#pragma once

#include <string>
#include <utility>
#include <vector>

/**
 * \brief Result of the "add user" input form.
 *
 * Holds the values entered by the operator, or signals that the form
 * was cancelled (in which case none of the other fields should be used).
 * The librarian-specific fields (\c librarianFullName, \c libraryName,
 * \c startDay, \c endDay, \c startTime, \c endTime) are only populated
 * (and only meaningful) when \c role is "librarian".
 */
struct NewUserInput {
    std::string login;
    std::string password;
    std::string role;
    std::string librarianFullName;
    std::string libraryName;
    std::string startDay;
    std::string endDay;
    std::string startTime;
    std::string endTime;
    bool cancelled = false;
};

/**
 * \brief Result of the "add book" input form.
 *
 * Only \c title and \c author are required; \c genre, \c publisher,
 * \c year and \c isbn may be left empty (they become NULL in the
 * database). \c year is entered as free text and parsed/validated by the
 * caller, since ftxui's Input only edits strings.
 */
struct NewBookInput {
    std::string title;
    std::string author;
    std::string genre;
    std::string publisher;
    std::string year;
    std::string isbn;
    bool cancelled = false;
};

/**
 * \brief Result of the "add reader" input form.
 *
 * \c name and \c categoryType are required; \c address and \c phone are
 * optional. The remaining fields describe the reader's category and only
 * the ones relevant to the chosen \c categoryType need to be filled in
 * (e.g. \c schoolNumber for a pupil, \c institutionName/\c major for a
 * student); the others are simply left empty and stored as NULL.
 */
struct NewReaderInput {
    std::string name;
    std::string address;
    std::string phone;
    std::string categoryType;
    std::string institutionName;
    std::string major;
    std::string schoolNumber;
    std::string kindergartenName;
    std::string researchField;
    bool cancelled = false;
};

/**
 * \brief Result of the "add visit" input form.
 *
 * \c readerName and \c libraryName are required and must match an
 * existing reader/library; \c bookTitle is optional (a visit does not
 * have to involve borrowing a book).
 */
struct NewVisitInput {
    std::string readerName;
    std::string libraryName;
    std::string bookTitle;
    bool cancelled = false;
};

/**
 * \interface IConsoleUtils
 * \brief Interface for the class responsible for rendering the console UI.
 *
 * Provides console rendering of the login window, the menu window and the
 * data-entry forms ("add user", "add book", "add reader", "add visit").
 * This allows different UI implementations to be plugged in without
 * changing the application logic.
 */
class IConsoleUtils {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Must be virtual so that objects are destroyed correctly through a
     * pointer to this interface.
     */
    virtual ~IConsoleUtils() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Displays the login window in the console.
     *
     * Lets the user enter a login and a password. Once the user presses
     * "Start", the method returns the entered login/password pair.
     *
     * \param exitRequested Output parameter: set to true if the user chose
     *        to exit the application (the "Exit" button) or if an
     *        unrecoverable UI rendering error occurred. The caller must
     *        check this flag and shut down cleanly (through normal stack
     *        unwinding/destructors) instead of calling exit().
     * \return The login/password pair entered by the user.
     */
    virtual std::pair<std::string, std::string> showLoginWindow(bool& exitRequested) = 0;
    /**
     * \brief Displays a selectable menu in the console.
     *
     * Displays the menu options passed in `entries`. Once the user picks
     * an option, the method returns its index.
     *
     * \param entries List of menu entries the user can choose from.
     * \return Index of the chosen entry.
     */
    virtual int showMenuWindow(const std::vector<std::string>& entries) = 0;
    /**
     * \brief Displays the "add new user" input form.
     *
     * Lets the operator enter a login, a password and pick a role for the
     * new user. If the role is "librarian", also lets the operator enter
     * the librarian's full name, pick an existing library from
     * \p libraryNames, and set a work schedule (day range and shift
     * times). If the operator cancels the form, the returned
     * NewUserInput::cancelled flag is set to true and the other fields
     * must not be used.
     *
     * \param libraryNames Existing library names to choose from, used only
     *        when the operator picks the "librarian" role.
     * \return The entered new-user data, or a cancelled result.
     */
    virtual NewUserInput showAddUserWindow(const std::vector<std::string>& libraryNames) = 0;
    /**
     * \brief Displays the "add new book" input form.
     * \return The entered new-book data, or a cancelled result.
     */
    virtual NewBookInput showAddBookWindow() = 0;
    /**
     * \brief Displays the "add new reader" input form.
     * \return The entered new-reader data, or a cancelled result.
     */
    virtual NewReaderInput showAddReaderWindow() = 0;
    /**
     * \brief Displays the "add new visit" input form.
     *
     * \param readerNames Existing reader names to choose from.
     * \param libraryNames Existing library names to choose from.
     * \param bookTitles Existing book titles to choose from (a "no book"
     *        option is always available, since a visit doesn't have to
     *        involve borrowing a book).
     * \return The entered new-visit data, or a cancelled result.
     */
    virtual NewVisitInput showAddVisitWindow(const std::vector<std::string>& readerNames,
        const std::vector<std::string>& libraryNames,
        const std::vector<std::string>& bookTitles) = 0;
#pragma endregion
};
