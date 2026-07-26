#include "ApplicationCoordinator.h"
#include "UserContext.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <memory>

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
    Logger::getInstance().log("ApplicationCoordinator destructor: releasing resources.");
   
    if (dbConnect) {
        dbConnect->disconnect();
        dbConnect.reset();
        Logger::getInstance().log("DB connection released in ApplicationCoordinator.");
    }
    authorization.reset();
    userContext.reset();
    menuCreator.reset();
    dbFactory.reset();
    consoleUtils.reset();

    Logger::getInstance().log("All services reset in ApplicationCoordinator.");
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
        std::cerr << "Unable to connect to the database." << std::endl;
        return;
    }
    while (handleAuthorization()) {
        // Keep showing the login window until the user asks to exit.
    }
}

bool ApplicationCoordinator::handleAuthorization() {
    bool isAuthorized = false;
    std::string username;
    std::string password;

    while (!isAuthorized) {
        bool exitRequested = false;
        auto [login, pass] = consoleUtils->showLoginWindow(exitRequested);
        if (exitRequested) {
            // The user asked to exit the application: return control so the
            // stack unwinds normally and all destructors (including the DB
            // disconnect) run correctly.
            return false;
        }

        // Reject empty login/password before hitting the database at all;
        // previously an empty submission was sent straight to authorize(),
        // which meant an unnecessary round trip and no clear feedback for
        // the "nothing typed" case.
        if (login.empty() || pass.empty()) {
            std::cerr << "Login and password must not be empty." << std::endl;
            clearConsole();
            continue;
        }

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
    return true;
}

bool ApplicationCoordinator::displayMenu() {
    std::string role = userContext->getRole();
    std::unique_ptr<IMenu> menu = menuCreator->createMenu(role);

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
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
#pragma endregion
