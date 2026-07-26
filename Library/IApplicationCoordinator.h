#pragma once

/**
 * \interface IApplicationCoordinator
 * \brief Interface for the application coordinator.
 *
 * Manages application startup, user authorization, and navigation between
 * menus.
 */
class IApplicationCoordinator {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Ensures derived-class objects are destroyed correctly.
     */
    virtual ~IApplicationCoordinator() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Initializes the application.
     *
     * Runs the application's main logic, including user authorization.
     */
    virtual void initialize() = 0;
    /**
     * \brief Drives the user authorization process.
     *
     * If authorization succeeds, creates the menu matching the user's role.
     *
     * \return true if the application should keep running (show the login
     *         window again); false if the user asked to exit.
     */
    virtual bool handleAuthorization() = 0;
    /**
     * \brief Shows the user's menu after authorization.
     *
     * \return true if the menu was shown successfully; false otherwise.
     */
    virtual bool displayMenu() = 0;
#pragma endregion
};
