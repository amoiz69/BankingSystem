#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <fstream>
#include <unordered_map>
using namespace std;

const int size = 30;

// Simple Bank class to hold bank details
class Bank
{
public:
    string name;
    string IBAN;

    Bank(string _name, string _IBAN) : name(_name), IBAN(_IBAN) {}
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
    string password;

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
    User(string _name, long _cnic, string _dob, Bank _bank, int _balance, string pass)
        : name(_name), cnic(_cnic), dob(_dob), bank(_bank), account_balance(_balance), password(pass)
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
        if (!inFile)
        {
            cerr << "Error: Unable to open file." << endl;
            return false; // Return false if unable to open the file
        }

        string line;
        while (getline(inFile, line))
        {
            std::istringstream iss(line); // Corrected namespace
            string token;
            vector<string> tokens;

            while (getline(iss, token, '|')) // Corrected namespace
            {
                tokens.push_back(token);
            }

            if (tokens.size() >= 9 && stol(tokens[1]) == cnic)
            {
                // CNIC match found, close the file and return true
                inFile.close();
                cout << "User found in file." << endl;
                return true;
            }
        }

        // Close the file and return false if CNIC match is not found
        inFile.close();
        cout << "User not found in file." << endl;
        return false;
    }

    // Function to retrieve account details from the file
    void retrieveAccountDetailsFromFile()
    {
        ifstream inFile("UserData.txt");
        if (!inFile)
        {
            cerr << "Error: Unable to open UserData.txt." << endl;
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            // Tokenize the line using '|' as delimiter
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, '|'))
            {
                tokens.push_back(token);
            }

            // Check if the line contains the user's CNIC
            if (tokens.size() >= 2 && tokens[1] == to_string(cnic))
            {
                // Ensure that the line has enough tokens for required fields
                if (tokens.size() >= 9)
                {
                    try
                    {
                        accountNumber = stoll(tokens[3]);
                        iban = tokens[6];
                        account_balance = stoi(tokens[7]);
                    }
                    catch (const std::invalid_argument &e)
                    {
                        cerr << "Error: Invalid argument encountered while parsing account number, IBAN, or balance." << endl;
                        inFile.close();
                        return;
                    }
                    catch (const std::out_of_range &e)
                    {
                        cerr << "Error: Out of range exception encountered while parsing account number, IBAN, or balance." << endl;
                        inFile.close();
                        return;
                    }
                }
                else
                {
                    cerr << "Error: Insufficient data in UserData.txt for user with CNIC: " << cnic << endl;
                    inFile.close();
                    return;
                }
                // Exit loop if user data is found
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
            myfile << name << "|" << cnic << "|" << dob << "|" << address << "|" << accountNumber << "|" << bank.name << "|" << iban << "|" << account_balance << "|" << password << endl;
            myfile.close();
            cout << "User data written to file successfully." << endl;
        }
        else
        {
            cerr << "Unable to open file." << endl;
        }
    }

    // Function to update balance in the UserData.txt file
    void updateBalanceInFile()
    {
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
            // Tokenize the line using '|' as delimiter
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, '|'))
            {
                tokens.push_back(token);
            }

            // Check if the line contains the user's CNIC
            if (tokens.size() >= 2 && tokens[1] == to_string(cnic))
            {
                // Update balance in the line
                if (tokens.size() >= 9)
                {
                    tokens[8] = to_string(account_balance); // Update balance
                    userFound = true;
                }
            }

            // Write the modified line to the temporary file
            for (const string &token : tokens)
            {
                tempFile << token << '|';
            }
            tempFile << endl;
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

