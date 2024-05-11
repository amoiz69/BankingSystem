#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

class User
{
};

vector<User> NumUsers;

// Simple Bank structure to hold bank details
struct Bank
{
    string name;
    string IBAN;
};

// Forward declaration of the User class
class User
{
public:
    string name;
    int cnic;
    string dob;
    string address;
    int accountNumber;
    Bank bank;
    string iban;
    int account_balance;

    // Function to generate a random 10-digit account number
    void generateAccountNumber()
    {
        // Seed for random number generation
        srand(time(nullptr));
        // Generate a random 10-digit account number
        accountNumber = 0;
        for (int i = 0; i < 10; ++i)
        {
            accountNumber = accountNumber * 10 + (rand() % 10);
        }
    }

    // Function to generate IBAN
    void generateIBAN()
    {
        // Generate a random IBAN (for demonstration purposes, this generates a simple IBAN)
        iban = bank.IBAN + to_string(accountNumber);
    }

    User(string name, int cnic, string dob, Bank bank, int balance)
        : name(name), cnic(cnic), dob(dob), bank(bank), account_balance(balance)
    {
    }
};

class Transaction
{
private:
    bool status; // Transaction status
    User *sender;
    User *receiver;
    int amount;

public:
    // Constructor
    Transaction() : status(false){};

    // Function to perform transaction
    void fundTransfer(User &sender, User &receiver, int amount)
    {
        if (sender.account_balance < amount)
        {
            cout << "Error: Insufficient Balance." << endl;
            return;
        }

        sender.account_balance -= amount;
        receiver.account_balance += amount;

        status = true;
        cout << "Transaction Successful." << endl;
    }
};

int main()
{
    // Seed for random number generation
    srand(time(nullptr));

    fstream myfile("UserData.txt", ios::in);

    // Sample banks
    Bank banks[10] = {
        {"Meezan", "PK47"},
        {"HBL", "PK48"},
        {"Sadapay", "PK49"},
        {"Nayapay", "PK50"}};

    // Sample users
    User sender("Abdul Moiz", 3740678497467, "06/10/2005", banks[0], 10000);
    User receiver("Fakhir", 3740678495457, "10/10/2005", banks[1], 5000);

    // Sample transaction
    Transaction transaction;
    transaction.fundTransfer(sender, receiver, 100);

    // Print sender's and receiver's updated balances
    cout << "Sender's Updated Balance: " << sender.account_balance << endl;
    cout << "Receiver's Updated Balance: " << receiver.account_balance << endl;

    return 0;
}
