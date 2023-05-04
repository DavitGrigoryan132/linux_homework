#include "bank_system.h"
#include "accounts_shared_memory.h"

#include <string>


int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::invalid_argument("Invalid Arguments!");
    }

    key_t key = ftok("/home", 111);

    AccountsSharedMemory::Create(std::atoi(argv[1]), key);

    return 0;
}
