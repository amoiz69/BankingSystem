#include <iostream>
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

void displayTransactionHistory() {
    cout << "Transaction History:" << endl;
    for (int i = 0; i < numTransactions; ++i) {
        cout << "Account: " << transactions[i].accountNumber << ", ";
        cout << "Time: " << ctime(&transactions[i].timeStamp);
        cout << "Amount: " << transactions[i].amount << endl;
    }
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
    // Sample usage of the functions
    addTransaction(2023032, 5000.0);
    addTransaction(2023010, 10000.0);
    addTransaction(2023195, 25000.57);
    addTransaction(2023011, 45000.76);

    displayTransactionHistory();

    cout << endl;

    getTransactionDetails(2023011);

    return 0;
}
