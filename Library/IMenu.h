#pragma once

#include <string>
#include <vector>

/**
 * \interface IMenu
 * \brief Interface for a console user menu.
 *
 * Defines the basic operations for working with a menu.
 */
class IMenu {
public:
#pragma region Destructor
    /**
     * \brief Virtual destructor.
     *
     * Allows derived-class objects to be destroyed correctly through a
     * pointer to the base class.
     */
    virtual ~IMenu() = default;
#pragma endregion

#pragma region Methods
    /**
     * \brief Handles the user's menu selection.
     *
     * \param selected Index of the chosen menu entry.
     * \return true if the selection was handled successfully; false
     *         otherwise.
     */
    virtual bool handleSelection(int selected) = 0;
    /**
     * \brief Returns the list of menu entries.
     *
     * \return A vector of strings, where each string is a menu entry label.
     */
    virtual std::vector<std::string> getEntries() const = 0;
#pragma endregion
};
