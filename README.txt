===========================================================================
|		2025_04_11_16_00_00					  |
|		University Of Roehampton				  |
|		Student 001: Issa Amtot/A0015038			  |
|		Student 002: Ali Ganji/GAN23599848                        |
===========================================================================
|                     Veterinary Management System                        |
===========================================================================
A C++ console application designed for managing veterinary clinic operations
including patients, appointments, and customer records.
===========================================================================
Overview

The Veterinary Management System (VMS) is a solution for veterinary clinics 
to efficiently manage client information, pet records, and appointment 
scheduling. With role-based access control, the system offers different 
capabilities to administrators, veterinarians, staff members, and customers.
===========================================================================
Features

Multi-User Role-Based System
Admin: Full access to all system features
Veterinarian: Access to medical records and appointment management
Staff: Customer and appointment management
Customer: Self-service portal for pet and appointment management
Pet Management
Register and update pet information
Track vaccination status
Manage medical histories with timestamped entries
View detailed pet information
Appointment Scheduling
Schedule new appointments with conflict detection
Update appointment status (Scheduled/Completed/Cancelled)
Automatic status updates for past appointments
View appointment history by pet
Customer Management
Register new customers with contact information
Self-service customer portal
Update customer details
Secure password-based authentication
Data Persistence
CSV-based data storage
Automatic data saving
Graceful error handling for file operations
==========================================================================
File Structure

The system uses CSV files for data storage:

owners.csv - Customer information
pets.csv - Pet records linked to owners
appointments.csv - Appointment information
admin.txt, vet.txt, staff.txt - Role-based password files
==========================================================================
Usage

Login Access
The system provides four types of user roles:

Administrator
Default credentials: username: admin, password: admin123
Full access to all system features

Veterinarian
Default credentials: username: vet, password: vet456
Focus on medical records and appointment management

Staff Member
Default credentials: username: staff, password: staff789
Customer and appointment management

Customer
Self-registration or login with personal credentials
Access to personal and pet information
==========================================================================
Main Features
For Admin/Staff:
Add, update, and manage customer records
Add and update pet information
Schedule and manage appointments

For Veterinarians:
View and update pet medical histories
Manage appointment statuses
Access complete medical records

For Customers:
View personal profile
Register and view pet information
Schedule and view appointments
Data Validation
The system implements comprehensive data validation including:

Format validation for emails, phone numbers, and dates
Conflict detection for appointments
Appropriate status transitions
Password security
Exception Handling
Custom exception classes for robust error handling:

InvalidRoleException
LoginFailedException
FileAccessException
FileWriteException
Implementation Details
==========================================================================
Classes

Pet: Manages pet information including medical history and vaccination 
status
Owner: Manages customer information and associated pets
Appointment: Handles appointment scheduling with status tracking
VMS: Core system class that coordinates all operations
Data Security
Simple password encryption for demonstration purposes
CSV data escaping to handle special characters
CSV Handling
Custom CSV parsing and generation
Proper escaping and unescaping of fields with special characters
Relational data integrity across files
==========================================================================
This project is available for educational purposes and can be modified 
and distributed freely.

Acknowledgments
Developed as an educational project for demonstrating C++ OOP principles
