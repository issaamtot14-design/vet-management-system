#pragma once
#include <string>

class Pet {
public:
    std::string name, breed, medicalHistory;
    int age;
    bool vaccinated;

    Pet(std::string n, std::string b, int a, std::string mh, bool v);
    std::string toCSV() const;
    static Pet fromCSV(const std::string& line);
};