#include "PostgresOutput.h"
#include "Logger.h"
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif

#pragma region Constructor
PostgresOutput::PostgresOutput() {
    Logger::getInstance().log("PostgresOutput object created");
    startTime = std::chrono::steady_clock::now();
}
#pragma endregion

#pragma region Destructor
PostgresOutput::~PostgresOutput() {
    Logger::getInstance().log("PostgresOutput object destroyed");
}
#pragma endregion

#pragma region Methods
void PostgresOutput::addColumnHeader(const std::string& header) {
    headers.push_back(header);
    Logger::getInstance().log("Added column header: " + header);
}

void PostgresOutput::addRow(const std::vector<std::string>& row) {
    buffer.push_back(row);
}

void PostgresOutput::clear() {
    headers.clear();
    buffer.clear();
    Logger::getInstance().log("Cleared");
}

void PostgresOutput::writeToFile() {
    std::ofstream outputFile("output.txt");

    // Check that the file opened successfully.
    if (!outputFile.is_open()) {
        Logger::getInstance().log("Failed to open the file");
        return;
    }

    Logger::getInstance().log("Starting to write to the file");
    Logger::getInstance().log("Headers: " + std::to_string(headers.size()) + ", rows: " + std::to_string(buffer.size()));

    // Step 1: Determine column widths.
    auto visualLength = [](const std::string& str) -> size_t {
        size_t count = 0;
        for (unsigned char c : str) {
            if ((c & 0xC0) != 0x80) ++count;
        }
        return count;
    };

    std::vector<size_t> columnWidths(headers.size(), 0);
    for (size_t i = 0; i < headers.size(); ++i) {
        columnWidths[i] = visualLength(headers[i]);
    }

    // Determine the maximum column widths.
    for (const auto& row : buffer) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i < columnWidths.size())
                columnWidths[i] = (columnWidths[i] > row[i].size()) ? columnWidths[i] : row[i].size();
        }
    }

    std::vector<std::string> outputLines;

    // Step 2: Build a padded output line.
    auto buildLine = [&](const std::vector<std::string>& items) {
        std::ostringstream oss;
        for (size_t i = 0; i < items.size(); ++i) {
            size_t visualLen = visualLength(items[i]);
            oss << items[i]
                << std::string((columnWidths[i] + 2 > visualLen) ? (columnWidths[i] + 2 - visualLen) : 2, ' ');
        }
        return oss.str();
    };

    // Add the header row and a dashed separator.
    outputLines.push_back(buildLine(headers));

    std::vector<std::string> dashes(headers.size());
    for (size_t i = 0; i < headers.size(); ++i)
        dashes[i] = std::string(columnWidths[i], '-');
    outputLines.push_back(buildLine(dashes));

    // Step 3: Add the data rows.
    for (const auto& row : buffer) {
        outputLines.push_back(buildLine(row));
    }

    // Step 4: Determine the longest output line.
    size_t maxRowLength = 0;
    for (const auto& line : outputLines) {
        if (line.length() > maxRowLength) {
            maxRowLength = line.length();
        }
    }

    // Step 5: Write the aligned lines to the file.
    for (const auto& line : outputLines) {
        outputFile << line << std::string(maxRowLength - line.length(), ' ') << "\n";
    }

    outputFile.close();
    Logger::getInstance().log("Successfully wrote to the file");

    // Step 6: Open the file in Notepad, if there is any data.
    if (!buffer.empty()) {
#ifdef _WIN32
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        std::wstring command = L"notepad.exe output.txt";
        if (CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            Logger::getInstance().log("Opening Notepad");
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            Logger::getInstance().log("User closed Notepad");
        }
        else {
            Logger::getInstance().log("Failed to open Notepad");
        }
#endif
    }
    else {
        Logger::getInstance().log("Not opening Notepad, no data");
    }

    // Step 7: Clear the file after viewing (optional).
    bool clearAfterView = false;
    if (clearAfterView) {
        std::ofstream clearFile("output.txt", std::ios::trunc);
        clearFile.close();
        Logger::getInstance().log("Cleared output.txt");
    }
}
#pragma endregion
