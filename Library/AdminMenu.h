#pragma once

#include "IMenu.h"
#include "IDBConnect.h"
#include "IConsoleUtils.h"
#include <memory>

/**
 * \class AdminMenu
 * \brief Administrator menu.
 *
 * Implements the extended menu with administrator-only capabilities.
 */
class AdminMenu : public IMenu {
private:
#pragma region Fields
    /**
     * \brief Database connection.
     */
    std::shared_ptr<IDBConnect> dbConnect;
    /**
     * \brief Console UI, used to collect input for the "add user" form.
     */
    std::shared_ptr<IConsoleUtils> consoleUtils;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs AdminMenu.
     * \param dbConnect Database connection.
     * \param consoleUtils Console UI used for the "add user" input form.
     */
    AdminMenu(std::shared_ptr<IDBConnect> dbConnect, std::shared_ptr<IConsoleUtils> consoleUtils);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     */
    virtual ~AdminMenu();
#pragma endregion

#pragma region Methods
    /**
     * \brief Returns the list of admin menu entries.
     * \return Vector of menu entry labels.
     */
    virtual std::vector<std::string> getEntries() const override;
    /**
     * \brief Handles the user's menu selection.
     * \param selected Index of the chosen menu entry.
     * \return true if the menu should be shown again; false to exit.
     */
    virtual bool handleSelection(int selected) override;
#pragma endregion
};
