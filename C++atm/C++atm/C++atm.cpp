#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Account {
private:
	int accountNumber;
	int pin;
	double sortCode;
	double balance;

public:
	void saveToFile() {
		ofstream file("accounts.txt") {}
	}
	void createAccount() {
		cout << "Enter your account number: ";
		cin >> accountNumber;
		cout << "Enter your sort code: ";
		cin >> sortCode;
		cout << "Enter your pin";
		cin >> pin;
		balance = 100;
		saveToFile();
	}
};