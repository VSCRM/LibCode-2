#include "MenuCreator.h"
#include "Logger.h"
#include <utility>

#pragma region Constructor
MenuCreator::MenuCreator(std::shared_ptr<IDBConnect> dbConnect, std::shared_ptr<IConsoleUtils> consoleUtils)
    : dbConnect(std::move(dbConnect)), consoleUtils(std::move(consoleUtils)) {
}
#pragma endregion

#pragma region Destructor
MenuCreator::~MenuCreator() {
    Logger::getInstance().log("[MenuCreator] Destructor called via the factory method.");
}
#pragma endregion

#pragma region Methods
std::unique_ptr<IMenu> MenuCreator::createMenu(const std::string& role) const {
    if (role == "admin") {
        return std::make_unique<AdminMenu>(dbConnect, consoleUtils);
    }
    else if (role == "librarian") {
        return std::make_unique<LibrarianMenu>(dbConnect, consoleUtils);
    }
    else {
        return nullptr;
    }
}
#pragma endregion
