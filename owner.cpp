#include "owner.h"
#include "csv_utils.h"
#include <sstream>

Owner::Owner(std::string n, int a, std::string addr, std::string ph, std::string em, std::string pw)
    : name(n), age(a), address(addr), phone(ph), email(em), password(pw) {
}

void Owner::addPet(const Pet& pet) {
    pets.push_back(pet);
}

std::string Owner::toCSV() const {
    return csv_utils::escapeCSV(name) + "," +
        std::to_string(age) + "," +
        csv_utils::escapeCSV(address) + "," +
        csv_utils::escapeCSV(phone) + "," +
        csv_utils::escapeCSV(email) + "," +
        csv_utils::escapeCSV(password);
}

Owner Owner::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string name, ageStr, address, phone, email, password;

    getline(ss, name, ',');
    getline(ss, ageStr, ',');
    getline(ss, address, ',');
    getline(ss, phone, ',');
    getline(ss, email, ',');
    getline(ss, password, ',');

    return Owner(
        csv_utils::unescapeCSV(name),
        std::stoi(ageStr),
        csv_utils::unescapeCSV(address),
        csv_utils::unescapeCSV(phone),
        csv_utils::unescapeCSV(email),
        csv_utils::unescapeCSV(password)
    );
}