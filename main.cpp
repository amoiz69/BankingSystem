#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <unordered_map>
using namespace std;

const int size = 30;

// Simple Bank structure to hold bank details
struct Bank
{
    string name;
    string IBAN;
};

// Declaration of the User class
class User
{
private:
    static unordered_map<long, long long> cnicToAccountNumberMap; // Map to store CNIC to account number mapping

public:
    string name;
    long cnic;
    string dob;
    string address;
    long long accountNumber; // Use long long for account number
    Bank bank;
    string iban;
    int account_balance;

    // Function to generate account number based on CNIC
    void generateAccountNumber()
    {
        if (cnicToAccountNumberMap.find(cnic) == cnicToAccountNumberMap.end())
        {
            // If CNIC not found in the map, generate a new account number
            srand(cnic);                                      // Seed based on CNIC for consistency
            accountNumber = 1000000000 + rand() % 9000000000; // Generate a 10-digit number
            cnicToAccountNumberMap[cnic] = accountNumber;     // Store in map for future reference
        }
        else
        {
            // If CNIC found in the map, use the existing account number
            accountNumber = cnicToAccountNumberMap[cnic];
        }
    }

    // Function to generate IBAN
    void generateIBAN()
    {
        // Generate a random IBAN (for demonstration purposes, this generates a simple IBAN)
        iban = bank.IBAN + to_string(accountNumber);
    }

    // Constructor with file writing
    User(string _name, long _cnic, string _dob, Bank _bank, int _balance)
        : name(_name), cnic(_cnic), dob(_dob), bank(_bank), account_balance(_balance)
    {
        // Check if the user already exists in the file
        if (userExistsInFile())
        {
            // If user exists, retrieve account number and IBAN from the file
            retrieveAccountDetailsFromFile();
        }
        else
        {
            // If user does not exist, generate account number and IBAN
            generateAccountNumber();
            generateIBAN();
            writeToFile(); // Write user data to file
        }
    }

    // Function to check if user already exists in the file
    bool userExistsInFile()
    {
        ifstream inFile("UserData.txt");
        string line;
        while (getline(inFile, line))
        {
            if (line.find("CNIC: " + to_string(cnic)) != string::npos)
            {
                inFile.close();
                return true; // User found in file
            }
        }
        inFile.close();
        return false; // User not found in file
    }

    // Function to retrieve account details from the file
    void retrieveAccountDetailsFromFile()
    {
        ifstream inFile("UserData.txt");
        string line;
        while (getline(inFile, line))
        {
            if (line.find("CNIC: " + to_string(cnic)) != string::npos)
            {
                // Account number
                getline(inFile, line);
                size_t pos = line.find("Account Number: ");
                accountNumber = stoll(line.substr(pos + 17));

                // IBAN
                getline(inFile, line);
                pos = line.find("IBAN: ");
                iban = line.substr(pos + 6);

                // Exit loop
                break;
            }
        }
        inFile.close();
    }

