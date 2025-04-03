#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations
#include <sstream> // for string streams
#include <chrono>
#include <iomanip>
#include <cmath> // for round function

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
        cout << "1. USD ($)\n"; // got rid of GBP cause not a foreign currency
        cout << "2. EUR (€)\n";
        cout << "Choice: ";
    }

    double convert(double amount, string from, string to) {
        if (from == "GBP" && to == "USD") return amount * gbpToUsd;
        if (from == "GBP" && to == "EUR") return amount * gbpToEur;
        if (from == "USD" && to == "GBP") return amount * usdToGbp;
        if (from == "EUR" && to == "GBP") return amount * eurToGbp;
        return amount; // fallback
    }
};

//phase 4.0 transaction history
struct Transaction {
    string type;
    double amount;
    string timestamp;
    string currency;  // Added to track currency type
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

    vector<Transaction> transactions;

    void logTransaction(string type, double amount, string curr = "GBP") {  // Modified
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        t.currency = curr;  // Store currency information
        transactions.push_back(t);

        ofstream log("account_transactions.txt", ios::app);// saving transactions to files
        if (log.is_open()) {
            log << t.timestamp << "|" << type << "|" << curr << "|£" << amount << endl;
        }
    }

public:
    double getBalance() { return balance; }

    void saveToFile() {// function to save account details to file
        ofstream file("accounts.txt", ios::trunc);  // ios::app = append mode this adds to file instead of overwriting//changed to trunc to overwrite
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
    void resetPIN() {//reset pin function update 3.5
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

        if (newPIN != confirmPIN) {// if pins do not have same value
            cout << "Pin dooes not match.\n";
            return;
        }

        pin = newPIN;
        saveToFile();
        cout << "Pin change successful.\n";
    }
    void deposit(double amount, string fromCurrency = "GBP", string transType = "DEPOSIT") {
        if (fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
            logTransaction("FOREIGN DEPOSIT", amount, fromCurrency);  // Log currency
        }
        else {
            balance += amount;
            saveToFile();//overites data to show new balace
            logTransaction(transType, amount);
            cout << "Deposit successful.\n";
        }
    }

    void withdraw(double amount, string transType = "WITHDRAWAL") {
        if (amount > balance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
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
            string currencySymbol = (t.currency == "GBP") ? "£" :
                (t.currency == "USD") ? "$" :
                (t.currency == "EUR") ? "€" : "";

            cout << t.timestamp << " - " << t.type << " "
                << currencySymbol << t.amount;
            if (t.currency != "GBP") cout << " (" << t.currency << ")";
            cout << endl;
        }
    }

    bool VerifyPin() {
        int enteredPin;
        cout << "Enter pin: ";
        cin >> enteredPin;
        if (enteredPin == pin) {
            return true;
        }
        else {
            cout << "Incorrect pin.\n";
            return false;
        }
    }
};

//phase 2.1 creating class for savings
class Savings {//class for savings
private:
    int saccountNumber;
    int ssortCode;
    double sbalance;
    double interestRate = 0.02; //2% intrest rate from trasnfering into savings

    vector<Transaction> transactions;

    void logTransaction(string type, double amount) {
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        t.currency = "GBP";  // Savings only in GBP
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
        if (interest > 0) {
            sbalance += interest;
            saveToFile();
            logTransaction("INTEREST APPLIED", interest);  // Track interest separately
        }
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
    void deposit(double amount, string transType = "DEPOSIT") {
        sbalance += amount;
        applyInterest();
        saveToFile();//overites data to show new balace
        logTransaction(transType, amount);
        cout << "Savings deposit successful.\n";
    }

    void withdraw(double amount, string transType = "WITHDRAWAL") {
        if (amount > sbalance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
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
            cout << t.timestamp << " - " << t.type << " £" << t.amount;
            if (t.type == "INTEREST APPLIED") cout << " (Auto)";
            cout << endl;
        }
    }
};

//now creating main menu
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
        cout << "8. Transfer from Savings\n";
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
                // PIN verification fixed: removed semicolon after VerifyPin()
                if (user.VerifyPin()) {
                    double amount;
                    cout << "Transfer amount to savings: £";
                    cin >> amount;
                    if (amount <= user.getBalance()) {
                        double netAmount = round((amount * 0.98) * 100) / 100; // 2% fee for transfer whilst keeping digits
                        user.withdraw(amount, "TRANSFER TO SAVINGS");
                        usersavings.deposit(amount, "TRANSFER FROM MAIN");
                    }
                    else {
                        cout << "Insufficient funds for transfer.\n";
                    }
                }
            }
            else {
                usersavings.createSavingsAccount();
                hasSavings = true;
            }
            break;
        }

        case 8: {  // Transfer from Savings with PIN check
            if (hasSavings) {
                // PIN verification fixed: removed semicolon after VerifyPin()
                if (user.VerifyPin()) {
                    double amount;
                    cout << "Transfer amount to main account: £";
                    cin >> amount;
                    if (amount <= usersavings.getBalance()) {
                        double netAmount = round((amount * 0.98) * 100) / 100; // 2% fee for savings to main too
                        usersavings.withdraw(amount, "TRANSFER TO MAIN");
                        user.deposit(amount, "GBP", "TRANSFER FROM SAVINGS");
                    }
                    else {
                        cout << "Insufficient funds in savings.\n";
                    }
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
            usersavings.showTransactionHistory();
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

            string currencies[] = { "USD", "EUR" }; // removed GBP
            user.deposit(amount, currencies[currencyChoice - 1]);
            break;
        }

        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (true);

    return 0;
}