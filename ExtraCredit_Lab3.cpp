#include <iostream>
#include <iomanip>

class Account {
public:
    Account(const std::string& number, const std::string& holder, double balance)
        : accountNumber(number), accountHolder(holder), balance(balance) {}

    virtual ~Account() {}

    void displayDetails() const {
        std::cout << "Account Details for " << getType() << " (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawal successful. $" << amount << " withdrawn." << std::endl;
        } else {
            std::cout << "Insufficient funds. Withdrawal failed." << std::endl;
        }
    }

    virtual Account& operator+(const Account& other) {
        double transferAmount = 300;  // Change this value as needed
        this->withdraw(transferAmount);
        const_cast<Account&>(other).deposit(transferAmount);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "Account Details for " << account.getType() << " (ID: " << account.accountNumber << "):" << std::endl;
        os << "   Holder: " << account.accountHolder << std::endl;
        os << "   Balance: $" << std::fixed << std::setprecision(2) << account.balance << std::endl;
        return os;
    }

    virtual std::string getType() const = 0;

protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;
};

class SavingsAccount : public Account {
public:
    SavingsAccount(const std::string& number, const std::string& holder, double balance, double interestRate)
        : Account(number, holder, balance), interestRate(interestRate) {}

    void withdraw(double amount) override {
        double minBalance = interestRate * 1000;  // Minimum balance based on interest rate
        if (balance - amount >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawal successful. $" << amount << " withdrawn." << std::endl;
        } else {
            std::cout << "Minimum balance requirement not met. Withdrawal failed." << std::endl;
        }
    }

    std::string getType() const override {
        return "Savings Account";
    }

private:
    double interestRate;
};

class CurrentAccount : public Account {
public:
    CurrentAccount(const std::string& number, const std::string& holder, double balance, double overdraftLimit)
        : Account(number, holder, balance), overdraftLimit(overdraftLimit) {}

    void withdraw(double amount) override {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
            std::cout << "Withdrawal successful. $" << amount << " withdrawn." << std::endl;
        } else {
            std::cout << "Overdraft limit exceeded. Withdrawal failed." << std::endl;
        }
    }

    std::string getType() const override {
        return "Current Account";
    }

private:
    double overdraftLimit;
};

int main() {
    Account* savings = new SavingsAccount("S123", "John Doe", 1000, 0.02);
    Account* current = new CurrentAccount("C456", "Jane Doe", 2000, 500);

    savings->displayDetails();
    current->displayDetails();

    savings->deposit(500);
    current->withdraw(1000);

    savings->displayDetails();
    current->displayDetails();

    Account& newCurrent = (*current) + (*savings);

    savings->displayDetails();
    newCurrent.displayDetails();

    delete savings;
    delete current;

    return 0;
}
