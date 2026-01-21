#define _CRT_SECURE_NO_WARNINGS

#include "vms.h"
#include "exceptions.h"
#include "csv_utils.h"
#include "security.h"
#include "menus.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <regex>
#include <ctime>
#include <iomanip>

// Private Helper Methods

template<typename T>
T VMS::getValidInput(const std::string& prompt, std::function<bool(const T&)> validator) {
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

std::string VMS::getValidStringInput(const std::string& prompt, std::function<bool(const std::string&)> validator) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (validator(input)) return input;
        std::cout << "Invalid input. Please try again.\n";
    }
}

bool VMS::isValidName(const std::string& name) const {
    return std::regex_match(name, std::regex(R"(^[A-Za-z\s\-']{2,50}$)"));
}

bool VMS::isValidAddress(const std::string& address) const {
    return address.length() >= 5 && address.length() <= 100;
}

bool VMS::isValidEmail(const std::string& email) const {
    return std::regex_match(email, std::regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"));
}

bool VMS::isValidPhone(const std::string& phone) const {
    return phone.length() == 11 && std::all_of(phone.begin(), phone.end(), ::isdigit);
}

bool VMS::isValidDate(const std::string& date) const {
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

bool VMS::isValidTime(const std::string& time) const {
    return std::regex_match(time, std::regex(R"(^\d{2}:\d{2}$)"));
}

bool VMS::isValidPassword(const std::string& password) const {
    return password.length() >= 6;
}

void VMS::updateAllAppointmentStatuses() {
    for (auto& appointment : appointments) {
        appointment.updateStatus();
    }
}

int VMS::displayRoleMenu(const std::string& title, const std::vector<std::string>& options, int maxOptions) {
    return ui::displayRoleMenu(title, options, maxOptions);
}

bool VMS::isDateTimeInFuture(const std::string& date, const std::string& time) const {
    tm apptTime = {};
    std::stringstream datetime(date + " " + time);
    datetime >> std::get_time(&apptTime, "%Y-%m-%d %H:%M");
    if (datetime.fail()) return false;

    time_t apptTimestamp = mktime(&apptTime);
    time_t now = std::time(nullptr);
    return apptTimestamp > now;
}

bool VMS::hasTimeConflict(const std::string& date, const std::string& time) const {
    for (const auto& appt : appointments) {
        if (appt.date == date && appt.time == time && appt.status != "Cancelled") {
            return true;
        }
    }
    return false;
}

bool VMS::isDuplicateAppointment(const std::string& ownerName, const std::string& petName,
    const std::string& date, const std::string& time) const {
    for (const auto& appt : appointments) {
        if (appt.owner.name == ownerName &&
            appt.pet.name == petName &&
            appt.date == date &&
            appt.time == time &&
            appt.status != "Cancelled") {
            return true;
        }
    }
    return false;
}

bool VMS::isValidStatusTransition(const std::string& currentStatus, const std::string& newStatus) {
    if (currentStatus == newStatus) return true;

    if (currentStatus == "Scheduled") {
        return newStatus == "Completed" || newStatus == "Cancelled";
    }
    else if (currentStatus == "Completed") {
        return false;
    }
    else if (currentStatus == "Cancelled") {
        return false;
    }

    return false;
}

bool VMS::isVaccinationStatusAppropriate(int petAge, bool isVaccinated) {
    if (petAge < 1 && !isVaccinated) {
        return true;
    }
    return true;
}

// Public Methods

void VMS::addOwner(const Owner& owner) {
    owners.push_back(owner);
}

const std::vector<Owner>& VMS::getOwners() const {
    return owners;
}

bool VMS::validateCustomerLogin(const std::string& name, const std::string& password) const {
    for (const auto& owner : owners) {
        if (owner.name == name && owner.password == password) {
            return true;
        }
    }
    return false;
}

bool VMS::validateName(const std::string& name) const {
    return isValidName(name);
}

bool VMS::validateAddress(const std::string& address) const {
    return isValidAddress(address);
}

bool VMS::validateEmail(const std::string& email) const {
    return isValidEmail(email);
}

bool VMS::validatePhone(const std::string& phone) const {
    return isValidPhone(phone);
}

bool VMS::validateDate(const std::string& date) const {
    return isValidDate(date);
}

bool VMS::validateTime(const std::string& time) const {
    return isValidTime(time);
}

bool VMS::validatePassword(const std::string& password) const {
    return isValidPassword(password);
}

std::string VMS::getValidatedStringInput(const std::string& prompt, std::function<bool(const std::string&)> validator) {
    return getValidStringInput(prompt, validator);
}

template<typename T>
T VMS::getValidatedInput(const std::string& prompt, std::function<bool(const T&)> validator) {
    return getValidInput<T>(prompt, validator);
}

void VMS::viewPetAppointmentHistory() {
    std::string ownerName = getValidatedStringInput("Enter owner's name: ",
        [this](const std::string& s) { return validateName(s); });
    std::string petName = getValidatedStringInput("Enter pet's name: ",
        [this](const std::string& s) { return validateName(s); });

    bool found = false;
    std::cout << "\nAppointment History for " << petName << ":\n";

    for (const auto& appt : appointments) {
        if (appt.pet.name == petName && appt.owner.name == ownerName) {
            found = true;
            std::cout << "Date: " << appt.date << " | Time: " << appt.time
                << " | Status: " << appt.status << std::endl;
        }
    }

    if (!found) {
        std::cout << "No appointment history found for this pet.\n";
    }
}

void VMS::viewPetMedicalHistory(const std::string& role) {
    std::string ownerName = getValidatedStringInput("Enter owner's name: ",
        [this](const std::string& s) { return validateName(s); });
    std::string petName = getValidatedStringInput("Enter pet's name: ",
        [this](const std::string& s) { return validateName(s); });

    bool found = false;
    for (const auto& owner : owners) {
        if (owner.name == ownerName) {
            for (const auto& pet : owner.pets) {
                if (pet.name == petName) {
                    found = true;

                    std::cout << "\n--- Medical History for " << pet.name << " ---\n";
                    std::cout << "Owner: " << owner.name << "\n";
                    std::cout << "Breed: " << pet.breed << "\n";
                    std::cout << "Age: " << pet.age << "\n";
                    std::cout << "Vaccination Status: " << (pet.vaccinated ? "Vaccinated" : "Not Vaccinated") << "\n";
                    std::cout << "\nMedical History:\n";

                    if (pet.medicalHistory.empty()) {
                        std::cout << "No medical history recorded.\n";
                    }
                    else {
                        std::cout << pet.medicalHistory << "\n";
                    }

                    if (role == "admin" || role == "vet") {
                        std::cout << "\nAppointment History:\n";
                        bool hasAppointments = false;
                        for (const auto& appt : appointments) {
                            if (appt.pet.name == petName && appt.owner.name == ownerName) {
                                hasAppointments = true;
                                std::cout << "Date: " << appt.date << " | Time: " << appt.time
                                    << " | Status: " << appt.status << std::endl;
                            }
                        }

                        if (!hasAppointments) {
                            std::cout << "No appointment history found for this pet.\n";
                        }

                        std::cout << "\nMedical History Management Options:\n";
                        std::cout << "1. Add new entry to medical history\n";
                        std::cout << "2. Replace entire medical history\n";
                        std::cout << "3. Return to previous menu\n";

                        int choice = getValidatedInput<int>("Enter choice (1-3): ",
                            [](int c) { return c >= 1 && c <= 3; });

                        if (choice == 3) {
                            break;
                        }

                        std::string newMedHist;

                        if (choice == 1) {
                            newMedHist = getValidatedStringInput("Enter new medical history entry: ",
                                [](const std::string&) { return true; });

                            time_t now = time(nullptr);
                            tm* ltm = localtime(&now);
                            char dateBuffer[11];
                            strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", ltm);

                            for (auto& updateOwner : owners) {
                                if (updateOwner.name == ownerName) {
                                    for (auto& updatePet : updateOwner.pets) {
                                        if (updatePet.name == petName) {
                                            if (!updatePet.medicalHistory.empty()) {
                                                updatePet.medicalHistory += "\n\n";
                                            }
                                            updatePet.medicalHistory += "[" + std::string(dateBuffer) + "] " + newMedHist;

                                            for (auto& appt : appointments) {
                                                if (appt.pet.name == petName && appt.owner.name == ownerName) {
                                                    appt.pet = updatePet;
                                                }
                                            }

                                            saveData();
                                            std::cout << "Medical history updated successfully!\n";
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        else if (choice == 2) {
                            newMedHist = getValidatedStringInput("Enter new comprehensive medical history: ",
                                [](const std::string&) { return true; });

                            for (auto& updateOwner : owners) {
                                if (updateOwner.name == ownerName) {
                                    for (auto& updatePet : updateOwner.pets) {
                                        if (updatePet.name == petName) {
                                            updatePet.medicalHistory = newMedHist;

                                            for (auto& appt : appointments) {
                                                if (appt.pet.name == petName && appt.owner.name == ownerName) {
                                                    appt.pet = updatePet;
                                                }
                                            }

                                            saveData();
                                            std::cout << "Medical history replaced successfully!\n";
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

    if (!found) {
        std::cout << "Pet not found.\n";
    }
}

void VMS::displayCustomerMenu(const std::string& customerName) {
    Owner* customer = nullptr;
    for (auto& owner : owners) {
        if (owner.name == customerName) {
            customer = &owner;
            break;
        }
    }

    if (!customer) {
        std::cout << "Customer not found!\n";
        return;
    }

    std::vector<std::string> options = {
        "View My Profile",
        "View My Pets",
        "View My Appointments",
        "Add New Pet",
        "Schedule Appointment",
        "View Pet Appointment History"
    };
    int maxFunctionalOption = options.size();

    while (true) {
        std::cout << "\n--- Customer Menu (" << customerName << ") ---\n";
        for (size_t i = 0; i < options.size(); i++) {
            std::cout << (i + 1) << ". " << options[i] << "\n";
        }
        int exitOption = options.size() + 1;
        std::cout << exitOption << ". Back to Main Menu\n";

        int choice = getValidatedInput<int>("Enter choice: ",
            [maxFunctionalOption, exitOption](int c) {
                return (c >= 1 && c <= maxFunctionalOption) || c == exitOption;
            });

        if (choice == exitOption) return;

        switch (choice) {
        case 1: {
            std::cout << "\nYour Profile:\n";
            std::cout << "Name: " << customer->name << "\n";
            std::cout << "Age: " << customer->age << "\n";
            std::cout << "Address: " << customer->address << "\n";
            std::cout << "Phone: " << customer->phone << "\n";
            std::cout << "Email: " << customer->email << "\n";
            break;
        }
        case 2: {
            if (customer->pets.empty()) {
                std::cout << "You have no pets registered.\n";
            }
            else {
                std::cout << "\nYour Pets:\n";
                for (const auto& pet : customer->pets) {
                    std::cout << "- " << pet.name << " (" << pet.breed << "), Age: " << pet.age
                        << ", Vaccinated: " << (pet.vaccinated ? "Yes" : "No") << "\n";
                }
            }
            break;
        }
        case 3: {
            bool found = false;
            std::cout << "\nYour Appointments:\n";
            for (const auto& appt : appointments) {
                if (appt.owner.name == customerName) {
                    found = true;
                    std::cout << "Date: " << appt.date << " | Time: " << appt.time
                        << " | Pet: " << appt.pet.name << " | Status: " << appt.status << std::endl;
                }
            }
            if (!found) std::cout << "No appointments found.\n";
            break;
        }
        case 4: {
            std::string name = getValidatedStringInput("Enter pet name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string breed = getValidatedStringInput("Enter pet breed: ",
                [this](const std::string& s) { return validateName(s); });
            int age = getValidatedInput<int>("Enter pet age: ",
                [](int a) { return a > 0 && a < 30; });
            std::string medHist = getValidatedStringInput("Enter medical history: ",
                [](const std::string&) { return true; });
            bool vaccinated = getValidatedInput<bool>("Is pet vaccinated? (1 for Yes, 0 for No): ",
                [](bool) { return true; });

            if (!isVaccinationStatusAppropriate(age, vaccinated)) {
                std::cout << "Warning: Vaccination status may not be appropriate for this pet's age.\n";
                std::string confirm = getValidatedStringInput("Continue anyway? (y/n): ",
                    [](const std::string& s) { return s == "y" || s == "n"; });
                if (confirm == "n") break;
            }

            customer->addPet(Pet(name, breed, age, medHist, vaccinated));
            saveData();
            std::cout << "Pet added successfully!\n";
            break;
        }
        case 5: {
            if (customer->pets.empty()) {
                std::cout << "You need to add a pet first!\n";
                break;
            }

            std::cout << "Select a pet:\n";
            for (size_t i = 0; i < customer->pets.size(); i++) {
                std::cout << i + 1 << ". " << customer->pets[i].name << "\n";
            }
            int petChoice = getValidatedInput<int>("Enter pet number: ",
                [customer](int c) { return c > 0 && c <= static_cast<int>(customer->pets.size()); });

            std::string date = getValidatedStringInput("Enter date (YYYY-MM-DD): ",
                [this](const std::string& s) { return validateDate(s); });
            std::string time = getValidatedStringInput("Enter time (HH:MM): ",
                [this](const std::string& s) { return validateTime(s); });

            if (!isDateTimeInFuture(date, time)) {
                std::cout << "Error: Cannot schedule appointments in the past.\n";
                break;
            }

            if (hasTimeConflict(date, time)) {
                std::cout << "Error: There is already an appointment at this time.\n";
                break;
            }

            if (isDuplicateAppointment(customer->name, customer->pets[petChoice - 1].name, date, time)) {
                std::cout << "Error: This pet already has an appointment at this time.\n";
                break;
            }

            appointments.push_back(Appointment(date, time, customer->pets[petChoice - 1], *customer, "Scheduled"));
            saveData();
            std::cout << "Appointment scheduled successfully!\n";
            break;
        }
        case 6: {
            if (customer->pets.empty()) {
                std::cout << "You have no pets registered.\n";
                break;
            }

            std::cout << "Select a pet to view appointment history:\n";
            for (size_t i = 0; i < customer->pets.size(); i++) {
                std::cout << i + 1 << ". " << customer->pets[i].name << "\n";
            }
            int petChoice = getValidatedInput<int>("Enter pet number: ",
                [customer](int c) { return c > 0 && c <= static_cast<int>(customer->pets.size()); });

            std::string petName = customer->pets[petChoice - 1].name;
            bool found = false;
            std::cout << "\nAppointment History for " << petName << ":\n";

            for (const auto& appt : appointments) {
                if (appt.pet.name == petName && appt.owner.name == customer->name) {
                    found = true;
                    std::cout << "Date: " << appt.date << " | Time: " << appt.time
                        << " | Status: " << appt.status << std::endl;
                }
            }

            if (!found) {
                std::cout << "No appointment history found for this pet.\n";
            }
            break;
        }
        }
    }
}

void VMS::displayPetsMenu(const std::string& role) {
    std::vector<std::string> options = { "View All Pets", "View Pet Medical History" };
    int maxFunctionalOption = 2;

    if (role == "admin" || role == "staff") {
        options.push_back("Update Pet");
        maxFunctionalOption = 3;
    }
    if (role == "admin") {
        options.push_back("Delete Pet");
        maxFunctionalOption = 4;
    }

    while (true) {
        int choice = displayRoleMenu("Pets Menu", options, maxFunctionalOption);
        if (choice == -1) return;

        switch (choice) {
        case 1: {
            if (owners.empty()) {
                std::cout << "No owners found.\n";
            }
            else {
                for (const auto& owner : owners) {
                    if (!owner.pets.empty()) {
                        std::cout << "\nOwner: " << owner.name << "\n";
                        for (const auto& pet : owner.pets) {
                            std::cout << "- " << pet.name << " (" << pet.breed << "), Age: " << pet.age
                                << ", Vaccinated: " << (pet.vaccinated ? "Yes" : "No") << "\n";
                        }
                    }
                }
            }
            break;
        }
        case 2: {
            viewPetMedicalHistory(role);
            break;
        }
        case 3: {
            if (role != "admin" && role != "staff") {
                std::cout << "Access denied.\n";
                break;
            }
            std::string ownerName = getValidatedStringInput("Enter owner's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string petName = getValidatedStringInput("Enter pet's name: ",
                [this](const std::string& s) { return validateName(s); });
            bool found = false;
            for (auto& owner : owners) {
                if (owner.name == ownerName) {
                    for (auto& pet : owner.pets) {
                        if (pet.name == petName) {
                            found = true;
                            std::cout << "Current pet details:\n";
                            std::cout << "Name: " << pet.name << "\nBreed: " << pet.breed << "\nAge: " << pet.age
                                << "\nMedical History: " << pet.medicalHistory << "\nVaccinated: "
                                << (pet.vaccinated ? "Yes" : "No") << "\n";

                            std::string newMedHist = getValidatedStringInput("Enter new medical history: ",
                                [](const std::string&) { return true; });
                            bool newVaccinated = getValidatedInput<bool>("Update vaccination status? (1 for Yes, 0 for No): ",
                                [](bool) { return true; });

                            pet.medicalHistory = newMedHist;
                            pet.vaccinated = newVaccinated;

                            for (auto& appt : appointments) {
                                if (appt.pet.name == petName && appt.owner.name == ownerName) {
                                    appt.pet = pet;
                                }
                            }

                            saveData();
                            std::cout << "Pet updated successfully!\n";
                            break;
                        }
                    }
                }
            }
            if (!found) std::cout << "Pet not found.\n";
            break;
        }
        case 4: {
            if (role != "admin") {
                std::cout << "Access denied. Admin only.\n";
                break;
            }
            std::string ownerName = getValidatedStringInput("Enter owner's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string petName = getValidatedStringInput("Enter pet's name: ",
                [this](const std::string& s) { return validateName(s); });
            bool found = false;
            for (auto itOwner = owners.begin(); itOwner != owners.end(); ++itOwner) {
                if (itOwner->name == ownerName) {
                    for (auto itPet = itOwner->pets.begin(); itPet != itOwner->pets.end(); ++itPet) {
                        if (itPet->name == petName) {
                            found = true;
                            itOwner->pets.erase(itPet);

                            std::vector<Appointment> updatedAppointments;
                            for (const auto& appt : appointments) {
                                if (!(appt.pet.name == petName && appt.owner.name == ownerName)) {
                                    updatedAppointments.push_back(appt);
                                }
                            }
                            appointments = updatedAppointments;

                            saveData();
                            std::cout << "Pet deleted successfully!\n";
                            break;
                        }
                    }
                }
            }
            if (!found) std::cout << "Pet not found.\n";
            break;
        }
        }
    }
}

void VMS::displayAppointmentMenu(const std::string& role) {
    std::vector<std::string> options = { "View All Appointments" };
    int maxFunctionalOption = 1;
    if (role == "admin" || role == "staff") {
        options.push_back("Schedule Appointment");
        options.push_back("Update Appointment");
        maxFunctionalOption = 3;
    }
    if (role == "admin" || role == "vet") {
        options.push_back("Cancel Appointment");
        maxFunctionalOption = 4;
    }

    while (true) {
        int choice = displayRoleMenu("Appointments Menu", options, maxFunctionalOption);
        if (choice == -1) return;

        switch (choice) {
        case 1: {
            if (appointments.empty()) {
                std::cout << "No appointments found.\n";
            }
            else {
                for (const auto& appt : appointments) {
                    std::cout << "Date: " << appt.date << " | Time: " << appt.time
                        << " | Pet: " << appt.pet.name << " | Owner: " << appt.owner.name
                        << " | Status: " << appt.status << "\n";
                }
            }
            break;
        }
        case 2: {
            if (role != "admin" && role != "staff") {
                std::cout << "Access denied.\n";
                break;
            }
            std::string ownerName = getValidatedStringInput("Enter owner's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string petName = getValidatedStringInput("Enter pet's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string date = getValidatedStringInput("Enter date (YYYY-MM-DD): ",
                [this](const std::string& s) { return validateDate(s); });
            std::string time = getValidatedStringInput("Enter time (HH:MM): ",
                [this](const std::string& s) { return validateTime(s); });

            if (!isDateTimeInFuture(date, time)) {
                std::cout << "Error: Cannot schedule appointments in the past.\n";
                break;
            }

            if (hasTimeConflict(date, time)) {
                std::cout << "Error: There is already an appointment at this time.\n";
                break;
            }

            if (isDuplicateAppointment(ownerName, petName, date, time)) {
                std::cout << "Error: This pet already has an appointment at this time.\n";
                break;
            }

            bool found = false;
            for (const auto& owner : owners) {
                if (owner.name == ownerName) {
                    for (const auto& pet : owner.pets) {
                        if (pet.name == petName) {
                            found = true;
                            appointments.push_back(Appointment(date, time, pet, owner, "Scheduled"));
                            saveData();
                            std::cout << "Appointment scheduled successfully!\n";
                            break;
                        }
                    }
                }
            }
            if (!found) std::cout << "Owner/pet not found.\n";
            break;
        }
        case 3: {
            if (role != "admin" && role != "staff") {
                std::cout << "Access denied.\n";
                break;
            }
            std::string ownerName = getValidatedStringInput("Enter owner's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string petName = getValidatedStringInput("Enter pet's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string date = getValidatedStringInput("Enter appointment date: ",
                [this](const std::string& s) { return validateDate(s); });
            std::string time = getValidatedStringInput("Enter appointment time: ",
                [this](const std::string& s) { return validateTime(s); });

            bool found = false;
            for (auto& appt : appointments) {
                if (appt.owner.name == ownerName && appt.pet.name == petName &&
                    appt.date == date && appt.time == time) {
                    found = true;

                    std::string newStatus = getValidatedStringInput("Enter new status (Scheduled/Completed/Cancelled): ",
                        [&appt, this](const std::string& s) {
                            if (s == "Scheduled" && appt.isInPast()) {
                                std::cout << "Cannot set a past appointment to Scheduled status.\n";
                                return false;
                            }
                            if (!isValidStatusTransition(appt.status, s)) {
                                std::cout << "Invalid status transition from " << appt.status << " to " << s << ".\n";
                                return false;
                            }
                            return s == "Scheduled" || s == "Completed" || s == "Cancelled";
                        });

                    appt.status = newStatus;
                    saveData();
                    std::cout << "Appointment updated successfully!\n";
                    break;
                }
            }
            if (!found) std::cout << "Appointment not found.\n";
            break;
        }
        case 4: {
            if (role != "admin" && role != "vet") {
                std::cout << "Access denied. Admin/vet only.\n";
                break;
            }
            std::string ownerName = getValidatedStringInput("Enter owner's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string petName = getValidatedStringInput("Enter pet's name: ",
                [this](const std::string& s) { return validateName(s); });
            std::string date = getValidatedStringInput("Enter appointment date: ",
                [this](const std::string& s) { return validateDate(s); });
            std::string time = getValidatedStringInput("Enter appointment time: ",
                [this](const std::string& s) { return validateTime(s); });

            bool found = false;
            for (auto it = appointments.begin(); it != appointments.end(); ++it) {
                if (it->owner.name == ownerName && it->pet.name == petName &&
                    it->date == date && it->time == time) {
                    found = true;

                    it->status = "Cancelled";
                    saveData();
                    std::cout << "Appointment cancelled successfully!\n";
                    break;
                }
            }
            if (!found) std::cout << "Appointment not found.\n";
            break;
        }
        }
    }
}

void VMS::displayOwnersMenu(const std::string& role) {
    std::vector<std::string> options = { "View All Owners" };
    int maxFunctionalOption = 1;

    if (role == "admin" || role == "staff") {
        options.push_back("Add New Owner");
        options.push_back("Update Owner");
        maxFunctionalOption = 3;
    }
    if (role == "admin") {
        options.push_back("Delete Owner");
        maxFunctionalOption = 4;
    }

    while (true) {
        int choice = displayRoleMenu("Owners Menu", options, maxFunctionalOption);
        if (choice == -1) return;

        switch (choice) {
        case 1: {
            if (owners.empty()) {
                std::cout << "No owners found.\n";
            }
            else {
                for (const auto& owner : owners) {
                    std::cout << "\nName: " << owner.name << "\nAge: " << owner.age
                        << "\nAddress: " << owner.address << "\nPhone: " << owner.phone
                        << "\nEmail: " << owner.email << "\n";
                }
            }
            break;
        }
        case 2: {
            if (role != "admin" && role != "staff") {
                std::cout << "Access denied.\n";
                break;
            }
            std::string name = getValidatedStringInput("Enter owner name: ",
                [this](const std::string& s) { return validateName(s); });

            bool nameExists = false;
            for (const auto& owner : owners) {
                if (owner.name == name) {
                    nameExists = true;
                    break;
                }
            }
            if (nameExists) {
                std::cout << "An owner with this name already exists. Please use a different name.\n";
                break;
            }

            int age = getValidatedInput<int>("Enter owner age: ",
                [](int a) { return a >= 18 && a <= 120; });
            std::string address = getValidatedStringInput("Enter address: ",
                [this](const std::string& s) { return validateAddress(s); });
            std::string phone = getValidatedStringInput("Enter phone (11 digits): ",
                [this](const std::string& s) { return validatePhone(s); });
            std::string email = getValidatedStringInput("Enter email: ",
                [this](const std::string& s) { return validateEmail(s); });
            std::string password = getValidatedStringInput("Enter password (min 6 characters): ",
                [this](const std::string& s) { return validatePassword(s); });

            owners.push_back(Owner(name, age, address, phone, email, security::simpleEncrypt(password)));
            saveData();
            std::cout << "Owner added successfully!\n";
            break;
        }
        case 3: {
            if (role != "admin" && role != "staff") {
                std::cout << "Access denied.\n";
                break;
            }
            std::string name = getValidatedStringInput("Enter owner name to update: ",
                [this](const std::string& s) { return validateName(s); });
            bool found = false;
            for (auto& owner : owners) {
                if (owner.name == name) {
                    found = true;
                    std::cout << "Current details:\n";
                    std::cout << "Name: " << owner.name << "\nAge: " << owner.age
                        << "\nAddress: " << owner.address << "\nPhone: " << owner.phone
                        << "\nEmail: " << owner.email << "\n";

                    std::string newAddress = getValidatedStringInput("Enter new address: ",
                        [this](const std::string& s) { return validateAddress(s); });
                    std::string newPhone = getValidatedStringInput("Enter new phone: ",
                        [this](const std::string& s) { return validatePhone(s); });
                    std::string newEmail = getValidatedStringInput("Enter new email: ",
                        [this](const std::string& s) { return validateEmail(s); });

                    owner.address = newAddress;
                    owner.phone = newPhone;
                    owner.email = newEmail;

                    for (auto& appt : appointments) {
                        if (appt.owner.name == name) {
                            appt.owner = owner;
                        }
                    }

                    saveData();
                    std::cout << "Owner updated successfully!\n";
                    break;
                }
            }
            if (!found) std::cout << "Owner not found.\n";
            break;
        }
        case 4: {
            if (role != "admin") {
                std::cout << "Access denied. Admin only.\n";
                break;
            }
            std::string name = getValidatedStringInput("Enter owner name to delete: ",
                [this](const std::string& s) { return validateName(s); });
            bool found = false;
            for (auto it = owners.begin(); it != owners.end(); ++it) {
                if (it->name == name) {
                    found = true;

                    // Remove all appointments for this owner
                    std::vector<Appointment> updatedAppointments;
                    for (const auto& appt : appointments) {
                        if (appt.owner.name != name) {
                            updatedAppointments.push_back(appt);
                        }
                    }
                    appointments = updatedAppointments;

                    owners.erase(it);
                    saveData();
                    std::cout << "Owner deleted successfully!\n";
                    break;
                }
            }
            if (!found) std::cout << "Owner not found.\n";
            break;
        }
        }
    }
}

void VMS::displayMenu(const std::string& role) {
    updateAllAppointmentStatuses(); // Update appointment statuses first

    std::vector<std::string> options;

    if (role == "admin") {
        options = { "View Profile", "Pets Menu", "Appointments Menu", "Owners Menu" };
    }
    else if (role == "vet") {
        options = { "View Profile", "Pets Menu", "Appointments Menu" };
    }
    else if (role == "staff") {
        options = { "View Profile", "Pets Menu", "Appointments Menu", "Owners Menu" };
    }
    else {
        // Customer menu
        displayCustomerMenu(role);
        return;
    }

    while (true) {
        int choice = displayRoleMenu("Main Menu", options, options.size());
        if (choice == -1) {
            return; // Exit to login screen
        }

        switch (choice) {
        case 1:
            // View Profile
            std::cout << "\nCurrent Role: " <<
                (role == "admin" ? "Administrator" :
                    role == "vet" ? "Veterinarian" :
                    role == "staff" ? "Staff Member" : "Customer") << "\n";
            break;
        case 2:
            displayPetsMenu(role);
            break;
        case 3:
            displayAppointmentMenu(role);
            break;
        case 4:
            if (role == "admin" || role == "staff") {
                displayOwnersMenu(role);
            }
            break;
        }
    }
}

void VMS::saveData() {
    try {
        updateAllAppointmentStatuses(); // Update statuses before saving

        // Save owners
        std::ofstream ownerFile("owners.csv");
        if (!ownerFile.is_open()) {
            throw FileWriteException("owners.csv");
        }
        for (const auto& owner : owners) {
            ownerFile << owner.toCSV() << "\n";
        }
        ownerFile.close();

        // Save pets
        std::ofstream petFile("pets.csv");
        if (!petFile.is_open()) {
            throw FileWriteException("pets.csv");
        }
        for (const auto& owner : owners) {
            for (const auto& pet : owner.pets) {
                petFile << csv_utils::escapeCSV(owner.name) << "," << pet.toCSV() << "\n";
            }
        }
        petFile.close();

        // Save appointments
        std::ofstream apptFile("appointments.csv");
        if (!apptFile.is_open()) {
            throw FileWriteException("appointments.csv");
        }
        for (const auto& appt : appointments) {
            apptFile << appt.toCSV() << "\n";
        }
        apptFile.close();

    }
    catch (const FileWriteException& e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error saving data: " << e.what() << std::endl;
    }
}

void VMS::loadData() {
    try {
        // Load owners
        std::ifstream ownerFile("owners.csv");
        if (ownerFile.is_open()) {
            std::string line;
            while (getline(ownerFile, line)) {
                if (!line.empty()) {
                    owners.push_back(Owner::fromCSV(line));
                }
            }
            ownerFile.close();
        }

        // Load pets
        std::ifstream petFile("pets.csv");
        if (petFile.is_open()) {
            std::string line;
            while (getline(petFile, line)) {
                if (!line.empty()) {
                    size_t commaPos = line.find(',');
                    std::string ownerName = csv_utils::unescapeCSV(line.substr(0, commaPos));
                    std::string petData = line.substr(commaPos + 1);

                    for (auto& owner : owners) {
                        if (owner.name == ownerName) {
                            owner.addPet(Pet::fromCSV(petData));
                            break;
                        }
                    }
                }
            }
            petFile.close();
        }

        // Load appointments
        std::ifstream apptFile("appointments.csv");
        if (apptFile.is_open()) {
            std::string line;
            while (getline(apptFile, line)) {
                if (!line.empty()) {
                    appointments.push_back(Appointment::fromCSV(line, owners));
                }
            }
            apptFile.close();
        }

        updateAllAppointmentStatuses(); // Update statuses after loading

    }
    catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
    }
}

// Explicit template instantiations
template int VMS::getValidInput<int>(const std::string&, std::function<bool(const int&)>);
template bool VMS::getValidInput<bool>(const std::string&, std::function<bool(const bool&)>);
template int VMS::getValidatedInput<int>(const std::string&, std::function<bool(const int&)>);
template bool VMS::getValidatedInput<bool>(const std::string&, std::function<bool(const bool&)>);