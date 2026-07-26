#include "ConsoleUtilsDecorator.h"
#include "Logger.h"
#include <iostream>
#include <utility>
#ifdef _WIN32
#include <windows.h>
#endif

#pragma region Constructor
ConsoleUtilsDecorator::ConsoleUtilsDecorator(std::shared_ptr<IConsoleUtils> original)
    : original_(std::move(original)) {
}
#pragma endregion

#pragma region Destructor
ConsoleUtilsDecorator::~ConsoleUtilsDecorator() {
    Logger::getInstance().log("[ConsoleUtilsDecorator] Tearing down the UI.");
}
#pragma endregion

#pragma region Func to get width
namespace {
    int getTerminalWidth() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
#endif
        return 80;
    }
}
#pragma endregion

#pragma region Authorization UI
std::pair<std::string, std::string> ConsoleUtilsDecorator::showLoginWindow(bool& exitRequested) {
    int width = getTerminalWidth();
    std::string header = "Login Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;
    
    std::cout << "\033[31m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;
   
    return original_->showLoginWindow(exitRequested);
}
#pragma endregion

#pragma region Main UI
int ConsoleUtilsDecorator::showMenuWindow(const std::vector<std::string>& entries) {
    int width = getTerminalWidth();
    std::string header = "Menu Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;

    std::cout << "\033[32m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;

    return original_->showMenuWindow(entries);
}
#pragma endregion

#pragma region Add User UI
NewUserInput ConsoleUtilsDecorator::showAddUserWindow(const std::vector<std::string>& libraryNames) {
    int width = getTerminalWidth();
    std::string header = "Add User Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;

    std::cout << "\033[33m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;

    return original_->showAddUserWindow(libraryNames);
}
#pragma endregion

#pragma region Add Book UI
NewBookInput ConsoleUtilsDecorator::showAddBookWindow() {
    int width = getTerminalWidth();
    std::string header = "Add Book Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;

    std::cout << "\033[36m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;

    return original_->showAddBookWindow();
}
#pragma endregion

#pragma region Add Reader UI
NewReaderInput ConsoleUtilsDecorator::showAddReaderWindow() {
    int width = getTerminalWidth();
    std::string header = "Add Reader Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;

    std::cout << "\033[35m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;

    return original_->showAddReaderWindow();
}
#pragma endregion

#pragma region Add Visit UI
NewVisitInput ConsoleUtilsDecorator::showAddVisitWindow(const std::vector<std::string>& readerNames,
    const std::vector<std::string>& libraryNames,
    const std::vector<std::string>& bookTitles) {
    int width = getTerminalWidth();
    std::string header = "Add Visit Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;

    std::cout << "\033[34m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;

    return original_->showAddVisitWindow(readerNames, libraryNames, bookTitles);
}
#pragma endregion
