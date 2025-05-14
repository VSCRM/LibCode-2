#include "ApplicationCoordinator.h"
#include "UserContext.h"
#include "Logger.h"
#include <fstream>
#include <iostream>

#pragma region Constructor
ApplicationCoordinator::ApplicationCoordinator(
    std::shared_ptr<IAuthorization> authorization,
    std::shared_ptr<IUserContext> userContext,
    std::shared_ptr<IMenuCreator> menuCreator,
    std::shared_ptr<IDBFactory> dbFactory,
    std::shared_ptr<IConsoleUtils> consoleUtils
) : authorization(authorization), userContext(userContext), menuCreator(menuCreator), dbFactory(dbFactory), consoleUtils(consoleUtils) {
}
#pragma endregion

#pragma region Destructor
ApplicationCoordinator::~ApplicationCoordinator() {
    Logger::getInstance().log("Деструктор ApplicationCoordinator: очищення ресурсів.");
   
    if (dbConnect) {
        dbConnect->disconnect();
        dbConnect.reset();
        Logger::getInstance().log("DB-з'єднання очищено в ApplicationCoordinator.");
    }
    authorization.reset();
    userContext.reset();
    menuCreator.reset();
    dbFactory.reset();
    consoleUtils.reset();

    Logger::getInstance().log("Всі сервіси обнулено в ApplicationCoordinator.");
}
#pragma endregion

#pragma region Methods
void ApplicationCoordinator::clearLogFile() {
    std::ofstream logFile("log.txt", std::ios::trunc);
}

void ApplicationCoordinator::initialize() {
    clearLogFile();

    dbConnect = dbFactory->createConnection();

    if (!dbConnect->connect()) {
        std::cerr << "Неможливо підключитися до бази даних." << std::endl;
        return;
    }
    while (true) {
        handleAuthorization();
    }
}

void ApplicationCoordinator::handleAuthorization() {
    bool isAuthorized = false;
    std::string username;
    std::string password;

    while (!isAuthorized) {
        auto [login, pass] = consoleUtils->showLoginWindow();
        username = login;
        password = pass;

        isAuthorized = authorization->authorize(username, password);

        if (!isAuthorized) {
            clearConsole();
        }
    }

    std::string role = authorization->getUserRole(username);
    userContext = std::make_shared<UserContext>(username, role);

    displayMenu();
    clearConsole();
}

bool ApplicationCoordinator::displayMenu() {
    std::string role = userContext->getRole();
    IMenu* menu = menuCreator->createMenu(role);

    if (menu != nullptr) {
        bool continueSession = true;
        while (continueSession) {
            clearConsole();
            std::vector<std::string> entries = menu->getEntries(); 
            int selected = consoleUtils->showMenuWindow(entries);   
            continueSession = menu->handleSelection(selected);     
            if (continueSession)
                clearConsole();
        }
        return false;
    }
    else {
        clearConsole();
        return false;
    }
}

void ApplicationCoordinator::clearConsole() {
    system("cls");
}
#pragma endregion