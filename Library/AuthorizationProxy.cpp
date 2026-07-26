#include "AuthorizationProxy.h"
#include "Logger.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#pragma region Constructor
AuthorizationProxy::AuthorizationProxy(std::shared_ptr<IDBConnect> dbConnect) {
    realAuthorization = std::make_unique<Authorization>(std::move(dbConnect));
    Logger::getInstance().log("AuthorizationProxy constructor: checking time-based access.");
}
#pragma endregion

#pragma region Destructor
AuthorizationProxy::~AuthorizationProxy() {
    Logger::getInstance().log("~AuthorizationProxy destructor: releasing resources.");
}
#pragma endregion

#pragma region Methods
bool AuthorizationProxy::authorize(const std::string& username, const std::string& password) {
    std::time_t now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);
    int hour = localTime.tm_hour;

    if (hour < 7 || hour >= 16) {
        Logger::getInstance().log(
            "Authorization attempt outside the allowed window (" + std::to_string(hour) + ":00). Access denied."
        );
        std::cerr << "Access is only allowed between 07:00 and 16:00. Current time: " << hour << ":00\n";
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return false;
    }

    Logger::getInstance().log(
        "Authorization attempt within the allowed window: " + std::to_string(hour) + ":00."
    );
    return realAuthorization->authorize(username, password);
}

std::string AuthorizationProxy::getUserRole(const std::string& username) {
    return realAuthorization->getUserRole(username);
}
#pragma endregion
