#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations
#include <sstream> // for string streams
#include <chrono> // for time stamps for transaction histoy and function
#include <iomanip> // for date and time formatting
#include <cmath> // for round function
#include <limits> // for input validation

using namespace std;

//phase 2.2 creating class for currency converter had to move to the begining
class CurrencyConverter {
private:
    const double gbpToUsd = 1.27; // Conversion rates for currency exchange
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
struct Transaction {// structure to store transaction details
    string type;
    double amount;
    string timestamp;
    string currency;  // Added to track currency type
};

string getCurrentTime() {// time function created
    auto now = chrono::system_clock::now(); // gets current time
    time_t now_time = chrono::system_clock::to_time_t(now); // converts time to calander time
    tm tm_struct;
    localtime_s(&tm_struct, &now_time); // convert to local time
    stringstream ss;
    ss << put_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
    return ss.str(); // return formated string
}

//phase 1.1 adding account class
class Account {// class for creating an account and holding account details
private:
    int accountNumber;
    int pin;
    int sortCode;
    double balance;
    string currency = "GBP"; // Defualt currency 
    string name; // Added name field

    void convertAndDeposit(double amount, string fromCurrency) {// converts $ or € to GBP and deposits
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
                << balance << " "
                << name << "\n"; // Store name with account details
            file.close();
        }
        else {
            cout << "Error saving account!\n";//error if file cant open
        }
    }

    void createAccount() {//function to create account by getting user info
        while (true) {
            cout << "\n-=-=-=- Welcome to Haven ATM account set up -=-=-=-\n";
            cout << "Enter your full name: ";
            cin.ignore(); // Clear input buffer
            getline(cin, name); // Get full name with spaces

            cout << "Enter your account number: ";
            if (!(cin >> accountNumber)) {
                handleInvalidInput();
                continue;
            }

            cout << "Enter your sort code: ";
            if (!(cin >> sortCode)) {
                handleInvalidInput();
                continue;
            }

            cout << "Enter your pin: ";
            if (!(cin >> pin)) {
                handleInvalidInput();
                continue;
            }

            balance = 1000.00; //startinh balance of 1k
            saveToFile();
            cout << "Account created successfully.\n";
            break;
        }
    }

    void resetPIN() {//reset pin function update 3.5
        int oldPIN, newPIN, confirmPIN;
        cout << "Enter old pin: ";
        if (!(cin >> oldPIN)) {
            handleInvalidInput();
            return;
        }

        if (oldPIN != pin) {//Checking if old pin matches pin entered
            cout << "Incorrect pin.\n";
            return;
        }

        cout << "Enter new pin: ";
        if (!(cin >> newPIN)) {
            handleInvalidInput();
            return;
        }

        cout << "Confirm pin: ";
        if (!(cin >> confirmPIN)) {
            handleInvalidInput();
            return;
        }

        if (newPIN != confirmPIN) {// if pins do not have same value
            cout << "Pin dooes not match.\n";
            return;
        }

        pin = newPIN;
        saveToFile();
        cout << "Pin change successful.\n";
    }

    void deposit(double amount, string fromCurrency = "GBP", string transType = "Deposit") {
        if (fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
            logTransaction("Foreign deposit", amount, fromCurrency);  // Log currency
        }
        else {
            balance += amount;
            saveToFile();//overites data to show new balace
            logTransaction(transType, amount);
            cout << "Deposit successful.\n";
        }
    }

    void withdraw(double amount, string transType = "Withdrawal") {
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
        cout << "current balance: £" << balance << "\n"; //displaying current balance
    }

    void showTransactionHistory() {// showing transaction history and applying correnct symbol
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

    bool VerifyPin() {// method to verify pin, this is called when user wants to stransfer too and from savings
        int enteredPin;
        cout << "Enter pin: ";
        if (!(cin >> enteredPin)) {
            handleInvalidInput();
            return false;
        }
        if (enteredPin == pin) {
            return true;
        }
        else {
            cout << "Incorrect pin.\n";
            return false;
        }
    }

private:
    void handleInvalidInput() {// for try again
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please try again.\n";
    }
};

//phase 2.1 creating class for savings
class Savings {//class for savings
private:// storing details
    int saccountNumber;
    int ssortCode;
    double sbalance;

    vector<Transaction> transactions;// to store transaction history for savings account

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

    void createSavingsAccount() {// create new savings account setup menu process
        while (true) {
            cout << "\n-=-=-=- Savings Account Setup -=-=-=-\n ";
            cout << "Enter savings account number: ";
            if (!(cin >> saccountNumber)) {
                handleInvalidInput();
                continue;
            }

            cout << "Enter sort code: ";
            if (!(cin >> ssortCode)) {
                handleInvalidInput();
                continue;
            }

            sbalance = 10.00;
            saveToFile("savings.txt");
            cout << "Savings account created.\n";
            break;
        }
    }

    void saveToFile(string filename = "savings.txt") {// saving account details to file
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << saccountNumber << " "
                << ssortCode << " "
                << sbalance << "\n";
            file.close();// closes file stream
        }
    }

    // depo into savings
    void deposit(double amount, string transType = "Deposit") {
        sbalance += amount;
        saveToFile();//overites data to show new balace
        logTransaction(transType, amount);
        cout << "Savings deposit successful.\n";
    }

    // with from savings
    void withdraw(double amount, string transType = "Withdrawal") {
        if (amount > sbalance) {//calculation to check if withdrawl amount is above balance decilne
            cout << "Insufficient funds.\n";
        }
        else {//anything else allow andf save new balance to file
            sbalance -= amount;
            saveToFile();
            logTransaction(transType, amount);
            cout << "Savings withdrawl successful.\n";
        }
    }

