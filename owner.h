#pragma once
#include <string>
#include <vector>
#include "pet.h"

class Owner {
public:
    std::string name, address, phone, email, password;
    int age;
    std::vector<Pet> pets;

    Owner(std::string n, int a, std::string addr, std::string ph, std::string em, std::string pw = "");
    void addPet(const Pet& pet);
    std::string toCSV() const;
    static Owner fromCSV(const std::string& line);
};