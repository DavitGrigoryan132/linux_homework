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
    else if (errno == EEXIST) {
        std::cerr << "File exists" << std::endl;
    }
    else if (errno == ENOMEM) {
        std::cerr << "Storage allocation request failed" << std::endl;
    }
    else if (errno == ENOSPC) {
        std::cerr << "No space available" << std::endl;
    }

    return errno;
}


int main() {
    
    auto file_path = "key_file.txt";

    auto fd = creat(file_path, 0666);

    auto key = ftok(file_path, 'A');
    auto shmid = shmget(key, 1024, 0666 | IPC_CREAT | IPC_EXCL);
    
    auto error_number = check_shmget_error();
    if (error_number != 0) {
        return error_number;
    }
    else {
        std::cout << "Shared memory segment initialized!" << std::endl;
    }
    return 0;
}
