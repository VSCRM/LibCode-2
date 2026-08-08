#pragma once

#include <string>
#include <unordered_map>

/**
 * \class EnvConfig
 * \brief Loads key=value pairs from a .env file and exposes them to the app.
 *
 * This lets secrets such as the PostgreSQL login/password live outside the
 * source code (in a local, git-ignored .env file) instead of being hardcoded
 * in main.cpp. Real process environment variables (set via the OS, a
 * container, or CI) always take priority over values coming from the .env
 * file, so deployments can override configuration without touching any file.
 */
class EnvConfig {
private:
#pragma region Fields
    /**
     * \brief Key/value pairs parsed from the .env file.
     */
    static std::unordered_map<std::string, std::string> values;
    /**
     * \brief Whether load() has already been called.
     */
    static bool loaded;
#pragma endregion

#pragma region Private methods
    /**
     * \brief Strips leading/trailing whitespace, and a single pair of
     * surrounding quotes ("...") or ('...'), from a string.
     * \param text The string to trim.
     * \return The trimmed string.
     */
    static std::string trim(const std::string& text);
    /**
     * \brief Parses a single file into the internal `values` map.
     * \param path Path to the file to parse.
     * \return true if the file was opened and parsed, false if it does not exist.
     */
    static bool parseFile(const std::string& path);
#pragma endregion

public:
#pragma region Methods
    /**
     * \brief Reads a .env file (if present) into memory.
     *
     * Lines of the form KEY=VALUE are parsed; blank lines and lines starting
     * with '#' are ignored. Safe to call more than once (subsequent calls
     * are no-ops).
     *
     * If \p path does not exist (e.g. a developer has not created a local
     * .env yet), \p fallbackPath is tried instead - by default this is
     * ".env.example", the template committed to the repository. This means
     * the app still starts out of the box with sensible defaults; it only
     * falls back to real OS environment variables if neither file exists.
     *
     * \param path Path to the .env file (defaults to ".env" in the working directory).
     * \param fallbackPath Path tried if \p path is missing (defaults to ".env.example").
     */
    static void load(const std::string& path = ".env", const std::string& fallbackPath = ".env.example");

    /**
     * \brief Retrieves a configuration value.
     *
     * Lookup order: 1) a real OS environment variable named \p key,
     * 2) the value loaded from the .env file, 3) \p defaultValue.
     *
     * \param key Name of the variable (e.g. "DB_PASSWORD").
     * \param defaultValue Value returned if the key is not set anywhere.
     * \return The resolved value.
     */
    static std::string get(const std::string& key, const std::string& defaultValue = "");
#pragma endregion
};
