#pragma once
#include <string>
#include <functional>

namespace input_validation {
    std::string getValidStringInput(const std::string& prompt,
        std::function<bool(const std::string&)> validator);

    template<typename T>
    T getValidInput(const std::string& prompt, std::function<bool(const T&)> validator);

    bool isValidName(const std::string& name);
    bool isValidAddress(const std::string& address);
    bool isValidEmail(const std::string& email);
    bool isValidPhone(const std::string& phone);
    bool isValidDate(const std::string& date);
    bool isValidTime(const std::string& time);
    bool isValidPassword(const std::string& password);
}