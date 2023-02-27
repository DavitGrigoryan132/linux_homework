#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <cstring>


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

    auto data = static_cast<char*>(shmat(shmid, nullptr, 0));
    error_number = check_shmat_error();
    if (error_number != 0) {
        return error_number;
    }

    std::cout << "Write data: (write q to exit) ";
    
    std::string input_word{};
    std::string input_string{};

    std::size_t string_size = 0;
    while (std::cin >> input_word) {
        if (input_word == "q") {
            break;
        }
        if (string_size >= 1024) {
            std::cout << "Not enough memory" << std::endl;
            break;
        }
        else {
            if (string_size == 0) {
                input_string += input_word;
                string_size += input_word.size();
            }
            else {
                input_string += ("." + input_word);
                string_size += input_word.size() + 1;
            }
        }
    }
    input_string += static_cast<char>(0);
    string_size++;

    memcpy(data, input_string.c_str(), string_size);

    std::cout << "Data written in memory" << std::endl;
    
    shmdt(data);

    return 0;
}
