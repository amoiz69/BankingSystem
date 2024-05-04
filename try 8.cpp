#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

const int MAX_TRANSACTIONS = 100;

struct Transaction {
    int accountNumber;
    time_t timeStamp;
    double amount;
};

Transaction transactions[MAX_TRANSACTIONS];
int numTransactions = 0;

void addTransaction(int accountNumber, double amount) {
    if (numTransactions < MAX_TRANSACTIONS) {
        transactions[numTransactions].accountNumber = accountNumber;
        transactions[numTransactions].timeStamp = time(nullptr);
        transactions[numTransactions].amount = amount;
        numTransactions++;
    } else {
        cout << "Transaction history is full." << endl;
    }
}

string displayTransactionHistory() {
    string history;
    history += "Transaction History:\n";
    for (int i = 0; i < numTransactions; ++i) {
        history += "Account: " + to_string(transactions[i].accountNumber) + ", ";
        history += "Time: " + string(ctime(&transactions[i].timeStamp));
        history += "Amount: " + to_string(transactions[i].amount) + "\n";
    }
    return history;
}

void getTransactionDetails(int accountNumber) {
    cout << "Transaction Details for Account " << accountNumber << ":" << endl;
    bool found = false;
    for (int i = 0; i < numTransactions; ++i) {
        if (transactions[i].accountNumber == accountNumber) {
            found = true;
            cout << "Time: " << ctime(&transactions[i].timeStamp);
            cout << "Amount: " << transactions[i].amount << endl;
        }
    }
    if (!found) {
        cout << "No transactions found for account " << accountNumber << "." << endl;
    }
}

int main() {
    ofstream myfile("Transaction_Details.txt", ios::out);
    addTransaction(2023032, 5000.0);
    addTransaction(2023010, 10000.0);
    addTransaction(2023195, 25000.57);
    addTransaction(2023011, 45000.76);
    addTransaction(2023764, 6438.43);
	addTransaction(2023564, 200.43);
	
    if (myfile.is_open()) {
        // Write transaction history to file
        myfile << displayTransactionHistory() << endl;
        myfile.close();
        cout << "Transaction history has been written to Transaction_Details.txt" << endl;
    } else {
        cout << "Unable to open file." << endl;
    }

    return 0;
}
