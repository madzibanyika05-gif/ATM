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
    const double usdToGbp = 1/1.27;
    const double eurToGbp = 1/1.17;

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
    
    void logTransaction(string type, double amount) {
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        transactions.push_back(t);
        
        ofstream log("account_transactions.txt",ios::app);// saving transactions to files
        if (log.is_open()) {
            log << t.timestamp << "|" << type << "|£" << amount << endl;
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
    
    void logTransaction(string type, double amouunt) {
        Transaction t;
        t.type = type;
        t.amount = amount;
        t.timestamp = getCurrentTime();
        transaction.push_back(t);
        
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
        logTransaction("DEPOSIT", amount);
        cout << "Savings deposit successful.\n";
    }

    void withdraw(double amount) {//function for withdraws
        if (amount > sbalance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
            sbalance -= amount;
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
    if (hasSavings) {
        cout << "6. Transfer to Savings\n";
        cout << "7. Check savings balance\n";
        cout << "8. Exit\n";
    }
    else {
        cout << "6. Create savings account\n";
        cout << "7. Exit\n";
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

        case 6: {
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
            user.resetPIN();
            break;
        }
                
        case 7: {
            if (hasSavings) {
                usersavings.checkBalance();
            }
            else {
                cout << "Thank you for banking with Haven ATM.\n";
                return 0;
            }
            break;
            }

        case 8: {
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
