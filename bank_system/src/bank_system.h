
#pragma once

#include "account.h"
#include "semaphores.h"
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <iostream>
#include <vector>

class BankSystem {
public:
    explicit BankSystem(key_t key);

    void PrintCurrentBalance(int id) const;

    void PrintMinBalance(int id) const;

    void PrintMaxBalance(int id) const;

    void Freeze(int id) const;

    void Unfreeze(int id) const;

    void FillAllBalances(int money) const;

    void WithdrawFromAllBalances(int money) const;

    void SetMinBalance(int id, int new_min_balance) const;

    void SetMaxBalance(int id, int new_max_balance) const;

    void Transaction(int source_id, int dest_id, int money) const;

    void Delete() const;

private:
    [[nodiscard]] std::size_t GetSharedMemorySize() const;

    struct AccountsVector {
        std::size_t number_of_accounts_;
        Account* accounts_;
    };

    AccountsVector members_;
    int shared_memory_id_;
};
