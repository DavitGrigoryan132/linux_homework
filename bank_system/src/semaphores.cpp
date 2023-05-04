#include "semaphores.h"

Semaphores::Semaphores(key_t key, int number) : number_of_semaphores_{ number }, key_{ key } {
    id_ = semget(key_, number_of_semaphores_, 0666 | IPC_CREAT | IPC_EXCL);
    if (id_ == -1) {
        throw std::invalid_argument{"Cannot initialize semaphore!"};
    }

    for (int i = 0; i < number_of_semaphores_; ++i) {
        Unlock(i);
    }
}

void Semaphores::Lock(unsigned short index) const {
    if (index >= 0 and index < number_of_semaphores_) {
        struct sembuf sb = {.sem_num = index, .sem_op = -1, .sem_flg = 0};

        semop(id_, &sb, number_of_semaphores_);
    }
    else {
        std::cout << "index is out of range!" << std::endl;
    }
}

void Semaphores::Unlock(unsigned short index) const {
    if (index >= 0 and index < number_of_semaphores_) {
        struct sembuf sb = {.sem_num = index, .sem_op = 1, .sem_flg = 0};

        semop(id_, &sb, number_of_semaphores_);
    }
    else {
        std::cout << "index is out of range!" << std::endl;
    }
}

void Semaphores::Delete() const {
    for (int i = 0; i < number_of_semaphores_; ++i) {
        semctl(id_, i, IPC_RMID);
    }
}
