#define _CRT_SECURE_NO_WARNINGS
#include "vms.h"
#include "login.h"
#include <iostream>

int main() {
    try {
        ui::createDefaultPasswordFiles();

        VMS vms;
        vms.loadData();

        while (true) {
            std::string role = ui::login(vms);
            if (role.empty()) {
                std::cout << "Login failed. Try again? (y/n): ";
                std::string choice;
                std::getline(std::cin, choice);
                if (choice != "y" && choice != "Y") {
                    std::cout << "Exiting program.\n";
                    break;
                }
                continue;
            }

            vms.displayMenu(role);
            vms.saveData();

            std::cout << "Do you want to log in again? (y/n): ";
            std::string again;
            std::getline(std::cin, again);
            if (again != "y" && again != "Y") {
                std::cout << "Thank you for using the Veterinary Management System. Goodbye!\n";
                break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "A critical error occurred: " << e.what() << std::endl;
        std::cout << "The program will now exit." << std::endl;
        return 1;
    }

    return 0;
}