#pragma once
#include <string>
#include <vector>
#include "pet.h"
#include "owner.h"

class Appointment {
public:
    std::string date, time, status;
    Pet pet;
    Owner owner;

    Appointment(std::string d, std::string t, Pet p, Owner o, std::string s);
    std::string toCSV() const;
    static Appointment fromCSV(const std::string& line, const std::vector<Owner>& allOwners);
    bool isInPast() const;
    void updateStatus();
};