#include "csv_utils.h"

namespace csv_utils {
    std::string escapeCSV(const std::string& field) {
        std::string result = field;

        if (field.find(',') != std::string::npos ||
            field.find('\"') != std::string::npos ||
            field.find('\n') != std::string::npos) {

            size_t pos = 0;
            while ((pos = result.find('\"', pos)) != std::string::npos) {
                result.insert(pos, 1, '\"');
                pos += 2;
            }

            result = '\"' + result + '\"';
        }
        return result;
    }

    std::string unescapeCSV(const std::string& field) {
        std::string result = field;

        if (result.size() >= 2 && result.front() == '\"' && result.back() == '\"') {
            result = result.substr(1, result.size() - 2);

            size_t pos = 0;
            while ((pos = result.find('\"\"', pos)) != std::string::npos) {
                result.erase(pos, 1);
                pos += 1;
            }
        }
        return result;
    }
}