#include "menus.h"
#include "input_validation.h"
#include <iostream>
#include <functional>

namespace ui {
    int displayRoleMenu(const std::string& title, const std::vector<std::string>& options, int maxOptions) {
        std::cout << "\n--- " << title << " ---\n";

        for (size_t i = 0; i < options.size(); i++) {
            std::cout << (i + 1) << ". " << options[i] << "\n";
        }

        int exitOption = options.size() + 1;
        std::cout << exitOption << ". Back to Previous Menu\n";

        auto validator = [exitOption, maxOptions](int c) {
            return (c >= 1 && c <= maxOptions) || c == exitOption;
            };

        int choice = input_validation::getValidInput<int>("Enter choice: ", validator);

        return (choice == exitOption) ? -1 : choice;
    }
}