    void checkBalance() {// balance is checked 
        cout << "Savings balance: £" << sbalance << "\n";
    }

    void showTransactionHistory() {// show transaction history for savings account
        cout << "\nSavings Transaction History:\n";
        for (auto& t : transactions) {
            cout << t.timestamp << " - " << t.type << " £" << t.amount << endl;
        }
    }

private:
    void handleInvalidInput() {// for try again 
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again.\n";
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
    if (hasSavings) {// if savings has been created extra options are displayed
        cout << "7. Transfer to Savings\n";
        cout << "8. Transfer from Savings\n";
        cout << "9. Check savings balance\n";
        cout << "10. Exit\n";
    }
    else {
        cout << "7. Create savings account\n"; // if no savings created these are displayed
        cout << "8. Exit\n";
    }
}

// Global input validation handler
bool handleGlobalInputError(int& attempts) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input. Please try again.\n";
    if (++attempts >= 3) {
        cout << "Too many incorrect attempts. Exiting...\n";
        return true;
    }
    return false;
}

int main() {// main porgrem execution and calling
    Account user; // main account object
    Savings usersavings; // savings account object
    CurrencyConverter converter; // currency converter object
    bool hasSavings = false; // for tracking savings account status
    int invalidAttempts = 0; // Track consecutive invalid inputs

    user.createAccount(); // begin with account creation

    int choice; // store users menu selection
    do {
        showMainMenu(hasSavings); // displays menu for user, hassavigs so that it shows the correct options

        if (!(cin >> choice)) {
            if (handleGlobalInputError(invalidAttempts)) return 0;
            continue;
        }

        int maxChoice = hasSavings ? 10 : 8;
        if (choice < 1 || choice > maxChoice) {
            cout << "Invalid option. Please try again.\n";
            if (++invalidAttempts >= 3) {
                cout << "Too many incorrect attempts. Exiting...\n";
                return 0;
            }
            continue;
        }

        invalidAttempts = 0; // Reset counter on valid input

        switch (choice) {
        case 1:
            user.checkBalance();// checks main account balance
            break;

        case 2: {// deposit to main account
            double amount;
            while (true) {
                cout << "Enter deposit amount: £";
                if (!(cin >> amount) || amount <= 0) {
                    if (handleGlobalInputError(invalidAttempts)) return 0;
                    continue;
                }
                break;
            }
            user.deposit(amount);
            break;
        }

        case 3: {// withdraw from main account
            double amount;
            while (true) {
                cout << "Enter withdrawal amount: £";
                if (!(cin >> amount) || amount <= 0) {
                    if (handleGlobalInputError(invalidAttempts)) return 0;
                    continue;
                }
                break;
            }
            user.withdraw(amount);
            break;
        }

        case 7: {// savings transfer or creation
            if (hasSavings) {
                if (user.VerifyPin()) {
                    double amount;
                    while (true) {
                        cout << "Transfer amount to savings: £";
                        if (!(cin >> amount) || amount <= 0) {
                            if (handleGlobalInputError(invalidAttempts)) return 0;
                            continue;
                        }
                        break;
                    }
                    if (amount <= user.getBalance()) {
                        double fee = amount * 0.02;
                        double netAmount = round((amount - fee) * 100) / 100;
                        user.withdraw(amount, "Transfer to savings");
                        usersavings.deposit(netAmount, "Transfer from main"); // fixed typo
                        cout << "Service fee (2%): £" << fee << endl; // fee noti
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
                if (user.VerifyPin()) {
                    double amount;
                    while (true) {
                        cout << "Transfer amount to main account: £";
                        if (!(cin >> amount) || amount <= 0) {
                            if (handleGlobalInputError(invalidAttempts)) return 0;
                            continue;
                        }
                        break;
                    }
                    if (amount <= usersavings.getBalance()) {
                        double fee = amount * 0.02;
                        double netAmount = round((amount - fee) * 100) / 100;
                        usersavings.withdraw(amount, "Transfer to main"); // fixed typo
                        user.deposit(netAmount, "GBP", "Transfer from savings");
                        cout << "Service fee (2%): £" << fee << endl; // fee noti
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

        case 5: {// pin reset function
            user.resetPIN();
            break;
        }

        case 6: {// transaction history function
            user.showTransactionHistory();
            usersavings.showTransactionHistory();
            break;
        }

        case 9: {// savings balance check
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

        case 10: {//Exit program
            if (hasSavings) {
                cout << "Thank you for banking with Haven ATM.\n";
                return 0;
            }
            break;
        }

        case 4: {// deposit foreign currency
            int currencyChoice;
            do {
                converter.showCurrencyMenu();
                if (!(cin >> currencyChoice)) {
                    if (handleGlobalInputError(invalidAttempts)) return 0;
                    continue;
                }
                if (currencyChoice < 1 || currencyChoice > 2) {
                    cout << "Invalid currency choice. Please try again.\n";
                    if (++invalidAttempts >= 3) {
                        cout << "Too many incorrect attempts. Exiting...\n";
                        return 0;
                    }
                    continue;
                }
                break;
            } while (true);

            double amount;
            while (true) {
                cout << "Enter amount: ";
                if (!(cin >> amount) || amount <= 0) {
                    if (handleGlobalInputError(invalidAttempts)) return 0;
                    continue;
                }
                break;
            }

            string currencies[] = { "USD", "EUR" };
            user.deposit(amount, currencies[currencyChoice - 1]);
            break;
        }

        default:
            break;
        }
    } while (true);// infinite loop until exit

    return 0;
}