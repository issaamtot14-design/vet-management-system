#pragma once
#include <string>

namespace csv_utils {
    std::string escapeCSV(const std::string& field);
    std::string unescapeCSV(const std::string& field);
}