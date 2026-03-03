#include "Logger.h"
#include <chrono>

#pragma region Constructor
Logger::Logger() {
    logFile.open("log.txt", std::ios::app);
}
#pragma endregion

#pragma region Destructor
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.flush();
        logFile.close();
    }
}
#pragma endregion

#pragma region Methods
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        logFile << "[" << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S") << "] "
            << message << std::endl;

        logFile.flush();
    }
}
#pragma endregion