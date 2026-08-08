#include "EnvConfig.h"
#include "Logger.h"
#include <fstream>
#include <cstdlib>

std::unordered_map<std::string, std::string> EnvConfig::values;
bool EnvConfig::loaded = false;

#pragma region Private methods
std::string EnvConfig::trim(const std::string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = text.find_last_not_of(" \t\r\n");
    std::string result = text.substr(start, end - start + 1);

    if (result.size() >= 2) {
        char first = result.front();
        char last = result.back();
        if ((first == '"' && last == '"') || (first == '\'' && last == '\'')) {
            result = result.substr(1, result.size() - 2);
        }
    }
    return result;
}

bool EnvConfig::parseFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string trimmedLine = trim(line);

        // Skip blank lines and comments.
        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            continue;
        }

        size_t separatorPos = trimmedLine.find('=');
        if (separatorPos == std::string::npos) {
            continue;
        }

        std::string key = trim(trimmedLine.substr(0, separatorPos));
        std::string value = trim(trimmedLine.substr(separatorPos + 1));

        if (!key.empty()) {
            values[key] = value;
        }
    }

    return true;
}
#pragma endregion

#pragma region Methods
void EnvConfig::load(const std::string& path, const std::string& fallbackPath) {
    if (loaded) {
        return;
    }
    loaded = true;

    if (parseFile(path)) {
        Logger::getInstance().log("[EnvConfig] Loaded configuration from " + path);
        return;
    }

    Logger::getInstance().log("[EnvConfig] " + path + " not found - trying fallback " + fallbackPath);

    if (parseFile(fallbackPath)) {
        Logger::getInstance().log("[EnvConfig] Loaded configuration from " + fallbackPath +
            " (create a " + path + " file to override these defaults).");
        return;
    }

    Logger::getInstance().log("[EnvConfig] Neither " + path + " nor " + fallbackPath +
        " was found - falling back to system environment variables only.");
}

std::string EnvConfig::get(const std::string& key, const std::string& defaultValue) {
    // A real OS environment variable always wins over the .env file, so
    // deployments (CI, containers, etc.) can override it without edits.
    const char* systemValue = std::getenv(key.c_str());
    if (systemValue != nullptr) {
        return std::string(systemValue);
    }

    auto it = values.find(key);
    if (it != values.end()) {
        return it->second;
    }

    return defaultValue;
}
#pragma endregion
