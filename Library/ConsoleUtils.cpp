#include "ConsoleUtils.h"
#include "Logger.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>

using namespace ftxui;

#pragma region Constructor
ConsoleUtils::ConsoleUtils() {
    Logger::getInstance().log("Конструктор ConsoleUtils: побудова графічного інтерфейсу.");
}
#pragma endregion

#pragma region Destructor
ConsoleUtils::~ConsoleUtils() {
    Logger::getInstance().log("Деструктор ConsoleUtils: очищення графічного інтерфейсу.");
}
#pragma endregion

#pragma region Authorization Ui
std::pair<std::string, std::string> ConsoleUtils::showLoginWindow() {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string login;
    std::string password;
    bool submitted = false;
    bool should_exit = false;

    InputOption input_login_option;
    InputOption input_password_option;
    input_password_option.password = true;

    auto input_login = Input(&login, "Login", input_login_option);
    auto input_password = Input(&password, "Password", input_password_option);

    auto login_button = Button("Start", [&] {
        submitted = true;
        screen.Exit();
        });

    auto exit_button = Button("Exit", [&] {
        should_exit = true;
        screen.Exit();
        });

    auto button_renderer = Renderer(Container::Horizontal({ login_button, exit_button }), [&] {
        return hbox({
            login_button->Render() | color(Color::Green) | borderRounded | size(WIDTH, GREATER_THAN, 30) | flex,
            text("  "),
            exit_button->Render() | color(Color::Red) | borderRounded | size(WIDTH, GREATER_THAN, 30) | flex
            }) | center;
        });

    auto container = Container::Vertical({
        input_login,
        input_password,
        button_renderer
        });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Sign In") | bold | center,
            separator(),
            hbox(text("Login: "), input_login->Render()) | size(WIDTH, GREATER_THAN, 2),
            hbox(text("Password: "), input_password->Render()) | size(WIDTH, GREATER_THAN, 2),
            separator(),
            button_renderer->Render() | center,
            });
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        exit(1);
    }

    if (should_exit) {
        exit(0);
    }

    return { login, password };
}
#pragma endregion

#pragma region Main UI
int ConsoleUtils::showMenuWindow(const std::vector<std::string>& entries) {
    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;

    MenuOption option;
    option.on_enter = [&] {
        screen.ExitLoopClosure()();
        };

    int max_length = 0;
    for (const auto& entry : entries) {
        max_length = std::max(max_length, static_cast<int>(entry.size()));
    }

    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return menu->Render()
            | border
            | size(WIDTH, GREATER_THAN, max_length + 2)
            | color(Color::Cyan);
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        return 1;
    }

    return selected;
}
#pragma endregion