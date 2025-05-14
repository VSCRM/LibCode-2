#include "LibrarianMenu.h"
#include "MainFeatures.h"
#include "Logger.h"
#include <iostream>

#pragma region Constructor
LibrarianMenu::LibrarianMenu(std::shared_ptr<IDBConnect> dbConnect)
    : dbConnect(std::move(dbConnect)) {
    Logger::getInstance().log("[LibrarianMenu] Конструктор викликано через фабричний метод");
}
#pragma endregion

#pragma region Destructor
LibrarianMenu::~LibrarianMenu() {
    Logger::getInstance().log("[LibrarianMenu] Деструктор викликано через фабричний метод. Очищення.");
}
#pragma endregion

#pragma region Methods
std::vector<std::string> LibrarianMenu::getEntries() const {
    return {
        "Information about books",
        "Information about readers",
        "Information about visits",
        "Most popular books",
        "Exit"
    };
}

bool LibrarianMenu::handleSelection(int selection) {
    MainFeatures mainFeatures(dbConnect);

    switch (selection) {
    case 0:
        mainFeatures.showInfoOfBooks();
        return true;
    case 1:
        mainFeatures.showInfoOfReaders();
        return true;
    case 2:
        mainFeatures.showInfoOfVisits();
        return true;
    case 3:
        mainFeatures.showMostPopularBooks();
        return true;
    case 4:
        return false;
    default:
        return true;
    }
}
#pragma endregion