#include "ResultAdapter.h"
#include "Logger.h"

#pragma region Destructor
ResultAdapter::~ResultAdapter() {
    Logger::getInstance().log("[ResultAdapter] Cache cleared");
}
#pragma endregion

#pragma region Methods
PostgresOutput ResultAdapter::convert(const pqxx::result& result) {
    PostgresOutput output;

    for (int i = 0; i < result.columns(); ++i) {
        output.addColumnHeader(result.column_name(i));
    }

    for (const auto& row : result) {
        std::vector<std::string> rowData;
        for (int i = 0; i < result.columns(); ++i) {
            rowData.push_back(row[i].is_null() ? "NULL" : row[i].as<std::string>());
        }
        output.addRow(rowData);
    }

    return output;
}
#pragma endregion