void addTransaction(int accountNumber, double amount);

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

    void fundTransfer(User &sender, long long receiverAccountNumber, int amount, vector<Bank> &banks)
    {

        // Open the input file
        ifstream inFile("UserData.txt");
        if (!inFile)
        {
            cerr << "Error: Unable to open file." << endl;
            return;
        }

        bool receiverFound = false;
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, '|'))
            {
                tokens.push_back(token);
            }

            // Check if the line contains the receiver's account number
            if (tokens.size() >= 9 && stoll(tokens[4]) == receiverAccountNumber)
            {
                // Find the bank

                string name = tokens[0];
                long cnic = stol(tokens[1]);
                string dob = tokens[2];
                // Bank finalbank = userbank;
                int balance = stoi(tokens[7]);
                string pass = tokens[8];
                // Found the receiver
                User receiver(name, cnic, dob, banks[0], balance, pass);
                receiver.accountNumber = stoll(tokens[4]); // Assign account number

                // Perform transaction
                if (sender.account_balance < amount)
                {
                    cout << "Error: Insufficient Balance." << endl;
                    inFile.close();
                    return;
                }

                sender.account_balance -= amount;
                receiver.account_balance += amount;

                // Update balances in file
                cout << "Updating balances in file..." << endl;
                sender.updateBalanceInFile();
                receiver.updateBalanceInFile();

                cout << "Transaction Successful." << endl;

                receiverFound = true;
                break;
            }
        }

        // Close the file
        inFile.close();

        if (!receiverFound)
        {
            cout << "Error: Receiver not found." << endl;
        }
    }

    void payBills(User &sender, vector<Bill> &bills_arr, int consumer_id)
    {
        bool paid = false; // Flag to track if any bill was paid

        // Iterate over the bills
        for (int i = 0; i < bills_arr.size(); i++)
        {
            // Check if the bill belongs to the specified consumer ID
            if (bills_arr[i].consumerID == consumer_id)
            {
                // Display bill details
                cout << "The Details of your Bill are: " << endl;
                cout << "Billing Company: " << bills_arr[i].billing_company << endl;
                cout << "Consumer ID: " << bills_arr[i].consumerID << endl;
                cout << "Amount to Pay: Rs." << bills_arr[i].bill_amount << endl;
                cout << endl;

                char input;
                cout << "Enter Y/y to Pay or press any other character to cancel" << endl;
                cin >> input;

                if (toupper(input) == 'Y')
                {
                    // Check if the sender has sufficient funds
                    if (sender.account_balance < bills_arr[i].bill_amount)
                    {
                        cout << "Error: Insufficient Funds." << endl;
                        return;
                    }

                    // Pay the bill
                    sender.account_balance -= bills_arr[i].bill_amount;
                    cout << "Bill Paid." << endl;

                    // Mark the bill as paid
                    paid = true;
                    addTransaction(sender.accountNumber, bills_arr[i].bill_amount);
                    sender.updateBalanceInFile();

                    // Exit the loop after paying the first bill for the specified consumer ID
                    break;
                }
            }
        }

        // If no bill was paid, display an error message
        if (!paid)
        {
            cout << "Error: No Bill exists against this Consumer ID." << endl;
        }

        // Remove paid bills from the vector
        // bills_arr.erase(remove_if(bills_arr.begin(), bills_arr.end(),
        //                           [consumer_id](const Bill &bill)
        //                           { return bill.consumerID == consumer_id; }),
        //                 bills_arr.end());
    }
};

bool validateCredentials(long inputCNIC, string inputPassword)
{
    ifstream inFile("UserData.txt");
    if (!inFile)
    {
        cerr << "Error: Unable to open file." << endl;
        return false; // Return false if unable to open the file
    }

    string line;
    while (getline(inFile, line))
    {
        istringstream iss(line);
        string token;
        vector<string> tokens;

        while (getline(iss, token, '|'))
        {
            tokens.push_back(token);
        }

        if (tokens.size() >= 9 && stol(tokens[1]) == inputCNIC && tokens[8] == inputPassword)
        {
            // CNIC and password match, close the file and return true
            inFile.close();
            cout << "Credentials validated successfully." << endl;
            return true;
        }
    }

    // Close the file and return false if CNIC or password doesn't match
    inFile.close();
    cout << "CNIC or password not found." << endl;
    return false;
}

void addUser(vector<Bank> &banks, int size, vector<User> &users)
{
    string name, dob, address, password;
    long cnic;
    int bankchoice;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter your CNIC number (13 digits): ";
    cin >> cnic;

    //  if CNIC length is not equal to 13, then user will enter again
    if (to_string(cnic).length() != 13)
    {
        cout << "Invalid CNIC number. Please enter 13 digits: ";
        cin >> cnic;
    }

    cout << "Enter your date of birth(dd/mm/yyyy): ";
    cin >> dob;

    cout << "Enter your address: " << endl;
    getline(cin, address);

    cout << "Choose a bank: " << endl;
    for (int i = 0; i < banks.size(); i++)
    {
        cout << to_string(i + 1) << " " << banks[i].name << endl;
    }
    cin >> bankchoice;

    cout << "Enter a password: " << endl;
    cin >> password;

    User newUser(name, cnic, dob, banks[bankchoice - 1], 0, password);
    users.push_back(newUser);
}

// Initialize static variable
unordered_map<long, long long> User::cnicToAccountNumberMap;

const int MAX_TRANSACTIONS = 1000;

struct Transaction_record
{
    int accountNumber;
    time_t timeStamp;
    double amount;
};

Transaction_record transactions[MAX_TRANSACTIONS];
int numTransactions = 0;

void addTransaction(int accountNumber, double amount)
{
    if (numTransactions < MAX_TRANSACTIONS)
    {
        transactions[numTransactions].accountNumber = accountNumber;
        transactions[numTransactions].timeStamp = time(nullptr);
        transactions[numTransactions].amount = amount;
        numTransactions++;
    }
    else
    {
        cout << "Transaction history is full." << endl;
    }
}

