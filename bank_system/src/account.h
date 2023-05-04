#pragma once

#include "semaphores.h"

#include <iostream>
#include <sys/ipc.h>
#include <sys/types.h>

class Account {
public:
    Account(int id, key_t key);

    void PrintCurrentBalance() const;

    void PrintMinBalance() const;

    void PrintMaxBalance() const;

    void Freeze();

    void Unfreeze();

    bool Withdraw(int x);

    bool Fill(int x);

    void SetMinBalance(int new_min_balance);

    void SetMaxBalance(int new_max_balance);

    void Delete();

private:
    int id_;

    Semaphores sems_;
    short unsigned int sem_index_{0};

    int current_balance_{ 0 };
    int min_balance_{ 0 };
    int max_balance_{ 1000000 };

    bool freezed_{ false };
};
