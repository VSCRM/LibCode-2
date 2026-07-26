#pragma once

#include <string>
#include <vector>

/**
 * \interface IAddOperations
 * \brief Interface for extra admin operations (managing users, viewing stats).
 *
 * Defines the operations used to manage users and view system statistics.
 */
class IAddOperations {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Allows derived-class objects to be destroyed correctly through a
     * pointer to this interface.
     */
    virtual ~IAddOperations() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Shows information about libraries.
     *
     * Prints information about the libraries registered in the system.
     */
    virtual void showInfoOfLibraries() = 0;
    /**
     * \brief Shows information about librarians.
     *
     * Prints information about the librarians working in the system.
     */
    virtual void showInfoOfLibrarians() = 0;
    /**
     * \brief Adds a new user to the system.
     *
     * Inserts a new row into the users table with the given credentials
     * and role. \p login, \p password and \p role must all be non-empty;
     * implementations should reject the call (without touching the
     * database) if any of them is empty.
     *
     * When \p role is "librarian", a matching librarian record and work
     * schedule are also created, so \p librarianFullName, \p libraryName,
     * \p startDay, \p endDay, \p startTime and \p endTime must all be
     * non-empty too (they are ignored for any other role).
     *
     * \param login Login of the new user (must be unique).
     * \param password Password of the new user.
     * \param role Role of the new user (e.g. "admin" or "librarian").
     * \param librarianFullName Librarian's full name (librarian role only).
     * \param libraryName Name of an existing library the librarian works
     *        at (librarian role only; must match an existing library).
     * \param startDay First day of the work week as a number 1-7
     *        (1 = Monday ... 7 = Sunday; librarian role only).
     * \param endDay Last day of the work week as a number 1-7
     *        (librarian role only).
     * \param startTime Work shift start time, e.g. "09:00" (librarian role only).
     * \param endTime Work shift end time, e.g. "18:00" (librarian role only).
     */
    virtual void addUser(const std::string& login, const std::string& password, const std::string& role,
        const std::string& librarianFullName, const std::string& libraryName,
        const std::string& startDay, const std::string& endDay,
        const std::string& startTime, const std::string& endTime) = 0;
    /**
     * \brief Shows overall system statistics.
     *
     * Prints statistics such as the number of users, libraries, etc.
     */
    virtual void showSystemStatistics() = 0;
    /**
     * \brief Lists the names of all existing libraries.
     *
     * Used to populate a selection list for the "add user" form when the
     * new user is a librarian. Returns an empty vector (and logs the
     * error) if the query fails.
     */
    virtual std::vector<std::string> getLibraryNames() = 0;
#pragma endregion
};
