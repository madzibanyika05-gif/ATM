#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

// Phase 1.1: Account Class
class Account {
private:
    int accountNumber;
    int pin;
    int sortCode;
    double balance;
    string currency = "GBP";

    void convertAndDeposit(double amount, string fromCurrency) {
        class CurrencyConverter converter;
        converter.updateRates();
        double converted = converter.convert(amount, fromCurrency, currency);
        balance += converted;
        saveToFile();
        cout << "Deposit successful. Converted amount: £" << converted << "\n";
    }

public:
    double getBalance() { return balance; }

    void saveToFile() {
        ofstream file("accounts.txt", ios::app);
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
        cout << "Current balance: £" << balance << "\n";
    }
};

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
        cout << "Savings balance: £" << sbalance << "\n";
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
        cout << "1. GBP (£)\n";
        cout << "2. USD ($)\n";
        cout << "3. EUR (€)\n";
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
