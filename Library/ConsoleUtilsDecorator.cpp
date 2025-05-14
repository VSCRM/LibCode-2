#include "ConsoleUtilsDecorator.h"
#include "Logger.h"
#include <iostream>
#include <windows.h>

#pragma region Constructor
ConsoleUtilsDecorator::ConsoleUtilsDecorator(std::shared_ptr<IConsoleUtils> original)
    : original_(std::move(original)) {
}
#pragma endregion

#pragma region Destructor
ConsoleUtilsDecorator::~ConsoleUtilsDecorator() {
    Logger::getInstance().log("[ConsoleUtilsDecorator] очищення UI");
}
#pragma endregion

#pragma region Func to get width
int getTerminalWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80;
}
#pragma endregion

#pragma region Autorization UI
std::pair<std::string, std::string> ConsoleUtilsDecorator::showLoginWindow() {
    int width = getTerminalWidth();
    std::string header = "Login Window (Styled by Decorator)";

    int padding = (width - static_cast<int>(header.length())) / 2;
    if (padding < 0) padding = 0;
    
    std::cout << "\033[31m" << std::string(padding, ' ') << header << "\033[0m" << std::endl;
   
    return original_->showLoginWindow();
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