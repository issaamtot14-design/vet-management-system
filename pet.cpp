#include "pet.h"
#include "csv_utils.h"
#include <sstream>

Pet::Pet(std::string n, std::string b, int a, std::string mh, bool v)
    : name(n), breed(b), age(a), medicalHistory(mh), vaccinated(v) {
}

std::string Pet::toCSV() const {
    return csv_utils::escapeCSV(name) + "," +
        csv_utils::escapeCSV(breed) + "," +
        std::to_string(age) + "," +
        csv_utils::escapeCSV(medicalHistory) + "," +
        (vaccinated ? "Yes" : "No");
}

Pet Pet::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string name, breed, ageStr, medHist, vacc;

    getline(ss, name, ',');
    getline(ss, breed, ',');
    getline(ss, ageStr, ',');
    getline(ss, medHist, ',');
    getline(ss, vacc, ',');

    return Pet(
        csv_utils::unescapeCSV(name),
        csv_utils::unescapeCSV(breed),
        std::stoi(ageStr),
        csv_utils::unescapeCSV(medHist),
        vacc == "Yes"
    );
}