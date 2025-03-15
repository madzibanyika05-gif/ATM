#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations

using namespace std;

class Account {// class for creating an account and holding account details
private:
    int accountNumber;
    int pin;
    int sortCode;
    double balance;

public:
    void saveToFile() {// function to save account details to file
        ofstream file("accounts.txt", ios::app);  // ios::app = append mode this adds to file instead of overwriting

        if (file.is_open()) {//checks if file opens successfully and writes account number etc...
            file << accountNumber << " "
                << sortCode << " "
                << pin << " "
                << balance << "\n";
            file.close();
        }
        else {
            cout << "Error saving account!\n";//error if file cant open
        }
    }

    void createAccount() {//function to create account by getting user info
        cout << "Enter your account number: ";// questions
        cin >> accountNumber;
        cout << "Enter your sort code: ";
        cin >> sortCode;
        cout << "Enter your pin: ";
        cin >> pin;
        balance = 1000000.00; //startinh balance of 1m
        saveToFile();
        cout << "Account created successfully.\n";//account made
    }
    void deposit(double amount) {//trasaction function being created
        balance += amount;
        saveToFile();//overites data to show new balace
        cout << "Deposit successful.\n";
    }
    
    void withdraw(double amount) {//function for withdraws
        if (amount > balance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        } else {//anything else allow andf save new balance to file
            balance -= amount;
            saveToFile();
            cout << "Withdrawl successful.\n";
        }
    }
    
    void checkBalance() {
        cout << "current balance: £" << balance << "\n";
    }
};

int main() {
    Account user;
    user.createAccount();
    return 0;// program ended succesfully
}

