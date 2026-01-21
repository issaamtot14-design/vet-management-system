#include "input_validation.h"
#include <iostream>
#include <limits>
#include <regex>
#include <algorithm>

namespace input_validation {

    std::string getValidStringInput(const std::string& prompt,
        std::function<bool(const std::string&)> validator) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (validator(input)) return input;
            std::cout << "Invalid input. Please try again.\n";
        }
    }

    template<typename T>
    T getValidInput(const std::string& prompt, std::function<bool(const T&)> validator) {
        T input;
        while (true) {
            std::cout << prompt;
            if (std::cin >> input && validator(input)) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return input;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n";
        }
    }

    bool isValidName(const std::string& name) {
        return std::regex_match(name, std::regex(R"(^[A-Za-z\s\-']{2,50}$)"));
    }

    bool isValidAddress(const std::string& address) {
        return address.length() >= 5 && address.length() <= 100;
    }

    bool isValidEmail(const std::string& email) {
        return std::regex_match(email, std::regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"));
    }

    bool isValidPhone(const std::string& phone) {
        return phone.length() == 11 && std::all_of(phone.begin(), phone.end(), ::isdigit);
    }

    bool isValidDate(const std::string& date) {
        // First check format with regex
        if (!std::regex_match(date, std::regex(R"(^\d{4}-\d{2}-\d{2}$)"))) {
            return false;
        }

        // Parse the date components
        int year, month, day;
        try {
            year = std::stoi(date.substr(0, 4));
            month = std::stoi(date.substr(5, 2));
            day = std::stoi(date.substr(8, 2));
        }
        catch (...) {
            return false;
        }

        // Check month range
        if (month < 1 || month > 12) {
            return false;
        }

        // Check day range based on month
        int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        // Adjust February for leap years
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
            daysInMonth[2] = 29;
        }

        if (day < 1 || day > daysInMonth[month]) {
            return false;
        }

        return true;
    }

    bool isValidTime(const std::string& time) {
        return std::regex_match(time, std::regex(R"(^\d{2}:\d{2}$)"));
    }

    bool isValidPassword(const std::string& password) {
        return password.length() >= 6;
    }

    // Explicit template instantiation for common types
    template int getValidInput<int>(const std::string&, std::function<bool(const int&)>);
    template bool getValidInput<bool>(const std::string&, std::function<bool(const bool&)>);
}