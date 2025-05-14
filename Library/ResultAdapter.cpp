#include "ResultAdapter.h"
#include "Logger.h"

#pragma region Static object
static PostgresOutput staticOutput;
#pragma endregion

#pragma region Destructor
ResultAdapter::~ResultAdapter() {
    Logger::getInstance().log("[ResultAdapter] очищення кешу");
}
#pragma endregion

#pragma region Methods
PostgresOutput& ResultAdapter::convert(const pqxx::result& result) {
    staticOutput.clear();

    for (int i = 0; i < result.columns(); ++i) {
        staticOutput.addColumnHeader(result.column_name(i));
    }

    for (const auto& row : result) {
        std::vector<std::string> rowData;
        for (int i = 0; i < result.columns(); ++i) {
            rowData.push_back(row[i].is_null() ? "NULL" : row[i].as<std::string>());
        }
        staticOutput.addRow(rowData);
    }

    return staticOutput;
}
#pragma endregion