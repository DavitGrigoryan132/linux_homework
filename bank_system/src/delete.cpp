#include "bank_system.h"
#include "accounts_shared_memory.h"

#include <string>


int main() {
    key_t key = ftok("/home", 111);

    BankSystem bank_system(key);
    bank_system.Delete();

    AccountsSharedMemory::Delete(key);
    return 0;
}
