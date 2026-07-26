#include "LibrarianMenu.h"
#include "MainFeatures.h"
#include "Logger.h"
#include <iostream>
#include <utility>
#include <vector>

#pragma region Constructor
LibrarianMenu::LibrarianMenu(std::shared_ptr<IDBConnect> dbConnect, std::shared_ptr<IConsoleUtils> consoleUtils)
    : dbConnect(std::move(dbConnect)), consoleUtils(std::move(consoleUtils)) {
    Logger::getInstance().log("[LibrarianMenu] Constructor called via the factory method.");
}
#pragma endregion

#pragma region Destructor
LibrarianMenu::~LibrarianMenu() {
    Logger::getInstance().log("[LibrarianMenu] Destructor called via the factory method. Cleaning up.");
}
#pragma endregion

#pragma region Methods
std::vector<std::string> LibrarianMenu::getEntries() const {
    return {
        "Information about books",
        "Information about readers",
        "Information about visits",
        "Most popular books",
        "Add book",
        "Add reader",
        "Add visit",
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
    case 4: {
        NewBookInput input = consoleUtils->showAddBookWindow();
        if (!input.cancelled) {
            mainFeatures.addBook(input.title, input.author, input.genre, input.publisher, input.year, input.isbn);
        }
        return true;
    }
    case 5: {
        NewReaderInput input = consoleUtils->showAddReaderWindow();
        if (!input.cancelled) {
            mainFeatures.addReader(input.name, input.address, input.phone, input.categoryType,
                input.institutionName, input.major, input.schoolNumber,
                input.kindergartenName, input.researchField);
        }
        return true;
    }
    case 6: {
        std::vector<std::string> readerNames = mainFeatures.getReaderNames();
        std::vector<std::string> libraryNames = mainFeatures.getLibraryNames();
        std::vector<std::string> bookTitles = mainFeatures.getBookTitles();
        NewVisitInput input = consoleUtils->showAddVisitWindow(readerNames, libraryNames, bookTitles);
        if (!input.cancelled) {
            mainFeatures.addVisit(input.readerName, input.libraryName, input.bookTitle);
        }
        return true;
    }
    case 7:
        return false;
    default:
        return true;
    }
}
#pragma endregion
