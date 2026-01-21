#pragma once
#include <vector>
#include <string>
#include <functional>
#include "owner.h"
#include "pet.h"
#include "appointment.h"

class VMS {
private:
    std::vector<Owner> owners;
    std::vector<Appointment> appointments;

    template<typename T>
    T getValidInput(const std::string& prompt, std::function<bool(const T&)> validator);

    std::string getValidStringInput(const std::string& prompt, std::function<bool(const std::string&)> validator);

    bool isValidName(const std::string& name) const;
    bool isValidAddress(const std::string& address) const;
    bool isValidEmail(const std::string& email) const;
    bool isValidPhone(const std::string& phone) const;
    bool isValidDate(const std::string& date) const;
    bool isValidTime(const std::string& time) const;
    bool isValidPassword(const std::string& password) const;

    void updateAllAppointmentStatuses();
    int displayRoleMenu(const std::string& title, const std::vector<std::string>& options, int maxOptions);
    bool isDateTimeInFuture(const std::string& date, const std::string& time) const;
    bool hasTimeConflict(const std::string& date, const std::string& time) const;
    bool isDuplicateAppointment(const std::string& ownerName, const std::string& petName,
        const std::string& date, const std::string& time) const;
    bool isValidStatusTransition(const std::string& currentStatus, const std::string& newStatus);
    bool isVaccinationStatusAppropriate(int petAge, bool isVaccinated);

public:
    void addOwner(const Owner& owner);
    const std::vector<Owner>& getOwners() const;

    bool validateCustomerLogin(const std::string& name, const std::string& password) const;

    bool validateName(const std::string& name) const;
    bool validateAddress(const std::string& address) const;
    bool validateEmail(const std::string& email) const;
    bool validatePhone(const std::string& phone) const;
    bool validateDate(const std::string& date) const;
    bool validateTime(const std::string& time) const;
    bool validatePassword(const std::string& password) const;

    std::string getValidatedStringInput(const std::string& prompt, std::function<bool(const std::string&)> validator);

    template<typename T>
    T getValidatedInput(const std::string& prompt, std::function<bool(const T&)> validator);

    void viewPetAppointmentHistory();
    void viewPetMedicalHistory(const std::string& role);

    void displayCustomerMenu(const std::string& customerName);
    void displayPetsMenu(const std::string& role);
    void displayAppointmentMenu(const std::string& role);
    void displayOwnersMenu(const std::string& role);
    void displayMenu(const std::string& role);

    void saveData();
    void loadData();
};