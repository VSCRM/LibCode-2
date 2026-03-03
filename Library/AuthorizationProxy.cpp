#include "AuthorizationProxy.h"
#include "Logger.h"
#include <iostream>
#include <ctime>

#pragma region Constructor
AuthorizationProxy::AuthorizationProxy(std::shared_ptr<IDBConnect> dbConnect) {
    realAuthorization = std::make_unique<Authorization>(std::move(dbConnect));
    Logger::getInstance().log("Конструктор AuthorizationProxy: перевірка часовогo доступу.");
}
#pragma endregion

#pragma region Destructor
AuthorizationProxy::~AuthorizationProxy() {
    Logger::getInstance().log("Деструктор ~AuthorizationProxy: очищення ресурсів.");
}
#pragma endregion

#pragma region Methods
bool AuthorizationProxy::authorize(const std::string& username, const std::string& password) {
    std::time_t now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);
    int hour = localTime.tm_hour;

    if (hour < 7 || hour >= 16) {
        Logger::getInstance().log(
            "Спроба авторизації поза дозволеним часом (" + std::to_string(hour) + ":00). Доступ заборонено."
        );
        std::cerr << "Доступ дозволено лише з 07:00 до 16:00. Поточний час: " << hour << ":00\n";
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return false;
    }

    Logger::getInstance().log(
        "Спроба авторизації в дозволений час: " + std::to_string(hour) + ":00."
    );
    return realAuthorization->authorize(username, password);
}

std::string AuthorizationProxy::getUserRole(const std::string& username) {
    return realAuthorization->getUserRole(username);
}
#pragma endregion