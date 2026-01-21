#pragma once
#include <exception>
#include <string>

class InvalidRoleException : public std::exception {
public:
    const char* what() const throw() {
        return "Error: Invalid role entered.";
    }
};

class LoginFailedException : public std::exception {
public:
    const char* what() const throw() {
        return "Error: Invalid credentials. Login failed.";
    }
};

class FileAccessException : public std::exception {
private:
    std::string message;
public:
    FileAccessException(const std::string& filename) {
        message = "Error: Could not access file " + filename;
    }

    const char* what() const throw() {
        return message.c_str();
    }
};

class FileWriteException : public std::exception {
private:
    std::string message;
public:
    FileWriteException(const std::string& filename) {
        message = "Error: Could not write to file " + filename;
    }

    const char* what() const throw() {
        return message.c_str();
    }
};