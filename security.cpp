#include "security.h"

namespace security {
    std::string simpleEncrypt(const std::string& password) {
        std::string encrypted = password;
        for (char& c : encrypted) {
            c = c + 1;
        }
        return encrypted;
    }

    std::string simpleDecrypt(const std::string& encrypted) {
        std::string password = encrypted;
        for (char& c : password) {
            c = c - 1;
        }
        return password;
    }
}