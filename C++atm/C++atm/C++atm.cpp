#include <iostream>  // for inputs and outputs to work
#include <string>  // used for string data type
#include <vector> // used for arrays
#include <fstream> // for file operations
#include <sstream> // for string streams
#include <cpprest/http_client.h> // for API to be called

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
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
        converter.updateRates();
        double converted = converter.convert(amount, fromCurrency, currency);
        deposit(converted);
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
        if(fromCurrency != currency) {
            convertAndDeposit(amount, fromCurrency);
            return;
        }else{
        balance += amount;
        saveToFile();//overites data to show new balace
        cout << "Deposit successful.\n";
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
//phase 2.2 creating class for currency converter
class CurrencyConverter
private:
    const string string apiKey = "f78cc7fbb42a99760e7657a1"; //API key
    double gbpToUsd = 1.27; // backup average exchange rate if api fails
    double gbpToEur = 1.17;
    double usdToEur = 0.86;
    
    public:
    //menu for currency converter
        void showCurrencyMenu() {
        cout << "\n-=-=-=- Select currency -=-=-=-\n";
        cout << "1. GBP (£)\n";
        cout << "2. USD ($)\n";
        cout << "3. EUR (€)\n";
        cout << "Choice: ";
        }

    void updateRates() {
        http_client client(U("https://v6.exchangerate-api.com/v6/f78cc7fbb42a99760e7657a1/latest/USD"));
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
    return amount; // Fallback
    }
};

//now creating main menu
void showMainMenu(bool hasSavings) {
    cout << "\n-=-=-=- Haven ATM, Main Menu -=-=-=-\n";
    cout << "1. Check balance\n";
    cout << "2. Deposit money\n";
    cout << "3. Withdraw money\n";
    cout << "7. Deposit foreign currency\n"
    cout << "4: ";
    if (hasSavings) {
        cout << "4. Transfer to Savings\n";
        cout << "5. Check savings balance\n";
        cout << "6. Exit\n";
        cout << "Exit\n";
    }
    else {
        cout << "4. Create savings account\n";
        cout << "5. Exit\n";
    }
}
int main() { //object called user under class account
    Account user;
    Savings usersavings;
    bool hasSavings = false;
    user.createAccount();

    int choice;
    do {//start of loop
        showMainMenu(hasSavings);
        cin >> choice;

        switch (choice) {
        case 1: {//goes to chek user option 1
            user.checkBalance();//calls the checkBalance function to display balance
            break;//exists switch from itterationa and goes back to main menu
        }
        case 2: {
            double amount;
            cout << "Enter deposit amount: £";
            cin >> amount;
            user.deposit(amount);
            break;
        }
        case 3: {
            double amount;
            cout << "Enter withdrawl amount: £";
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
            if (hasSavings)
                cout << "Thank you for banking with Haven ATM.\n";
        }
        case 7: {
            double amount;
            int currencyChoice;

            showCurrencyMenu()
                cin >> currencyChioce;

            cout << "Enter amount: ";
            cin >> amount;

            string currencies[] = { "GBP", "USD", "EUR" };
            user.deposit(amount, currencies[currencyChoice - 1]);
            break;
        }
        default: {
            cout << "Invalid option. Please try again.\n";

        }
        }
    } while (true);

    return 0;
}