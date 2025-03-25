#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// The custom exception classes here:

// Exception for any negative amount dep or withdrawn
// Classes use runtime_error to catch erros and throws back text
class NegativeAmountException : public std::runtime_error {
public:
    NegativeAmountException() : std::runtime_error("Amount cannot be negative!") {}
};

// This exception makes sure that if you withdraw over your balance amount, then it errors out
class OverdrawException : public std::runtime_error {
public:
    OverdrawException() : std::runtime_error("Insufficient funds!") {}
};

// makes it so that it errors out if you try to make a transaction while the account is closed
class InvalidAccountOperationException : public std::runtime_error {
public:
    InvalidAccountOperationException() : std::runtime_error("Account is closed for transactions!") {}
};

// class that holds the account number, balance, and sees if account is closed
class BankAccount {
private:
    std::string accountNumber;
    double balance;
    bool isActive;

public:
    BankAccount(std::string accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance), isActive(true) {}


    void deposit(double amount) {
        // check if account is on and throws exception if closed
        if (!isActive) throw InvalidAccountOperationException();
        //check if neg
        if (amount < 0) throw NegativeAmountException();
        balance += amount;
    }

    void withdraw(double amount) {
        //check if account active
        if (!isActive) throw InvalidAccountOperationException();
        //check if neg
        if (amount < 0) throw NegativeAmountException();
        if (amount > balance) throw OverdrawException();
        balance -= amount;
    }

    // grabs balance  
    double getBalance() const {
        return balance;
    }

    // checks account availability
    void closeAccount() {
        isActive = false;
    }
};

int main() {
    // ask for intial balance and store it in var
    double initialBalance;
    std::cout << "Enter initial balance: ";
    std::cin >> initialBalance;

    // create a pointer and id, helps to stop memory leak
    auto account = std::make_unique<BankAccount>("123456", initialBalance);
    std::cout << "Bank Account Created: #123456\n";

    // this is a loop which closes once the user chooses to exit
    int choice;
    do {
        //menu
        std::cout << "\nMenu:\n";
        std::cout << "1. Deposit\n";
        std::cout << "2. Withdraw\n";
        std::cout << "3. Check Balance\n";
        std::cout << "4. Close Account\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        // if input errors the thing out/ higher than 5 or alphabet it clears, skips loops redisplays
        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(100000, '\n'); 
            std::cout << "Invalid input. Enter a number (1-5) plz.\n";
            continue;
        }

        try { //this just handles the deposit withdraw etc selction of the user
            switch (choice) {
            case 1: {
                // allows user to deposit money and throws exception if it is neg
                double amount;
                std::cout << "Enter deposit amount: ";
                std::cin >> amount;
                account->deposit(amount);
                std::cout << "Depositing $" << amount << "...\n";
                break;
            }
            case 2: {
                // allows user to withdraw money as long as it's not neg or more than what is in the account
                double amount;
                std::cout << "Enter withdrawal amount: ";
                std::cin >> amount;
                account->withdraw(amount);
                std::cout << "Withdrawal $" << amount << "...\n";
                break;
            }
            case 3:
                //shows current bal
                std::cout << "Current Balance: $" << account->getBalance() << "\n";
                break;
            case 4:
                //denies transactions
                account->closeAccount();
                std::cout << "Account closed.\n";
                break;
            case 5:
                //exits
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
            }
        }
        catch (const std::exception& e) { //catches the erros
            std::cerr << "Error: " << e.what() << "\n";
        }

    } while (choice != 5);

    return 0;
}