string displayTransactionHistory()
{
    string history;
    history += "Transaction History:\n";
    for (int i = 0; i < numTransactions; ++i)
    {
        history += "Account: " + to_string(transactions[i].accountNumber) + ", ";
        history += "Time: " + string(ctime(&transactions[i].timeStamp));
        history += "Amount: " + to_string(transactions[i].amount) + "\n";
    }
    return history;
}

void getTransactionDetails(int accountNumber)
{
    cout << "Transaction Details for Account " << accountNumber << ":" << endl;
    bool found = false;
    for (int i = 0; i < numTransactions; ++i)
    {
        if (transactions[i].accountNumber == accountNumber)
        {
            found = true;
            cout << "Time: " << ctime(&transactions[i].timeStamp);
            cout << "Amount: " << transactions[i].amount << endl;
        }
    }
    if (!found)
    {
        cout << "No transactions found for account " << accountNumber << "." << endl;
    }
}

int main()
{
    fstream myfile("Transaction_Details.txt");
    // fstream file("Transaction_Details.txt");
    //  Sample banks
    vector<Bank> banksvector;

    Bank Meezan("Meezan", "PK47");
    Bank HBL("HBL", "PK48");
    Bank Sadapay("Sadapay", "PK50");
    Bank Nayapay("Nayapay", "PK49");

    banksvector.push_back(Meezan);
    banksvector.push_back(HBL);
    banksvector.push_back(Sadapay);
    banksvector.push_back(Nayapay);

    // Bank banks[size] = {Meezan, HBL, Nayapay, Sadapay};

    // Vector to store bills
    vector<Bill> bills;

    // Sample Bills
    Bill bill1(141523, 3500, "IESCO");
    Bill bill2(141569, 2500, "SNGPL");
    Bill bill3(141570, 3300, "PTCL");

    User sender("Abdul Moiz", 3740678496467, "06/10/2005", banksvector[0], 10000, "moiz123");
    User reciever("Abdullah Abid", 3740678597467, "11/09/2001", banksvector[1], 7000, "abdullah123");
    vector<User> users;
    users.push_back(sender);
    users.push_back(reciever);

    Transaction transaction;

    // Adding bills to vector
    bills.push_back(bill1);
    bills.push_back(bill2);
    bills.push_back(bill3);

    cout << "* * * * * * * * * * * * * * * * *" << endl;
    cout << "* Welcome to the Banking System *" << endl;
    cout << "* * * * * * * * * * * * * * * * *" << endl;
    cout << endl;

    int userloginchoice, userchoice, transfer_amount, consumer_id;
    long usercnic;
    long long beneficiary_accountnumber;
    string userpassword;
    cout << "Enter:\n1. Login\n2.Register " << endl;
    cin >> userloginchoice;

    switch (userloginchoice)
    {
        // Case to Login for existing user
    case 1:
        cout << "Enter your CNIC to login: " << endl;
        cin >> usercnic;
        cout << "Enter your password: " << endl;
        cin >> userpassword;
        if (validateCredentials(usercnic, userpassword))
        {
            cout << "Welcome!\n 1.Check Account Balance\n 2.Check Account Details\n3.Transfer Funds\n4.Pay Bills\n5.Check Transaction History" << endl;
            cin >> userchoice;
            switch (userchoice)
            {
            case 1:
                cout << "Your balance is Rs." << sender.account_balance << endl;
                break;

            case 2:
                cout << "Here are your Details: " << endl;
                cout << "Account Number: " << sender.accountNumber << endl;
                cout << "IBAN: " << sender.iban << endl;
                break;

            case 3:
                cout << "Enter the Account Number of the beneficiary: " << endl;
                cin >> beneficiary_accountnumber;
                cout << "Enter the amount to transfer" << endl;
                cin >> transfer_amount;
                transaction.fundTransfer(sender, beneficiary_accountnumber, transfer_amount, banksvector);
                addTransaction(sender.accountNumber, transfer_amount);
                break;

            case 4:
                cout << "Enter the Consumer ID for the bill you want to pay. " << endl;
                cin >> consumer_id;
                transaction.payBills(sender, bills, consumer_id);

                break;

            case 5:
                if (myfile.is_open())
                {
                    string line;

                    while (getline(myfile, line))
                    {
                        cout << line << endl;
                    }
                }
                else
                {
                    cout << "Unable to open file" << endl;
                }
                break;

            default:
                cout << "Invalid Option! Please enter again" << endl;
                break;
            }
        }
        else
        {
            cout << "Invalid Credentials!" << endl;
        }

        break;
    // Case to Register a new User
    case 2:
        addUser(banksvector, size, users);

        break;
    default:
        cout << "Wrong Input.Try Again." << endl;
        break;
    }

    if (myfile.is_open())
    {
        // Write transaction history to file
        myfile << displayTransactionHistory() << endl;
        myfile.close();
        cout << "Transaction history has been written to Transaction_Details.txt" << endl;
    }
    else
    {
        cout << "Unable to open file." << endl;
    }

    return 0;
}
