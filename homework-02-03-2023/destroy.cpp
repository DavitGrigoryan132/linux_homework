#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>
#include <errno.h>


int check_shmget_error() {
    if (errno == EACCES) {
        std::cerr << "Permission denied" << std::endl;
    }
    else if (errno == ENOENT) {
        std::cerr << "No such shared memory segment" << std::endl;
    }
    return errno;
}


int check_shctl_error() {
    if (errno == EACCES) {
        std::cerr << "Permission denied" << std::endl;
    }
    else if (errno == EFAULT) {
        std::cerr << "The address used for an argument is not correct" << std::endl;
    }
    else if (errno == EPERM) {
        std::cerr << "Operation nit permitted" << std::endl;
    }

    return errno;
}


int main() {
    auto key = ftok("key_file.txt", 'A');

    auto shmid = shmget(key, 1024, 0666);
    auto error_number = check_shmget_error();
    if (error_number != 0) {
        return error_number;
    }

    shmctl(shmid, IPC_RMID, nullptr);
    
    error_number = check_shctl_error();
    if (error_number != 0) {
        return error_number;
    }
    else {
        std::cout << "Shared memory segment removed!" << std::endl;
    }
    remove("key_file.txt");    
}
