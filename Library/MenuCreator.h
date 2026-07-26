#pragma once

#include "IMenuCreator.h"
#include "AdminMenu.h"
#include "LibrarianMenu.h"
#include "IDBConnect.h"
#include "IConsoleUtils.h"

/**
 * \class MenuCreator
 * \brief Concrete factory that creates the appropriate menu for a role.
 */
class MenuCreator : public IMenuCreator {
private:
#pragma region Fields
    /**
     * \brief Database connection.
     */
    std::shared_ptr<IDBConnect> dbConnect;
    /**
     * \brief Console UI, forwarded to menus that need to collect input
     *        (e.g. AdminMenu's "add user" form).
     */
    std::shared_ptr<IConsoleUtils> consoleUtils;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs the menu factory.
     * \param dbConnect Database connection.
     * \param consoleUtils Console UI passed on to created menus.
     */
    MenuCreator(std::shared_ptr<IDBConnect> dbConnect, std::shared_ptr<IConsoleUtils> consoleUtils);
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor.
     */
    virtual ~MenuCreator();
#pragma endregion

#pragma region Methods
    /**
     * \brief Creates the menu object for a given user role.
     *
     * \param role User role (e.g. "admin" or "librarian").
     * \return A smart pointer to the menu matching the role, or nullptr if
     *         the role is unknown.
     */
    virtual std::unique_ptr<IMenu> createMenu(const std::string& role) const override;
#pragma endregion
};
