#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

class SavingsAccount;

class Account {
  private:
    string number;
    string holderName;
    double balance;

    string generateAccountNumber(vector<Account> accounts) {
        int maxNumber = 0;
        for (auto account : accounts) {
            int currentNumber = stoi(account.number);
            if (currentNumber > maxNumber) {
                maxNumber = currentNumber;
            }
        }

        int newNumber = maxNumber + 1;
        stringstream ss;
        ss << setw(10) << setfill('0') << newNumber;

        return ss.str();
    }

  public:
    Account(string number, string holderName, double balance) {
        this->number = number;
        this->holderName = holderName;
        this->balance = balance;
    }
    Account(vector<Account> accounts, string holderName, double balance) {
        number = generateAccountNumber(accounts);
        this->holderName = holderName;
        this->balance = balance;
    }

    friend void addInterest(SavingsAccount getAccount);

    string getAccountNumber() {
        return number;
    }

    string getHolderName() {
        return holderName;
    }

    double getBalance() {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Depositing $" << amount << "..." << endl;
    }

    void withdraw(double amount) {
        balance -= amount;
        cout << "Withdrawing $" << amount << "..." << endl;
    }

    friend ostream& operator<<(ostream& mycout, Account& account);

    template <typename T>
    void transfer(T& account, double amount) {
        this->balance -= amount;
        account.balance += amount;
        cout << "Transferring $" << amount << " to " << account.holderName << "..." << endl;
    }
};

class SavingsAccount : public Account {
  private:
    double interestRate;
  public:
    SavingsAccount(string setAccountNumber, string setAccountHolderName, double setBalance, double interestRate)
        : Account(setAccountNumber, setAccountHolderName, setBalance) {
        this->interestRate = interestRate;
    }
    friend void addInterest(SavingsAccount getAccount);
};

ostream& operator<<(ostream& mycout, Account& account) {
    mycout << endl << "Account Number: " << account.getAccountNumber() << endl
           << "Account Holder Name: " << account.getHolderName() << endl
           << "Balance: $" << account.getBalance() << endl;
    return mycout;
}

void addInterest(SavingsAccount getAccount) {
    getAccount.deposit(getAccount.getBalance() * (getAccount.interestRate / 100));
}

void readFile(vector<Account>& accounts) {
    accounts.clear();

    ifstream fileIn("database.csv");
    if (fileIn.is_open()) {
        string line;
        while (getline(fileIn, line)) {
            stringstream ss(line);
            string number, name;
            double balance;
            if (getline(ss, number, ',') && getline(ss, name, ',') && ss >> balance)
                accounts.push_back(Account(number, name, balance));
        }
        fileIn.close();
    } else {
        ofstream newFile("database.csv");
        if (newFile.is_open()) {
            newFile.close();
        } else {
            cerr << "Unable to open or create file";
        }
    }
        
}

void saveToFile(vector<Account>& accounts) {
    ofstream fileOut("database.csv");
    if (fileOut.is_open()) {
        for (auto account : accounts) {
            fileOut << account.getAccountNumber() << ","
                    << account.getHolderName() << ","
                    << account.getBalance() << endl;
        }
        fileOut.close();
    } else
        cerr << "Unable to open file";
}

void menu() {
    vector<Account> accounts;
    int menuOption;
    bool running = true;

    while (running) {
        cout << "-----------------------------------------------------\n"
             << "\t\t\t\tWelcome to VM Bank"
             << "\n\n\tTo choose a function, enter its number label:\n\n"
             << "1) Create a new account\n"
             << "2) View account statement\n"
             << "3) Deposit funds\n"
             << "4) Withdraw funds\n"
             << "5) Transfer funds\n"
             << "6) Quit\n"
             << "-----------------------------------------------------\n"
             << "\n\n\t\t\tPlease enter your selection: ";
        cin >> menuOption;

        switch (menuOption) {
            case 1: {
                cout << "Please enter your full name: ";
                string holderName;
                cin >> ws;
                getline(cin, holderName);

                readFile(accounts);

                double initialBalance;
                cout << "Please enter initial balance: ";
                cin >> initialBalance;
                Account newAccount(accounts, holderName, initialBalance);
                accounts.push_back(newAccount);

                cout << newAccount;
                saveToFile(accounts);
                break;
            }
            case 2: {
                cout << "Please enter your Account Number: ";
                string searchNumber;
                cin >> searchNumber;
                readFile(accounts);

                bool found = false;
                for (auto account : accounts) {
                    if (account.getAccountNumber() == searchNumber) {
                        cout << account;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Invalid Account Number" << endl;
                }
                break;
            }
            case 3: {
                cout << "Please enter your Account Number: ";
                string searchNumber;
                cin >> searchNumber;
                cout << "Please enter the amount to deposit: ";
                double depositAmount;
                cin >> depositAmount;

                readFile(accounts);

                bool found = false;
                for (auto& account : accounts) {
                    if (account.getAccountNumber() == searchNumber) {
                        account.deposit(depositAmount);
                        cout << account;
                        saveToFile(accounts);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Invalid Account Number" << endl;
                }
                break;
            }
            case 4: {
                cout << "Please enter your Account Number: ";
                string searchNumber;
                cin >> searchNumber;
                cout << "Please enter the amount to Withdraw: ";
                double withdrawAmount;
                cin >> withdrawAmount;

                readFile(accounts);

                bool found = false;
                for (auto& account : accounts) {
                    if (account.getAccountNumber() == searchNumber) {
                        account.withdraw(withdrawAmount);
                        cout << account;
                        saveToFile(accounts);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Invalid Account Number" << endl;
                }
                break;
            }
            case 5: {
              cout << "Please enter your Account Number: ";
              string searchSender;
              cin >> searchSender;
              cout << "Enter the Account Number to transfer to: ";
              string searchReceiver;
              cin >> searchReceiver;
              cout << "Please enter the amount to Transfer: ";
              double transferAmount;
              cin >> transferAmount;

              readFile(accounts);

              bool foundSender = false;
              bool foundReceiver = false;
              for (auto &accountReceiver : accounts){
                  if (accountReceiver.getAccountNumber() == searchReceiver){
                      foundReceiver = true;
                      for (auto &accountSender : accounts){
                          if (accountSender.getAccountNumber() == searchSender){
                              accountSender.transfer(accountReceiver, transferAmount);
                              cout << accountSender;
                              foundSender = true;
                              saveToFile(accounts);
                              break;
                          }
                      }
                  }
              }
              if (!foundSender){
                  cout << "Invalid Sender Account Number" << endl;
              }
              if (!foundReceiver){
                  cout << "Invalid Receiver Account Number" << endl;
              }
              break; 
            }
            case 6: {
                cout << endl << "Thank you for choosing VM Bank" << endl;
                running = false;
                break;
            }
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }
}

int main() {
    menu();
    return 0;
}
