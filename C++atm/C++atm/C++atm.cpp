<<<<<<< HEAD
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cpprest/http_client.h>
=======
#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations
#include <sstream> // for string streams
<<<<<<< HEAD
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
=======
#include <cpprest/http_client.h> // for API to be called
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
#include <cpprest/json.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;
<<<<<<< HEAD

// Phase 1.1: Account Class
class Account {
=======
>>>>>>> 2478b1a80bf0929a1dced15ca51e431d98b5f1e7
//phase 1.1 adding account class
class Account {// class for creating an account and holding account details
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
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
<<<<<<< HEAD
        cout << "Deposit successful. Converted amount: Â£" << converted << "\n";
=======
        cout << "Deposit successful. Converted amount: £" << converted << "\n";
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
    }

    vector<Transaction> transactions;

    void logTransaction(string type, double amount) {
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        transactions.push_back(t);

        ofstream log("account_transactions.txt", ios::app);// saving transactions to files
        if (log.is_open()) {
            log << t.timestamp << "|" << type << "|£" << amount << endl;
        }
    }

public:
    double getBalance() { return balance; }

<<<<<<< HEAD
    void saveToFile() {
        ofstream file("accounts.txt", ios::app);
        if (file.is_open()) {
=======
    void saveToFile() {// function to save account details to file
        ofstream file("accounts.txt", ios::trunc);  // ios::app = append mode this adds to file instead of overwriting//changed to trunc to overwrite
        if (file.is_open()) {//checks if file opens successfully and writes account number etc...
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
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

<<<<<<< HEAD
    void createAccount() {
        cout << "Enter your account number: ";
=======
    void createAccount() {//function to create account by getting user info
        cout << "Enter your account number: ";// questions
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
        cin >> accountNumber;
        cout << "Enter your sort code: ";
        cin >> sortCode;
        cout << "Enter your pin: ";
        cin >> pin;
<<<<<<< HEAD
        balance = 1000.00;
=======
        balance = 1000.00; //startinh balance of 1k
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
        saveToFile();
        cout << "Account created successfully.\n";
    }

    void deposit(double amount, string fromCurrency = "GBP") {
        if (fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
        }
        else {
            balance += amount;
            saveToFile();
            cout << "Deposit successful.\n";
        }
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds.\n";
        }
        else {
            balance -= amount;
            saveToFile();
            cout << "Withdrawal successful.\n";
        }
    }

    void checkBalance() {
        cout << "Current balance: Â£" << balance << "\n";
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
    void deposit(double amount, string fromCurrency = "GBP") {//trasaction function being created
        if (fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
            logTransaction("FOREIGN DEPOSIT", amount);
        }
        else {
            balance += amount;
            saveToFile();//overites data to show new balace
            logTransaction("DEPOSIT", amount);
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
            logTransaction("WITHDRAWAL", amount);
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
        transactions.push_back(t);

        ofstream log("savings_transaction.txt", ios::app);
        if (log.is_open()) {
            log << t.timestamp << "|" << type << "| £" << amount << endl;
        }
    }

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
    void deposit(double amount) {//trasaction function being created
        sbalance += amount;
        applyInterest();
        saveToFile();//overites data to show new balace
        logTransaction("DEPOSIT", amount);
        cout << "Savings deposit successful.\n";
    }

    void withdraw(double amount) {//function for withdraws
        if (amount > sbalance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
            sbalance -= amount;
            applyInterest();
            saveToFile();
            logTransaction("WITHDRAWAL", amount);
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
        cout << "8. Check savings balance\n";
        cout << "9. Exit\n";
    }
    else {
        cout << "7. Create savings account\n";
        cout << "8. Exit\n";
    }
}

// Phase 2.1: Savings Class
class Savings {
private:
    int saccountNumber;
    int ssortCode;
    double sbalance;
    double interestRate = 0.02;

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
        cout << "Interest applied: Â£" << sbalance << "\n";
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

    void deposit(double amount) {
        sbalance += amount;
        saveToFile();
        cout << "Savings deposit successful.\n";
    }

    void withdraw(double amount) {
        if (amount > sbalance) {
            cout << "Insufficient funds.\n";
        }
        else {
            sbalance -= amount;
            saveToFile();
            cout << "Savings withdrawal successful.\n";
        }
    }

    void checkBalance() {
        cout << "Savings balance: Â£" << sbalance << "\n";
    }
};

// Phase 2.2: Currency Converter
class CurrencyConverter {
private:
    const string apiKey = "f78cc7fbb42a99760e7657a1";
    double gbpToUsd = 1.27;
    double gbpToEur = 1.17;

public:
    void showCurrencyMenu() {
        cout << "\n-=-=-=- Select currency -=-=-=-\n";
        cout << "1. GBP (Â£)\n";
        cout << "2. USD ($)\n";
        cout << "3. EUR (â‚¬)\n";
        cout << "Choice: ";
    }

    void updateRates() {
        http_client client(U("https://v6.exchangerate-api.com/v6/"));
        uri_builder builder(U(apiKey + "/latest/GBP"));

        client.request(methods::GET, builder.to_string())
            .then([](http_response response) {
            return response.extract_json();
                })
            .then([&](json::value json) {
            if (json.has_field(U("conversion_rates"))) {
                auto rates = json[U("conversion_rates")];
                gbpToUsd = rates[U("USD")].as_double();
                gbpToEur = rates[U("EUR")].as_double();
            }
                }).wait();
    }

    double convert(double amount, string from, string to) {
        if (from == "GBP" && to == "USD") return amount * gbpToUsd;
        if (from == "GBP" && to == "EUR") return amount * gbpToEur;
        if (from == "USD" && to == "GBP") return amount / gbpToUsd;
        if (from == "EUR" && to == "GBP") return amount / gbpToEur;
        return amount;
    }
};

// Main Menu
void showMainMenu(bool hasSavings) {
    cout << "\n-=-=-=- Haven ATM, Main Menu -=-=-=-\n";
    cout << "1. Check balance\n";
    cout << "2. Deposit money\n";
    cout << "3. Withdraw money\n";
    cout << "7. Deposit foreign currency\n";
    if (hasSavings) {
        cout << "4. Transfer to Savings\n";
        cout << "5. Check savings balance\n";
        cout << "6. Exit\n";
    }
    else {
        cout << "4. Create savings account\n";
        cout << "5. Exit\n";
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
<<<<<<< HEAD
            cout << "Enter deposit amount: Â£";
=======
            cout << "Enter deposit amount: £";
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
            cin >> amount;
            user.deposit(amount);
            break;
        }

        case 3: {
            double amount;
<<<<<<< HEAD
            cout << "Enter withdrawal amount: Â£";
=======
            cout << "Enter withdrawal amount: £";
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
            cin >> amount;
            user.withdraw(amount);
            break;
        }

        case 7: {
            if (hasSavings) {
                double amount;
<<<<<<< HEAD
                cout << "Transfer amount: Â£";
=======
                cout << "Transfer amount: £";
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
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
            user.resetPIN();
            break;
        }

        case 6: {
            user.showTransactionHistory();
            break;
        }

        case 8: {
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

        case 9: {
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
<<<<<<< HEAD
}
=======
}
>>>>>>> b5ae3032846c74fced9dd2b1ad0a29b8c48b3446
