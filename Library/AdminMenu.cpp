#include "AdminMenu.h"
#include "ExtendFeatures.h"
#include "Logger.h"
#include <utility>
#include <vector>

#pragma region Constructor
AdminMenu::AdminMenu(std::shared_ptr<IDBConnect> dbConnect, std::shared_ptr<IConsoleUtils> consoleUtils)
    : dbConnect(std::move(dbConnect)), consoleUtils(std::move(consoleUtils)) {
    Logger::getInstance().log("[AdminMenu] Constructor called via the factory method.");
}
#pragma endregion

#pragma region Destructor
AdminMenu::~AdminMenu() {
    Logger::getInstance().log("[AdminMenu] Destructor called via the factory method. Cleaning up.");
}
#pragma endregion

#pragma region Methods
std::vector<std::string> AdminMenu::getEntries() const {
    return {
        "Information about libraries",
        "Information about librarians",
        "Add user",
        "System statistics",
        "Log out"
    };
}

bool AdminMenu::handleSelection(int selection) {
    ExtendFeatures extendFeatures(dbConnect);

    switch (selection) {
    case 0:
        extendFeatures.showInfoOfLibraries();
        return true;
    case 1:
        extendFeatures.showInfoOfLibrarians();
        return true;
    case 2: {
        std::vector<std::string> libraryNames = extendFeatures.getLibraryNames();
        NewUserInput input = consoleUtils->showAddUserWindow(libraryNames);
        if (!input.cancelled) {
            extendFeatures.addUser(input.login, input.password, input.role,
                input.librarianFullName, input.libraryName,
                input.startDay, input.endDay, input.startTime, input.endTime);
        }
        return true;
    }
    case 3:
        extendFeatures.showSystemStatistics();
        return true;
    case 4:
        return false;
    default:
        return true;
    }
}
#pragma endregion
