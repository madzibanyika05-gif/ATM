#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations
#include <sstream> // for string streams
#include <chrono>
#include <iomanip>

using namespace std;

//phase 2.2 creating class for currency converter had to move to the begining
class CurrencyConverter {
private:
    const double gbpToUsd = 1.27; // backup average exchange rate if api fails
    const double gbpToEur = 1.17;
    const double usdToGbp = 1 / 1.27;
    const double eurToGbp = 1 / 1.17;

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

//phase 4.0 transaction history
struct Transaction {
    string type;
    double amount;
    string timestamp;
};

string getCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm tm_struct;
    localtime_s(&tm_struct, &now_time);
    stringstream ss;
    ss << put_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

//phase 1.1 adding account class
class Account {
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

    vector<Transaction> transactions;

    void logTransaction(string type, double amount) {
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        transactions.push_back(t);

        ofstream log("account_transactions.txt", ios::app);
        if (log.is_open()) {
            log << t.timestamp << "|" << type << "|£" << amount << endl;
        }
    }

public:
    double getBalance() { return balance; }

    void saveToFile() {
        ofstream file("accounts.txt", ios::trunc);
        if (file.is_open()) {
            file << accountNumber << " "
                << sortCode << " "
                << pin << " "
                << balance << "\n";
            file.close();
        }
        else {
            cout << "Error saving account!\n";
        }
    }

    void createAccount() {
        cout << "Enter your account number: ";
        cin >> accountNumber;
        cout << "Enter your sort code: ";
        cin >> sortCode;
        cout << "Enter your pin: ";
        cin >> pin;
        balance = 1000.00;
        saveToFile();
        cout << "Account created successfully.\n";
    }

    void resetPIN() {
        int oldPIN, newPIN, confirmPIN;
        cout << "Enter old pin: ";
        cin >> oldPIN;

        if (oldPIN != pin) {
            cout << "Incorrect pin.\n";
            return;
        }

        cout << "Enter new pin: ";
        cin >> newPIN;
        cout << "Confirm pin: ";
        cin >> confirmPIN;

        if (newPIN != confirmPIN) {
            cout << "Pin dooes not match.\n";
            return;
        }

        pin = newPIN;
        saveToFile();
        cout << "Pin change successful.\n";
    }

    // Modified to support different transaction types
    void deposit(double amount, string fromCurrency = "GBP", string transType = "DEPOSIT") {
        if (fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
            logTransaction("FOREIGN DEPOSIT", amount);
        }
        else {
            balance += amount;
            saveToFile();
            logTransaction(transType, amount);
            cout << "Deposit successful.\n";
        }
    }

    // Modified to support different transaction types
    void withdraw(double amount, string transType = "WITHDRAWAL") {
        if (amount > balance) {
            cout << "Insufficient funds.\n";
        }
        else {
            balance -= amount;
            saveToFile();
            logTransaction(transType, amount);
            cout << "Withdrawl successful.\n";
        }
    }

    void checkBalance() {
        cout << "current balance: £" << balance << "\n";
    }

    void showTransactionHistory() {
        cout << "\nAccount Transaction History:\n";
        for (auto& t : transactions) {
            cout << t.timestamp << " - " << t.type << " £" << t.amount << endl;
        }
    }
};

//phase 2.1 creating class for savings
class Savings {
private:
    int saccountNumber;
    int ssortCode;
    double sbalance;
    double interestRate = 0.02;

    vector<Transaction> transactions;

    void logTransaction(string type, double amount) {
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        transactions.push_back(t);

        ofstream log("savings_transaction.txt", ios::app);
        if (log.is_open()) {
            log << t.timestamp << "|" << type << "| £" << amount << endl;
        }
    }

public:
    double getBalance() { return sbalance; }  // Added balance getter

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
        double interest = sbalance * interestRate;
        sbalance += interest;
        saveToFile();
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

    // Modified to support different transaction types
    void deposit(double amount, string transType = "DEPOSIT") {
        sbalance += amount;
        applyInterest();
        saveToFile();
        logTransaction(transType, amount);
        cout << "Savings deposit successful.\n";
    }

    // Modified to support different transaction types
    void withdraw(double amount, string transType = "WITHDRAWAL") {
        if (amount > sbalance) {
            cout << "Insufficient funds.\n";
        }
        else {
            sbalance -= amount;
            applyInterest();
            saveToFile();
            logTransaction(transType, amount);
            cout << "Savings withdrawl successful.\n";
        }
    }

    void checkBalance() {
        cout << "Savings balance: £" << sbalance << "\n";
    }

    void showTransactionHistory() {
        cout << "\nSavings Transaction History:\n";
        for (auto& t : transactions) {
            cout << t.timestamp << " - " << t.type << " £" << t.amount << endl;
        }
    }
};

// Updated main menu with bidirectional transfers
void showMainMenu(bool hasSavings) {
    cout << "\n-=-=-=- Haven ATM, Main Menu -=-=-=-\n";
    cout << "1. Check balance\n";
    cout << "2. Deposit money\n";
    cout << "3. Withdraw money\n";
    cout << "4. Deposit foreign currency\n";
    cout << "5. Reset pin\n";
    cout << "6. Transaction history\n";
    if (hasSavings) {
        cout << "7. Transfer to Savings\n";
        cout << "8. Transfer from Savings\n";  // New transfer option
        cout << "9. Check savings balance\n";
        cout << "10. Exit\n";
    }
    else {
        cout << "7. Create savings account\n";
        cout << "8. Exit\n";
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

        case 7: {
            if (hasSavings) {
                double amount;
                cout << "Transfer amount to savings: £";
                cin >> amount;
                if (amount <= user.getBalance()) {
                    // Use TRANSFER_OUT type for main account
                    user.withdraw(amount, "TRANSFER_OUT");
                    // Use TRANSFER_IN type for savings account
                    usersavings.deposit(amount, "TRANSFER_IN");
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

        case 8: {  // New transfer from savings case
            if (hasSavings) {
                double amount;
                cout << "Transfer amount to main account: £";
                cin >> amount;
                if (amount <= usersavings.getBalance()) {
                    // Use TRANSFER_OUT type for savings account
                    usersavings.withdraw(amount, "TRANSFER_OUT");
                    // Use TRANSFER_IN type for main account
                    user.deposit(amount, "GBP", "TRANSFER_IN");
                }
                else {
                    cout << "Insufficient funds in savings.\n";
                }
            }
            else {
                cout << "Thank you for banking with Haven ATM.\n";
                return 0;
            }
            break;
        }

        case 5: {
            user.resetPIN();
            break;
        }

        case 6: {
            user.showTransactionHistory();
            break;
        }

        case 9: {
            if (hasSavings) {
                usersavings.checkBalance();
                break;
            }
            else {
                cout << "Thank you for banking with Haven ATM.\n";
                return 0;
            }
            break;
        }

        case 10: {
            if (hasSavings) {
                cout << "Thank you for banking with Haven ATM.\n";
                return 0;
            }
            break;
        }

        case 4: {
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