#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


int check_shmget_error() {
    if (errno == EACCES) {
        std::cerr << "Permission denied" << std::endl;
    }
    else if (errno == ENOENT) {
        std::cerr << "No such shared memory segment" << std::endl;
    }
    return errno;
}

int check_shmat_error() {
    // errno values for shmat is the same as for shmget
    return check_shmget_error();
}


int main() {
    auto key = ftok("key_file.txt", 'A');

    auto shmid = shmget(key, 1024, 0666);
    auto error_number = check_shmget_error();
    if (error_number != 0) {
        return error_number;
    }

    auto data = static_cast<char*>(shmat(shmid, nullptr, SHM_RDONLY));

    // printf("%s\n", data);

    // return 0;

    error_number = check_shmat_error();
    if (error_number != 0) {
        return error_number;
    }
    
    auto iter = data;
    while (*iter != static_cast<char>(0)) {
        while (*iter != '.' and *iter != static_cast<char>(0)) {
            std::cout << *iter;
            iter++;
        }
        if (*iter == static_cast<char>(0)) {
            break;
        }
        iter++;
        sleep(5);
    }
    
    shmdt(data);

    return 0;
}
