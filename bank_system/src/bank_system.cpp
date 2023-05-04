#include "bank_system.h"

BankSystem::BankSystem(key_t key) : members_{ 0, nullptr } {
    shared_memory_id_ = shmget(key, 0, 0666);
    if (shared_memory_id_ == -1) {
        throw std::logic_error("Cannot get shared memory id!");
    }
    members_.number_of_accounts_ = GetSharedMemorySize() / sizeof(Account);
    members_.accounts_ = static_cast<Account*>(shmat(shared_memory_id_, nullptr, 0));
}

void BankSystem::PrintCurrentBalance(int id) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].PrintCurrentBalance();
    }
}

void BankSystem::PrintMinBalance(int id) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].PrintMinBalance();
    }
}

void BankSystem::PrintMaxBalance(int id) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].PrintMaxBalance();
    }
}

void BankSystem::Freeze(int id) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].Freeze();
    }
}

void BankSystem::Unfreeze(int id) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].Unfreeze();
    }
}

void BankSystem::FillAllBalances(int money) const {
    if (members_.accounts_) {
        for (std::size_t id = 0; id < members_.number_of_accounts_; ++id) {
            members_.accounts_[id].Fill(money);
        }
    }
}

void BankSystem::WithdrawFromAllBalances(int money) const {
    if (members_.accounts_) {
        for (std::size_t id = 0; id < members_.number_of_accounts_; ++id) {
            members_.accounts_[id].Withdraw(money);
        }
    }
}

void BankSystem::SetMinBalance(int id, int new_min_balance) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].SetMinBalance(new_min_balance);
    }
}

void BankSystem::SetMaxBalance(int id, int new_max_balance) const {
    if (members_.accounts_ and id < members_.number_of_accounts_) {
        members_.accounts_[id].SetMaxBalance(new_max_balance);
    }
}

void BankSystem::Transaction(int source_id, int dest_id, int money) const {
    if (members_.accounts_ and source_id < members_.number_of_accounts_ and
        dest_id < members_.number_of_accounts_) {
        if (members_.accounts_[source_id].Withdraw(money)) {
            if (not members_.accounts_[dest_id].Fill(money)) {
                members_.accounts_[source_id].Fill(money);
            }
        }
    }
}

std::size_t BankSystem::GetSharedMemorySize() const {
    struct shmid_ds buf{};
    shmctl(shared_memory_id_, IPC_STAT, &buf);
    return buf.shm_segsz;
}

void BankSystem::Delete() const {
    for (std::size_t i = 0; i < members_.number_of_accounts_; ++i) {
        members_.accounts_[i].Delete();
    }
}
