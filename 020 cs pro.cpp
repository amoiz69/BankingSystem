#include <iostream>
#include <string>

using namespace std;
// base class
class Account {
protected:
	
    string name;
    string fatherName;
    string cnic;
    string dob;
    double balance;

public:
	  // constructor
	  // declaring constants to use protected members in public
    Account(const string& n, const string& fn, const string& c, const string& d, double initialBalance) : name(n), fatherName(fn), cnic(c), dob(d), balance(initialBalance) {}

    void displayMessage() {
    	cout<<endl;
        cout << "Account saved successfully with an initial balance of $" << balance << "." << endl;
    }
     // displaying the input values
    void displayDetails() {
        cout << "Account Details:" << endl;
        cout << "Name: " << name << endl;
        cout << "Father's Name: " << fatherName << endl;
        cout << "CNIC: " << cnic << endl;
        cout << "Date of Birth: " << dob << endl;
        cout << "Balance: $" << balance << endl;
    }
};

// derived class
// using hierarchical inheritance
class CurrentAccount : public Account {
public:
	// constructor
	// current account contants
    CurrentAccount(const string& n, const string& fn, const string& c, const string& d, double initialBalance) : Account(n, fn, c, d, initialBalance) {}
    
	// display the current amount
    void displayMessage() {
        cout << "Current account saved successfully with an initial balance of $" << balance << "." << endl;
    }
};

// derived class
// using hierarchical inheritance
class SavingsAccount : public Account {
public:
	// constructor
	// saving account contants
    SavingsAccount(const string& n, const string& fn, const string& c, const string& d, double initialBalance) : Account(n, fn, c, d, initialBalance) {}
   
    // display the saving amount
    void displayMessage() {
        cout << "Savings account saved successfully with an initial balance of $" << balance << "." << endl;
    }
};
