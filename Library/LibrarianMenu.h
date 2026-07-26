#pragma once

#include "IMenu.h"
#include "IDBConnect.h"
#include "IConsoleUtils.h"
#include <memory>

/**
 * \class LibrarianMenu
 * \brief Librarian menu.
 *
 * Implements the librarian-facing menu logic: viewing statistics and
 * adding books, readers, and visits.
 */
class LibrarianMenu : public IMenu {
private:
#pragma region Fields
    /**
     * \brief Database connection.
     */
    std::shared_ptr<IDBConnect> dbConnect;
    /**
     * \brief Console UI, used to collect input for the "add book"/"add
     *        reader"/"add visit" forms.
     */
    std::shared_ptr<IConsoleUtils> consoleUtils;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs LibrarianMenu.
     * \param dbConnect Database connection.
     * \param consoleUtils Console UI used for the data-entry forms.
     */
    LibrarianMenu(std::shared_ptr<IDBConnect> dbConnect, std::shared_ptr<IConsoleUtils> consoleUtils);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     *
     * Releases resources associated with the librarian menu.
     */
    virtual ~LibrarianMenu();
#pragma endregion

#pragma region Methods
    /**
     * \brief Returns the list of librarian menu entries.
     * \return Vector of menu entry labels.
     */
    virtual std::vector<std::string> getEntries() const override;
    /**
     * \brief Handles the user's menu selection.
     * \param selection Index of the chosen menu entry.
     * \return true if the menu should be shown again; false to exit.
     */
    virtual bool handleSelection(int selection) override;
#pragma endregion
};
