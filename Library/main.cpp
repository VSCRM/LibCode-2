#include "ApplicationCoordinator.h"
#include "AuthorizationProxy.h"
#include "ConsoleUtils.h" 
#include "ConsoleUtilsDecorator.h"
#include "EnvConfig.h"
#include "MenuCreator.h"
#include "PostgresFactory.h"
#include "UserContext.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // ConsoleUtilsDecorator prints raw ANSI escape codes (\x1b[31m ...)
    // directly via std::cout. A brand-new Windows console (conhost.exe,
    // the one you get by double-clicking the .exe) does NOT interpret
    // those by default - it prints the literal escape bytes as garbage
    // text, which is exactly what showed up before the login window's
    // first colored header. ftxui itself enables this mode internally as
    // soon as the first ScreenInteractive::Loop() runs, which is why
    // everything *after* the first Start/login looked fine - by then the
    // console had already been switched into VT mode. Doing it here,
    // once, up front, makes it correct from the very first line too.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &consoleMode)) {
        SetConsoleMode(hOut, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    {
        // Load DB_* variables from a local .env file. If .env does not
        // exist yet, EnvConfig falls back to the committed .env.example
        // template, so the app still runs out of the box - create a real
        // .env (see .env.example) to use your own credentials. Values
        // already present in the real OS environment are never
        // overwritten by either file - see EnvConfig::get().
        EnvConfig::load();

        std::string dbHost = EnvConfig::get("DB_HOST", "localhost");
        std::string dbPort = EnvConfig::get("DB_PORT", "5432");
        std::string dbName = EnvConfig::get("DB_NAME", "library");
        std::string dbUser = EnvConfig::get("DB_USER", "postgres");
        std::string dbPassword = EnvConfig::get("DB_PASSWORD", "");

        if (dbPassword.empty()) {
            std::cerr << "[Error] DB_PASSWORD is not set. Create a .env file "
                         "(see .env.example) or set the DB_PASSWORD "
                         "environment variable before running the app."
                      << std::endl;
            return 1;
        }

        std::string connectionString =
            "host=" + dbHost +
            " port=" + dbPort +
            " dbname=" + dbName +
            " user=" + dbUser +
            " password=" + dbPassword;

        std::shared_ptr<IDBFactory> dbFactory = std::make_shared<PostgresFactory>(connectionString);
        std::shared_ptr<IDBConnect> dbConnect = dbFactory->createConnection();

        std::shared_ptr<IAuthorization> authorization = std::make_shared<AuthorizationProxy>(dbConnect);
        std::shared_ptr<IUserContext> userContext = std::make_shared<UserContext>("", "");

        std::shared_ptr<ConsoleUtils> consoleUtils = std::make_shared<ConsoleUtils>();
        std::shared_ptr<IConsoleUtils> colorDecorator = std::make_shared<ConsoleUtilsDecorator>(consoleUtils);

        std::shared_ptr<IMenuCreator> menuCreator = std::make_shared<MenuCreator>(dbConnect, colorDecorator);

        ApplicationCoordinator coordinator(authorization, userContext, menuCreator, dbFactory, colorDecorator);
        coordinator.initialize();

        // All shared_ptr locals above (dbConnect, authorization, menuCreator,
        // coordinator, ...) are released automatically here, in reverse
        // declaration order, as soon as this block ends. No manual wait is
        // needed - a previous version of this function busy-waited on
        // dbConnect.use_count() dropping to 1, but that could never happen:
        // the very objects holding extra references to dbConnect
        // (authorization, menuCreator, coordinator) only let go of them once
        // this same block finishes, so the wait condition could never
        // become true on its own. That made the process hang forever after
        // "Exit" was selected, which is why the console window never closed
        // by itself and had to be force-closed.
    }
    return 0;
}
