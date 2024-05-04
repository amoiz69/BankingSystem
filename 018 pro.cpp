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

int main() {
    char choice;
    cout << "Do you want to create an account? (Y/N): ";
    cin >> choice;

    if (choice == 'Y' || choice == 'y') {
        string name, fatherName, cnic, dob;
        int accountType;
        double initialBalance;

        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter your father's name: ";
        getline(cin, fatherName);

        cout << "Enter your CNIC number (13 digits): ";
        cin >> cnic;
     
        //  if CNIC length is not equal to 13, then user will enter again
        if (cnic.length() != 13) {
            cout << "Invalid CNIC number. Please enter 13 digits: ";
            cin >> cnic;
        }

        cout << "Enter your date of birth: ";
        cin >> dob;

        cout << "Choose account type:\n1. Current Account\n2. Savings Account\nEnter choice (1/2): ";
        cin >> accountType;

         double minimumBalance;
        
        // assigning the minimum balance on every account
        if (accountType == 1) {
            minimumBalance = 100;
        } else if (accountType == 2) {
            minimumBalance = 200;
        } else {
            cerr << "Invalid choice." << endl;
            return 1;
        }

        // ask the user again again untill it write the required amount of money
        do {
            cout << "Enter initial balance: $";
            cin >> initialBalance;

            if (initialBalance < minimumBalance) {
                cout << "Initial balance for ";

                if (accountType == 1) {
                    cout << "a current account must be at least $100. ";
                } else {
                    cout << "a savings account must be at least $200. ";
                }

                cout << "Please enter again." << endl;
            }

        } while (initialBalance < minimumBalance);

        Account* acc;
        // applying switch case for account type
        switch (accountType) {
            case 1:
                acc = new CurrentAccount(name, fatherName, cnic, dob, initialBalance);
                break;
            case 2:
                acc = new SavingsAccount(name, fatherName, cnic, dob, initialBalance);
                break;
            default:
                cerr << "Invalid choice." << endl;
                return 1;
        }

        acc->displayMessage();
        acc->displayDetails();

        delete acc;
    } else {
        cout << "Thank you." << endl;
    }

    return 0;
}