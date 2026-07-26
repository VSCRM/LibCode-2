#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * \class Logger
 * \brief Singleton class that logs messages to a file.
 */
class Logger {
private:
#pragma region Fields
    /**
     * \brief The log output file.
     */
    std::ofstream logFile;
    /**
     * \brief Mutex guarding thread-safe logging.
     */
    std::mutex logMutex;
#pragma endregion

public:
#pragma region Constructor
    /**
     * \brief Constructor (used by the Singleton pattern).
     */
    Logger();
#pragma endregion

#pragma region Destructor
    /**
     * \brief Destructor that closes the log file.
     */
    ~Logger();
#pragma endregion

#pragma region Prohibition
    /**
     * \brief Copying a Logger object is disallowed.
     * This keeps a single point of access to the logger.
     */
    Logger(const Logger&) = delete;
    /**
     * \brief Copy assignment is disallowed for Logger.
     * This avoids copying logger instances.
     */
    Logger& operator=(const Logger&) = delete;
#pragma endregion

#pragma region Methods
    /**
     * \brief Gets the single Logger instance.
     * \return A reference to the Logger instance.
     */
    static Logger& getInstance();
    /**
     * \brief Writes a message to the log file.
     * \param message The message to write.
     */
    void log(const std::string& message);
#pragma endregion
};
