#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    string accountHolderName;
    string senderName; // New member variable
    string receiverName; // New member variable
    double balance;

public:
    BankAccount(string accNum, string accHolderName, string sender, string receiver, double initialBalance) {
        accountNumber = accNum;
        accountHolderName = accHolderName;
        senderName = sender; // Initialize sender name
        receiverName = receiver; // Initialize receiver name
        balance = initialBalance;
    }

    void displayAccountDetails() {
        cout << "Account Details:" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder Name: " << accountHolderName << endl;
        cout << "Sender Name: " << senderName << endl; // Display sender name
        cout << "Receiver Name: " << receiverName << endl; // Display receiver name
        cout << "Balance: $" << balance << endl;
    }

    void checkBalance() {
        cout << "Current Balance: $" << balance << endl;
    }
};

int main() {
    // Create a BankAccount object with sender and receiver names
    BankAccount myAccount("123456789", "John Doe", "Jane Smith", "John Doe", 1000.00);

    // Display account details
    myAccount.displayAccountDetails();

    // Check account balance
    myAccount.checkBalance();

    return 0;
}
