#include "accounts_shared_memory.h"

void AccountsSharedMemory::Create(std::size_t number_of_accounts, key_t key) {
    // Create shared memory
    bool initialized = false;
    int shared_memory_id = -1;
    if (not initialized) {
        shared_memory_id = shmget(key, number_of_accounts * sizeof(Account), IPC_CREAT | IPC_EXCL | 0666);
        if (shared_memory_id == -1) {
            throw std::logic_error("Can't create shared memory!");
        }
        initialized = true;
    }

    // If shared memory is initialized fill the memory
    if (initialized) {
        auto accounts = static_cast<Account*>(shmat(shared_memory_id, nullptr, 0));

        for(std::size_t i = 0; i < number_of_accounts; ++i) {
            auto sem_key = ftok("/home", static_cast<int>(123 + i));

            accounts[i] = Account{static_cast<int>(i), sem_key};
        }

        if (accounts) {
            shmdt(accounts);
        }
    }
}

void AccountsSharedMemory::Delete(key_t key) {
    auto shared_memory_id = shmget(key, 0, 0666);
    if (shared_memory_id == -1) {
        throw std::logic_error("Error while getting shared memory id!");
    }
    if (shmctl(shared_memory_id, IPC_RMID, nullptr) == -1) {
        throw std::logic_error("Error while deleting shared memory!");
    }
}
