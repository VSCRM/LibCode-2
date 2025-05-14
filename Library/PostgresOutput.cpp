#pragma endregion

#include "PostgresOutput.h"
#include "Logger.h"
#include <windows.h>

#pragma region Constructor
PostgresOutput::PostgresOutput() {
    Logger::getInstance().log("Створено об'єкт PostgresOutput");
    startTime = std::chrono::steady_clock::now();
}
#pragma endregion

#pragma region Destructor
PostgresOutput::~PostgresOutput() {
    Logger::getInstance().log("Закрито об'єкт PostgresOutput");
    Logger::getInstance().log("[PostgresConnect] З'єднання закрито.");
    Logger::getInstance().log("[ApplicationCoordinator] закриття всіх підмодулів.");
}
#pragma endregion

#pragma region Methods
void PostgresOutput::addColumnHeader(const std::string& header) {
    headers.push_back(header);
    Logger::getInstance().log("Додано заголовок до стовпців: " + header);
}

void PostgresOutput::addRow(const std::vector<std::string>& row) {
    buffer.push_back(row);
}

void PostgresOutput::clear() {
    headers.clear();
    buffer.clear();
    Logger::getInstance().log("Очищення");
}

void PostgresOutput::writeToFile() {
    std::ofstream outputFile("output.txt");

    // Перевірка на успішне відкриття файлу
    if (!outputFile.is_open()) {
        Logger::getInstance().log("Не вдалося відкрити файл");
        return;
    }

    Logger::getInstance().log("Початок запису в файл");
    Logger::getInstance().log("Заголовків: " + std::to_string(headers.size()) + ", рядків: " + std::to_string(buffer.size()));

    // Крок 1: Визначення ширини колонок
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

    // Визначення максимальних ширин колонок
    for (const auto& row : buffer) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i < columnWidths.size())
                columnWidths[i] = (columnWidths[i] > row[i].size()) ? columnWidths[i] : row[i].size();
        }
    }

    std::vector<std::string> outputLines;

    // Крок 2: Формування рядка для виведення
    auto buildLine = [&](const std::vector<std::string>& items) {
        std::ostringstream oss;
        for (size_t i = 0; i < items.size(); ++i) {
            size_t visualLen = visualLength(items[i]);
            oss << items[i]
                << std::string((columnWidths[i] + 2 > visualLen) ? (columnWidths[i] + 2 - visualLen) : 2, ' ');
        }
        return oss.str();
    };

    // Додавання заголовків та дефісів
    outputLines.push_back(buildLine(headers));

    std::vector<std::string> dashes(headers.size());
    for (size_t i = 0; i < headers.size(); ++i)
        dashes[i] = std::string(columnWidths[i], '-');
    outputLines.push_back(buildLine(dashes));

    // Крок 3: Додавання рядків з даними
    for (const auto& row : buffer) {
        outputLines.push_back(buildLine(row));
    }

    // Крок 4: Визначення максимальної довжини рядка
    size_t maxRowLength = 0;
    for (const auto& line : outputLines) {
        if (line.length() > maxRowLength) {
            maxRowLength = line.length();
        }
    }

    // Крок 5: Запис рядків у файл з вирівнюванням
    for (const auto& line : outputLines) {
        outputFile << line << std::string(maxRowLength - line.length(), ' ') << "\n";
    }

    outputFile.close();
    Logger::getInstance().log("Успішно записано в файл");

    // Крок 6: Відкриття файлу в Notepad, якщо є дані
    if (!buffer.empty()) {
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        std::wstring command = L"notepad.exe output.txt";
        if (CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            Logger::getInstance().log("Відкриття Блокнота");
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            Logger::getInstance().log("Користувач закрив Блокнот");
        }
        else {
            Logger::getInstance().log("Не вдалося відкрити Блокнот");
        }
    }
    else {
        Logger::getInstance().log("Не вдалося відкрити Блокнот, немає даних");
    }

    // Крок 7: Очищення файлу після перегляду (опційно)
    bool clearAfterView = false;
    if (clearAfterView) {
        std::ofstream clearFile("output.txt", std::ios::trunc);
        clearFile.close();
        Logger::getInstance().log("Очищення output.txt");
    }
}
#pragma endregion