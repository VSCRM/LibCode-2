#pragma once

#include <string>
#include <vector>

/**
 * \interface IBaseOperations
 * \brief Interface for the base (librarian) operations: viewing statistics
 *        and adding books, readers, and visits.
 */
class IBaseOperations {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Allows derived-class objects to be destroyed correctly through a
     * pointer to this interface.
     */
    virtual ~IBaseOperations() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Shows information about books.
     *
     * Prints detailed information about all books in the system.
     */
    virtual void showInfoOfBooks() = 0;
    /**
     * \brief Shows information about readers.
     *
     * Prints detailed information about all readers in the system.
     */
    virtual void showInfoOfReaders() = 0;
    /**
     * \brief Shows information about visits.
     *
     * Prints statistics about all visits to the library.
     */
    virtual void showInfoOfVisits() = 0;
    /**
     * \brief Shows the most popular books.
     *
     * Prints the list of the most popular books for a given period or by
     * some other criteria.
     */
    virtual void showMostPopularBooks() = 0;
    /**
     * \brief Adds a new book to the catalog.
     *
     * \p title and \p author must be non-empty; implementations should
     * reject the call (without touching the database) if either is empty.
     * The remaining fields may be empty (stored as NULL).
     *
     * \param title Book title.
     * \param author Author's name (looked up or created as needed).
     * \param genre Genre (optional).
     * \param publisher Publisher (optional).
     * \param year Publication year as free text (optional; parsed by the
     *        implementation, which must reject a non-numeric, non-empty value).
     * \param isbn ISBN (optional).
     */
    virtual void addBook(const std::string& title, const std::string& author,
        const std::string& genre, const std::string& publisher,
        const std::string& year, const std::string& isbn) = 0;
    /**
     * \brief Adds a new reader to the system.
     *
     * \p name and \p categoryType must be non-empty. \p address and
     * \p phone may be empty. The category-detail fields
     * (\p institutionName, \p major, \p schoolNumber, \p kindergartenName,
     * \p researchField) are all optional; only the ones relevant to
     * \p categoryType are typically filled in.
     *
     * \param name Reader's name.
     * \param address Reader's address (optional).
     * \param phone Reader's phone number (optional).
     * \param categoryType Reader's category (e.g. "pupil", "student").
     * \param institutionName Institution/employer name (optional).
     * \param major Field of study (optional).
     * \param schoolNumber School number as free text (optional; parsed by
     *        the implementation, which must reject a non-numeric,
     *        non-empty value).
     * \param kindergartenName Kindergarten name (optional).
     * \param researchField Research field (optional).
     */
    virtual void addReader(const std::string& name, const std::string& address,
        const std::string& phone, const std::string& categoryType,
        const std::string& institutionName, const std::string& major,
        const std::string& schoolNumber, const std::string& kindergartenName,
        const std::string& researchField) = 0;
    /**
     * \brief Records a new visit for an existing reader and library.
     *
     * \p readerName and \p libraryName must be non-empty and must match an
     * existing reader/library; implementations should reject the call
     * (without touching the database) if either is empty, and log an error
     * if either lookup fails to find a match. \p bookTitle is optional (a
     * visit does not have to involve borrowing a book).
     *
     * \param readerName Name of the reader who visited.
     * \param libraryName Name of the library that was visited.
     * \param bookTitle Title of the borrowed book (optional).
     */
    virtual void addVisit(const std::string& readerName, const std::string& libraryName,
        const std::string& bookTitle) = 0;
    /**
     * \brief Lists the names of all existing readers.
     *
     * Used to populate a selection list for the "add visit" form, so the
     * operator picks an existing reader instead of typing a name that may
     * not match any row. Returns an empty vector (and logs the error) if
     * the query fails.
     */
    virtual std::vector<std::string> getReaderNames() = 0;
    /**
     * \brief Lists the names of all existing libraries.
     *
     * Used to populate a selection list for the "add visit" form. Returns
     * an empty vector (and logs the error) if the query fails.
     */
    virtual std::vector<std::string> getLibraryNames() = 0;
    /**
     * \brief Lists the titles of all existing books.
     *
     * Used to populate a selection list for the "add visit" form. Returns
     * an empty vector (and logs the error) if the query fails.
     */
    virtual std::vector<std::string> getBookTitles() = 0;
#pragma endregion
};
