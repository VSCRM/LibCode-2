#pragma once

#include "IMenu.h"
#include <memory>
#include <string>

/**
 * \interface IMenuCreator
 * \brief Interface for a menu factory.
 *
 * Defines the method used to create a menu object matching a user role.
 */
class IMenuCreator {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Ensures derived-class objects are destroyed correctly through a
     * pointer to this interface.
     */
    virtual ~IMenuCreator() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Creates the menu object for a given user role.
     *
     * This is the factory method implemented to build the menu matching a
     * user's role (e.g. admin or librarian).
     *
     * \param role User role (e.g. "admin" or "librarian").
     * \return A smart pointer to the menu matching the role, or nullptr if
     *         the role is unknown. std::unique_ptr guarantees automatic
     *         cleanup and prevents resource leaks in the calling code.
     */
    virtual std::unique_ptr<IMenu> createMenu(const std::string& role) const = 0;
#pragma endregion
};
