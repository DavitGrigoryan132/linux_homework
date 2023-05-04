#include "account.h"

Account::Account(int id, key_t key): id_{id}, sems_{key, 1}
{}

void Account::PrintCurrentBalance() const {
    std::cout << "Account " << id_ << " Current balance: " << current_balance_ << std::endl;
}

void Account::PrintMinBalance() const {
    std::cout << "Account " << id_ << " Min balance: " << min_balance_ << std::endl;
}

void Account::PrintMaxBalance() const {
    std::cout << "Account " << id_ << " Max balance: " << max_balance_ << std::endl;
}

void Account::Freeze() {
    sems_.Lock(sem_index_);

    freezed_ = true;
    std::cout << "Account " << id_ << " freezed" << std::endl;
    sems_.Unlock(sem_index_);
}

void Account::Unfreeze() {
    sems_.Lock(sem_index_);

    freezed_ = false;
    std::cout << "Account " << id_ << " unfreezed" << std::endl;
    sems_.Unlock(sem_index_);
}

bool Account::Withdraw(int x) {
    bool is_withdrawed = true;

    sems_.Lock(sem_index_);
    if (freezed_) {

        std::cout << "Account " << id_ << " is freezed" << std::endl;

        sems_.Unlock(sem_index_);
        return false;
    }
    if (x > 0 and current_balance_ - x >= min_balance_) {
        current_balance_ -= x;

        std::cout << "Account " << id_ << " withdrawed " << x << std::endl;
    }
    else {
        std::cout << "Account " << id_ << " not enough money" << std::endl;

        is_withdrawed = false;
    }
    sems_.Unlock(sem_index_);

    return is_withdrawed;
}

bool Account::Fill(int x) {
    bool is_filled = true;

    sems_.Lock(sem_index_);
    if (freezed_) {

        std::cout << "Account " << id_ << " is freezed" << std::endl;
        sems_.Unlock(sem_index_);
        return false;
    }
    if (x > 0 and current_balance_ + x <= max_balance_) {
        current_balance_ += x;
        std::cout << "Account " << id_ << " is filled " << x << std::endl;
    }
    else {
        std::cout << "Account " << id_ << " can't be filled" << std::endl;

        is_filled = false;
    }
    sems_.Unlock(sem_index_);

    return is_filled;
}

void Account::SetMinBalance(int new_min_balance) {
    sems_.Lock(sem_index_);
    if (new_min_balance <= current_balance_) {
        min_balance_ = new_min_balance;

        std::cout << "Account " << id_ << " min balance is updated" << std::endl;
    }
    else {
        std::cout << "Account " << id_ << " min balance can't be updated" << std::endl;
    }

    sems_.Unlock(sem_index_);
}

void Account::SetMaxBalance(int new_max_balance) {
    sems_.Lock(sem_index_);
    if (new_max_balance >= current_balance_) {
        max_balance_ = new_max_balance;

        std::cout << "Account " << id_ << " max balance is updated" << std::endl;
    }
    else {
        std::cout << "Account " << id_ << " max balance can't be updated" << std::endl;
    }

    sems_.Unlock(sem_index_);
}

void Account::Delete() {
    sems_.Delete();
}
