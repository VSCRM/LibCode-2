#include "MenuCreator.h"
#include "Logger.h"

#pragma region Constructor
MenuCreator::MenuCreator(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
}
#pragma endregion

#pragma region Destructor
MenuCreator::~MenuCreator() {
    Logger::getInstance().log("[MenuCreator] Деструктор викликано через фабричний метод.");
}
#pragma endregion

#pragma region Methods
IMenu* MenuCreator::createMenu(const std::string& role) const {
    if (role == "admin") {
        return new AdminMenu(dbConnect);
    }
    else if (role == "librarian") {
        return new LibrarianMenu(dbConnect);
    }
    else {
        return nullptr;
    }
}
#pragma endregion