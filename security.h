#pragma once
#include <string>

namespace security {
    std::string simpleEncrypt(const std::string& password);
    std::string simpleDecrypt(const std::string& encrypted);
}