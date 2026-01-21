#include "appointment.h"
#include "csv_utils.h"
#include <sstream>
#include <ctime>
#include <iomanip>

Appointment::Appointment(std::string d, std::string t, Pet p, Owner o, std::string s)
    : date(d), time(t), pet(p), owner(o), status(s) {
}

std::string Appointment::toCSV() const {
    return csv_utils::escapeCSV(date) + "," +
        csv_utils::escapeCSV(time) + "," +
        csv_utils::escapeCSV(pet.name) + "," +
        csv_utils::escapeCSV(owner.name) + "," +
        csv_utils::escapeCSV(status);
}

Appointment Appointment::fromCSV(const std::string& line, const std::vector<Owner>& allOwners) {
    std::stringstream ss(line);
    std::string date, time, petName, ownerName, status;

    getline(ss, date, ',');
    getline(ss, time, ',');
    getline(ss, petName, ',');
    getline(ss, ownerName, ',');
    getline(ss, status, ',');

    date = csv_utils::unescapeCSV(date);
    time = csv_utils::unescapeCSV(time);
    petName = csv_utils::unescapeCSV(petName);
    ownerName = csv_utils::unescapeCSV(ownerName);
    status = csv_utils::unescapeCSV(status);

    Pet foundPet("", "", 0, "", false);
    Owner foundOwner("", 0, "", "", "");

    for (const auto& owner : allOwners) {
        if (owner.name == ownerName) {
            foundOwner = owner;
            for (const auto& pet : owner.pets) {
                if (pet.name == petName) {
                    foundPet = pet;
                    break;
                }
            }
            break;
        }
    }

    return Appointment(date, time, foundPet, foundOwner, status);
}

bool Appointment::isInPast() const {
    tm apptTime = {};
    std::stringstream datetime(date + " " + time);
    datetime >> std::get_time(&apptTime, "%Y-%m-%d %H:%M");
    if (datetime.fail()) return false;

    time_t apptTimestamp = mktime(&apptTime);
    time_t now = std::time(nullptr);
    return apptTimestamp < now;
}

void Appointment::updateStatus() {
    if (status == "Scheduled" && isInPast()) {
        status = "Completed";
    }
}