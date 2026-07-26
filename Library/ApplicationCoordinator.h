#pragma once

#include "IApplicationCoordinator.h"
#include "IMenuCreator.h"
#include "IAuthorization.h"
#include "IUserContext.h"
#include "IDBFactory.h"
#include "IDBConnect.h"
#include "IConsoleUtils.h" 

/**
 * \class ApplicationCoordinator
 * \brief Implements IApplicationCoordinator, driving startup, authorization,
 *        and navigation between menus.
 *
 * Coordinates the application's main logic, including authorization and
 * building the menu for the current user.
 */
class ApplicationCoordinator : public IApplicationCoordinator {
private:
#pragma region Fields
    /**
     * \brief Handles user authorization.
     */
    std::shared_ptr<IAuthorization> authorization;
    /**
     * \brief Holds data about the currently authorized user.
     */
    std::shared_ptr<IUserContext> userContext;
    /**
     * \brief Builds the menu matching the current user.
     */
    std::shared_ptr<IMenuCreator> menuCreator;
    /**
     * \brief Creates the database connection.
     */
    std::shared_ptr<IDBFactory> dbFactory;
    /**
     * \brief Holds the current database connection.
     */
    std::shared_ptr<IDBConnect> dbConnect;
    /**
     * \brief Console UI used to render the application.
     */
    std::shared_ptr<IConsoleUtils> consoleUtils;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructs ApplicationCoordinator.
     * \param authorization Handles user authorization.
     * \param userContext Holds data about the currently authorized user.
     * \param menuCreator Builds the menu matching the current user.
     * \param dbFactory Creates the database connection.
     * \param consoleUtils Console UI used to render the application.
     */
    ApplicationCoordinator(
        std::shared_ptr<IAuthorization> authorization,
        std::shared_ptr<IUserContext> userContext,
        std::shared_ptr<IMenuCreator> menuCreator,
        std::shared_ptr<IDBFactory> dbFactory,
        std::shared_ptr<IConsoleUtils> consoleUtils
    );
#pragma endregion

#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Closes any open resources and releases them.
     */
    virtual ~ApplicationCoordinator();
#pragma endregion

#pragma region Methods
    /**
     * \brief Initializes the application.
     *
     * Starts the application's main logic, e.g. drives authorization.
     */
    virtual void initialize() override;
    /**
     * \brief Drives the user authorization process.
     *
     * If authorization succeeds, builds and shows the matching menu.
     *
     * \return true if the application should keep running; false if the
     *         user asked to exit.
     */
    virtual bool handleAuthorization() override;
    /**
     * \brief Shows the user's menu after authorization.
     *
     * Builds and displays the menu matching the user's role.
     *
     * \return true if the menu was shown successfully; false otherwise.
     */
    virtual bool displayMenu() override;
    /**
     * \brief Clears the console.
     */
    virtual void clearConsole();
    /**
     * \brief Clears the log file.
     *
     * Removes all data from the log file.
     */
    virtual void clearLogFile();
#pragma endregion
};
