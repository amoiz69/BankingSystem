#include <iostream>
#include <string>

using namespace std;

class Account {
protected:
    string name;
    string fatherName;
    string cnic;
    string dob;
    double balance;

public:
    Account(const string& n, const string& fn, const string& c, const string& d, double initialBalance) : name(n), fatherName(fn), cnic(c), dob(d), balance(initialBalance) {}

    virtual void displayMessage() = 0;

    void displayDetails() {
        cout << "Account Details:\n";
        cout << "Name: " << name << endl;
        cout << "Father's Name: " << fatherName << endl;
        cout << "CNIC: " << cnic << endl;
        cout << "Date of Birth: " << dob << endl;
        cout << "Balance: $" << balance << endl;
    }
};

class CurrentAccount : public Account {
public:
    CurrentAccount(const string& n, const string& fn, const string& c, const string& d, double initialBalance) : Account(n, fn, c, d, initialBalance) {}

    void displayMessage() override {
        cout << "Current account saved successfully with an initial balance of $" << balance << ".\n";
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(const string& n, const string& fn, const string& c, const string& d, double initialBalance) : Account(n, fn, c, d, initialBalance) {}

    void displayMessage() override {
        cout << "Savings account saved successfully with an initial balance of $" << balance << ".\n";
    }
};
