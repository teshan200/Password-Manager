# 🔐 Console-Based Password Vault System

## 📚 Course Information
Course: Structured Programming II (In24-S2-IN1130)  
Language: C  
Type: Console-Based Application  

---

## 📌 Project Overview

This project is a console-based Password Management System developed using structured programming concepts in C.

The system allows users to securely manage website account credentials including:

- Website name  
- Username  
- Password (stored with basic XOR protection)

The application uses structures, arrays of structures, string functions, file handling, loops, and conditional statements as required by the course guidelines.

---

## 🏗 System Design

### 1️⃣ Structures Used

#### Account Structure
Stores individual account details:
- website
- username
- password (encrypted)

#### Vault Structure
- Array of Account (maximum 20 records)
- totalAccounts counter

The Vault structure manages all account records in memory using an array of structures.

---

## 💾 Data Storage

- Data is stored in a text file named: `vault.txt`
- Format used in file:

website|username|encryptedPassword

Example stored line:

gmail.com|teshan|Q@!d$%x

Passwords are encrypted before saving to prevent plain-text storage.

---

## 🔐 Password Protection (XOR Method)

The system uses a simple XOR-based protection method.

Encryption logic:

    encryptedChar = originalChar ^ '#'

The same function is used for both encryption and decryption.

This works because XOR has the mathematical property:

    A ^ B ^ B = A

Meaning:
If you apply XOR twice with the same key, the original value is restored.

⚠ Note:  
This is a basic protection mechanism for academic purposes only.  
It is not strong cryptographic security.

---

## 📋 Functional Requirements Implemented

### 🔹 Data Management (5 Activities)

1. Add new account  
2. Display all accounts  
3. Search account by website  
4. Modify account  
5. Delete account  

---

### 🔹 Logical / Processing Activities (5 Activities)

6. Count total saved accounts  
7. Find account with longest password  
8. Find account with shortest password  
9. Count accounts containing a substring in website name  
10. Check weak passwords (length < 6 characters)  

---

## 🖥 Program Features

- Menu-driven interface
- Input validation using fgets
- File handling (automatic load and save)
- Array shifting for delete operation
- String manipulation using:
  - strcmp
  - strcpy
  - strlen
  - strcat
  - strstr
  - strtok
- Bitwise operation (XOR) for password protection

---

## 🧠 Concepts Demonstrated

- Structures
- Array of structures
- String handling
- File handling
- Bitwise operators
- Loops and conditionals
- Menu-driven programming
- Data validation

---

## 📊 Sample Menu Options

1. Add new account  
2. Display all accounts  
3. Search account by website  
4. Modify account  
5. Delete account  
6. Count total saved accounts  
7. Find account with longest password  
8. Find account with shortest password  
9. Count websites with substring  
10. Check weak passwords  
0. Exit  

---

## 📁 File Structure

main.c  
vault.txt  
README.md  

---

## 👨‍💻 Author

Developed as part of Structured Programming II coursework By Teshan Pamodya