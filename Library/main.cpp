#include "ApplicationCoordinator.h"
#include "AuthorizationProxy.h"
#include "MenuCreator.h"
#include "UserContext.h"
#include "PostgresFactory.h"
#include "Logger.h"
#include "ConsoleUtilsDecorator.h"
#include "ConsoleUtils.h" 
#include <Windows.h>
#include <thread>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    {
        std::shared_ptr<IDBFactory> dbFactory = std::make_shared<PostgresFactory>("dbname=Library user=postgres password=google24");
        std::shared_ptr<IDBConnect> dbConnect = dbFactory->createConnection();

        std::shared_ptr<IAuthorization> authorization = std::make_shared<AuthorizationProxy>(dbConnect);
        std::shared_ptr<IUserContext> userContext = std::make_shared<UserContext>("", "");
        std::shared_ptr<IMenuCreator> menuCreator = std::make_shared<MenuCreator>(dbConnect);

        std::shared_ptr<ConsoleUtils> consoleUtils = std::make_shared<ConsoleUtils>();
        std::shared_ptr<IConsoleUtils> colorDecorator = std::make_shared<ConsoleUtilsDecorator>(consoleUtils);

        ApplicationCoordinator coordinator(authorization, userContext, menuCreator, dbFactory, colorDecorator);
        coordinator.initialize();

        while (dbConnect.use_count() > 1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        dbConnect.reset();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}