    // Function to write user data to file
    void writeToFile()
    {
        ofstream myfile("UserData.txt", ios::app); // Append mode

        if (myfile.is_open())
        {
            myfile << "Name: " << name << endl;
            myfile << "CNIC: " << cnic << endl;
            myfile << "Date of Birth: " << dob << endl;
            myfile << "Address: " << address << endl;
            myfile << "Account Number: " << accountNumber << endl;
            myfile << "Bank: " << bank.name << endl;
            myfile << "IBAN: " << iban << endl;
            myfile << "Balance: Rs." << account_balance << endl;
            myfile << endl; // Add a newline between user entries
            myfile.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }

    // Update balance in file
    void updateBalanceInFile()
    {
        // Debug: Print the CNIC of the user
        cout << "User CNIC: " << cnic << endl;

        // Open the input file
        ifstream inFile("UserData.txt");
        if (!inFile)
        {
            cerr << "Error: Unable to open input file." << endl;
            return;
        }

        // Create a temporary file
        ofstream tempFile("tempUserData.txt");
        if (!tempFile)
        {
            cerr << "Error: Unable to open temporary file." << endl;
            inFile.close();
            return;
        }

        string line;
        bool userFound = false;

        // Read the file line by line
        while (getline(inFile, line))
        {
            // Debug: Print each line of the file
            cout << "Line: " << line << endl;

            // Check if the line contains the user's CNIC
            if (line.find("CNIC: " + to_string(cnic)) != string::npos)
            {
                // Update balance in the line
                size_t pos = line.find("Balance: Rs.");
                if (pos != string::npos)
                {
                    line = line.substr(0, pos + 10) + to_string(account_balance); // Update balance
                    userFound = true;
                }
            }

            // Write the line to the temporary file
            tempFile << line << endl;
        }

        // Close both files
        inFile.close();
        tempFile.close();

        // Check if the user was found in the file
        if (!userFound)
        {
            cerr << "Error: User not found in file. Balance not updated." << endl;
            return;
        }

        // Remove the original file
        remove("UserData.txt");

        // Rename the temporary file to the original file name
        rename("tempUserData.txt", "UserData.txt");

        cout << "Balance updated in file." << endl;
    }

    // Display user data
    void displayUser()
    {
        cout << "Name: " << name << endl;
        cout << "CNIC: " << cnic << endl;
        cout << "Date of Birth: " << dob << endl;
        cout << "Address: " << address << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Bank: " << bank.name << endl;
        cout << "IBAN: " << iban << endl;
        cout << "Balance: Rs." << account_balance << endl;
    }
};

// Structure named Bill
class Bill
{
public:
    long consumerID;
    int bill_amount;
    string billing_company;

    Bill(long id, int amount, string company) : consumerID(id), bill_amount(amount), billing_company(company) {}
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

        // // Update balances in file
        // cout << "Updating balances in file..." << endl;
        // sender.updateBalanceInFile();
        // receiver.updateBalanceInFile();
        // cout << "Balances updated in file." << endl;
    }

    void payBills(User &sender, vector<Bill> &bills_arr, int consumer_id)
    {
        for (int i = 0; i < bills_arr.size(); i++)
        {
            if (bills_arr[i].bill_amount < sender.account_balance)
            {
                cout << "Error: Insufficient Funds." << endl;
                return;
            }
        }

        for (int i = 0; i < bills_arr.size(); i++)
        {
            if (bills_arr[i].consumerID != consumer_id)
            {
                cout << "Error: No Bill exists against this Consumer ID." << endl;
                return;
            }
        }

        for (int i = 0; i < bills_arr.size(); i++)
        {
            char input;
            if (bills_arr[i].consumerID == consumer_id)
            {
                cout << "The Details of your Bill are: " << endl;
                cout << "Billing Company: " << bills_arr[i].billing_company << endl;
                cout << "Consumer ID: " << bills_arr[i].consumerID << endl;
                cout << "Amount to Pay: Rs." << bills_arr[i].bill_amount << endl;
                cout << endl;
                cout << "Enter Y/y to Pay or press any other character to cancel" << endl;
                cin >> input;
                if (toupper(input) == 'Y')
                {
                    sender.account_balance -= bills_arr[i].bill_amount;
                    cout << "Bill Paid." << endl;
                }
                else
                {
                    return;
                }
            }
        }
    }
};

// Initialize static variable
unordered_map<long, long long> User::cnicToAccountNumberMap;

int main()
{
    // Sample banks
    Bank banks[size] = {
        {"Meezan", "PK47"},
        {"HBL", "PK48"},
        {"Sadapay", "PK49"},
        {"Nayapay", "PK50"}};

    Bill bill1(141523, 3500, "IESCO");
    Bill bill2(141569, 2500, "SNGPL");
    Bill bill3(141570, 3300, "PTCL");

    vector<Bill> bills(30);
    bills.push_back(bill1);
    bills.push_back(bill2);
    bills.push_back(bill3);

    // Sample users
    User sender("Abdul Moiz", 3740678497467, "06/10/2005", banks[0], 10000);
    User receiver("Fakhir", 3740678495457, "10/10/2005", banks[1], 5000);

    // Sample transaction
    Transaction transaction;
    transaction.fundTransfer(sender, receiver, 100);

    cout << sender.account_balance << endl;
    cout << receiver.account_balance << endl;

    sender.updateBalanceInFile();
    receiver.updateBalanceInFile();

    transaction.payBills(sender, bills, 141523);

    return 0;
}
