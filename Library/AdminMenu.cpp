#include "AdminMenu.h"
#include "ExtendFeatures.h"
#include "Logger.h"

#pragma region Constructor
AdminMenu::AdminMenu(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
    Logger::getInstance().log("[AdminMenu] Конструктор викликано через фабричний метод");
}
#pragma endregion

#pragma region Destructor
AdminMenu::~AdminMenu() {
    Logger::getInstance().log("[AdminMenu] Деструктор викликано через фабричний метод. Очищення.");
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
    case 2:
        extendFeatures.addUser();
        return true;
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