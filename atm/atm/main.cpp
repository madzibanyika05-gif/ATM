#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations
#include <sstream> // for string streams

using namespace std;
//phase 2.2 creating class for currency converter
class CurrencyConverter {
private:
    const double gbpToUsd = 1.27; // backup average exchange rate if api fails
    const double gbpToEur = 1.17;
    const double usdToGbp = 0.86;
    const double eurToGbp = 1.14;

public:
    //menu for currency converter
    void showCurrencyMenu() {
        cout << "\n-=-=-=- Select currency -=-=-=-\n";
        cout << "1. GBP (£)\n";
        cout << "2. USD ($)\n";
        cout << "3. EUR (€)\n";
        cout << "Choice: ";
    }

    double convert(double amount, string from, string to) {
        if (from == "GBP" && to == "USD") return amount * gbpToUsd;
        if (from == "GBP" && to == "EUR") return amount * gbpToEur;
        if (from == "USD" && to == "GBP") return amount * usdToGbp;
        if (from == "EUR" && to == "GBP") return amount * eurToGbp;
        return amount; // Fallback
    }
};
//phase 1.1 adding account class
class Account {// class for creating an account and holding account details
private:
    int accountNumber;
    int pin;
    int sortCode;
    double balance;
    string currency = "GBP";

    void convertAndDeposit(double amount, string fromCurrency) {
        CurrencyConverter converter;
        double converted = converter.convert(amount, fromCurrency, currency);
        balance += converted;
        saveToFile();
        cout << "Deposit successful. Converted amount: £" << converted << "\n";
    }

public:
    double getBalance() { return balance; }

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
        balance = 1000.00; //startinh balance of 1k
        saveToFile();
        cout << "Account created successfully.\n";//account made
    }

    void deposit(double amount, string fromCurrency = "GBP") {//trasaction function being created
        if (fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
        }
        else {
            balance += amount;
            saveToFile();//overites data to show new balace
            cout << "Deposit successful.\n";
        }
    }

    void withdraw(double amount) {//function for withdraws
        if (amount > balance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
            balance -= amount;
            saveToFile();
            cout << "Withdrawl successful.\n";
        }
    }

    void checkBalance() {
        cout << "current balance: £" << balance << "\n";
    }
};
//phase 2.1 creating class for savings
class Savings {//class for savings
private:
    int saccountNumber;
    int ssortCode;
    double sbalance;
    double interestRate = 0.02; //2% intrest rate from trasnfering into savings

public:
    void createSavingsAccount() {
        cout << "\n-=-=-=- Savings Account Setup -=-=-=-\n ";
        cout << "Enter savings account number: ";
        cin >> saccountNumber;
        cout << "Enter sort code: ";
        cin >> ssortCode;
        sbalance = 10.00;
        saveToFile("savings.txt");
        cout << "Savings account created!\n";
    }

    void applyInterest() {
        sbalance += sbalance * interestRate;
        saveToFile("savings.txt");
        cout << "Interest applied: £" << sbalance << "\n";
    }

    void saveToFile(string filename = "savings.txt") {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << saccountNumber << " "
                << ssortCode << " "
                << sbalance << "\n";
            file.close();
        }
    }
    void deposit(double amount) {//trasaction function being created
        sbalance += amount;
        saveToFile();//overites data to show new balace
        cout << "Savings deposit successful.\n";
    }

    void withdraw(double amount) {//function for withdraws
        if (amount > sbalance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
            sbalance -= amount;
            saveToFile();
            cout << "Savings withdrawl successful.\n";
        }
    }

    void checkBalance() {
        cout << "Savings balance: £" << sbalance << "\n";
    }
};

//now creating main menu
void showMainMenu(bool hasSavings) {
    cout << "\n-=-=-=- Haven ATM, Main Menu -=-=-=-\n";
    cout << "1. Check balance\n";
    cout << "2. Deposit money\n";
    cout << "3. Withdraw money\n";
    cout << "4. Deposit foreign currency\n";
    if (hasSavings) {
        cout << "5. Transfer to Savings\n";
        cout << "6. Check savings balance\n";
        cout << "7. Exit\n";
    }
    else {
        cout << "5. Create savings account\n";
        cout << "6. Exit\n";
    }
}

int main() {
    Account user;
    Savings usersavings;
    CurrencyConverter converter;
    bool hasSavings = false;
    user.createAccount();

    int choice;
    do {
        showMainMenu(hasSavings);
        cin >> choice;

        switch (choice) {
        case 1:
            user.checkBalance();
            break;

        case 2: {
            double amount;
            cout << "Enter deposit amount: £";
            cin >> amount;
            user.deposit(amount);
            break;
        }

        case 3: {
            double amount;
            cout << "Enter withdrawal amount: £";
            cin >> amount;
            user.withdraw(amount);
            break;
        }

        case 4: {
            if (hasSavings) {
                double amount;
                cout << "Transfer amount: £";
                cin >> amount;
                if (amount <= user.getBalance()) {
                    user.withdraw(amount);
                    usersavings.deposit(amount);
                }
                else {
                    cout << "Insufficient funds for transfer.\n";
                }
            }
            else {
                usersavings.createSavingsAccount();
                hasSavings = true;
            }
            break;
        }

        case 5: {
            if (hasSavings)
                usersavings.checkBalance();
            break;
        }

        case 6: {
            if (hasSavings) {
                cout << "Thank you for banking with Haven ATM.\n";
                return 0;
            }
            break;
        }

        case 7: {
            double amount;
            int currencyChoice;
            converter.showCurrencyMenu();
            cin >> currencyChoice;

            cout << "Enter amount: ";
            cin >> amount;

            string currencies[] = { "GBP", "USD", "EUR" };
            user.deposit(amount, currencies[currencyChoice - 1]);
            break;
        }

        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (true);

    return 0;
}
