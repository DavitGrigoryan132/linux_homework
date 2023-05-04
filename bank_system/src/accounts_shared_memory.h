#pragma once

#include "account.h"

#include <memory>
#include <iostream>
#include <vector>
#include <sys/shm.h>
#include <sys/ipc.h>


class AccountsSharedMemory {
public:
    static void Create(std::size_t number_of_accounts, key_t key = IPC_PRIVATE);

    static void Delete(key_t key = IPC_PRIVATE);
};
