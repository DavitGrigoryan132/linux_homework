#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <string>

class Semaphores {
public:
    explicit Semaphores(key_t key = IPC_PRIVATE, int number = 1);
    
    void Lock(short unsigned int index) const;

    void Unlock(short unsigned int index) const;

    void Delete() const;

private:
    int number_of_semaphores_;
    key_t key_;
    int id_;
};
