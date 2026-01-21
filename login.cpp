#include "login.h"
#include "exceptions.h"
#include "security.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace ui {
    std::string login(VMS& vms) {
        try {
            std::string role;
            std::cout << "\nVeterinary Management System Login\n";
            std::cout << "Available roles: admin, vet, staff, customer\n";
            std::cout << "Enter role: ";
            std::getline(std::cin, role);
            std::transform(role.begin(), role.end(), role.begin(), ::tolower);

            if (role == "admin" || role == "vet" || role == "staff") {
                std::string filename = role + ".txt";
                std::ifstream infile(filename);
                if (!infile) {
                    throw FileAccessException(filename);
                }

                std::string storedPassword;
                std::getline(infile, storedPassword);
                infile.close();

                std::string password;
                std::cout << "Enter password: ";
                std::getline(std::cin, password);

                if (password != storedPassword) {
                    throw LoginFailedException();
                }

                return role;
            }
            else if (role == "customer") {
                std::cout << "\nCustomer Portal\n";
                std::cout << "1. Register New Account\n";
                std::cout << "2. Login to Existing Account\n";
                int choice = vms.getValidatedInput<int>("Enter choice (1-2): ",
                    [](int c) { return c == 1 || c == 2; });

                if (choice == 1) {
                    std::cout << "\nNew Customer Registration\n";
                    std::string name = vms.getValidatedStringInput("Enter your full name: ",
                        [&vms](const std::string& s) { return vms.validateName(s); });

                    for (const auto& owner : vms.getOwners()) {
                        if (owner.name == name) {
                            std::cout << "Customer already exists. Please login instead.\n";
                            return "";
                        }
                    }

                    int age = vms.getValidatedInput<int>("Enter your age: ",
                        [](int a) { return a >= 18 && a <= 120; });
                    std::string address = vms.getValidatedStringInput("Enter your address: ",
                        [&vms](const std::string& s) { return vms.validateAddress(s); });
                    std::string phone = vms.getValidatedStringInput("Enter your phone (11 digits): ",
                        [&vms](const std::string& s) { return vms.validatePhone(s); });
                    std::string email = vms.getValidatedStringInput("Enter your email: ",
                        [&vms](const std::string& s) { return vms.validateEmail(s); });
                    std::string password = vms.getValidatedStringInput("Create a password (min 6 characters): ",
                        [&vms](const std::string& s) { return vms.validatePassword(s); });

                    Owner newCustomer(name, age, address, phone, email, security::simpleEncrypt(password));
                    vms.addOwner(newCustomer);
                    vms.saveData();

                    std::cout << "\nRegistration successful! Welcome " << name << "!\n";
                    return name;
                }
                else {
                    std::string name = vms.getValidatedStringInput("Enter your name: ",
                        [&vms](const std::string& s) { return vms.validateName(s); });
                    std::string password = vms.getValidatedStringInput("Enter your password: ",
                        [](const std::string&) { return true; });

                    bool found = false;
                    for (const auto& owner : vms.getOwners()) {
                        if (owner.name == name) {
                            if (password == security::simpleDecrypt(owner.password)) {
                                found = true;
                                break;
                            }
                        }
                    }

                    if (!found) {
                        throw LoginFailedException();
                    }

                    std::cout << "\nLogin successful! Welcome back, " << name << "!\n";
                    return name;
                }
            }
            else {
                throw InvalidRoleException();
            }
        }
        catch (const InvalidRoleException& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Please enter a valid role (admin, vet, staff, or customer)." << std::endl;
        }
        catch (const LoginFailedException& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Please check your credentials and try again." << std::endl;
        }
        catch (const FileAccessException& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Please ensure the system is properly set up." << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "An unexpected error occurred: " << e.what() << std::endl;
        }

        return "";
    }

    void createDefaultPasswordFiles() {
        std::vector<std::pair<std::string, std::string>> roles = {
            {"admin.txt", "admin123"},
            {"vet.txt", "vet456"},
            {"staff.txt", "staff789"}
        };

        for (const auto& role : roles) {
            std::ifstream infile(role.first);
            if (!infile.good()) {
                std::ofstream outfile(role.first);
                outfile << role.second;
                outfile.close();
            }
        }
    }
}