#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <array>



static const int BUF_SIZE = 4;

struct Buffer {
    void put_item(int value) {
        data_[write_index_] = value;
        write_index_ = (write_index_ + 1) % data_.size();
    }

    int get_item() {
        int res = data_[read_index_];
        read_index_ = (read_index_ + 1) % data_.size();
        return res;
    }

private:
    int write_index_{0};
    int read_index_{0};
    int shm_id_;
    std::array<int, BUF_SIZE> data_{};
};

key_t get_token() {
    auto token = ftok("ftok.txt", 7);
    if (token == -1) {
        throw std::invalid_argument{"ftok"};
    }
    return token;
}

int create_shared_memory(key_t token) {
    int shmid = shmget(token, BUF_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        throw std::logic_error{"shmget"};
    }
    return shmid;
}


int create_semaphore(key_t token) {
    int sem_id = semget(token, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (sem_id == -1) {
        throw std::logic_error{"semget"};
    }

    return sem_id;
}

void delete_semaphore(int sem_id) {
    auto ret_val = semctl(sem_id, 00, IPC_RMID);
    if (ret_val == -1) {
        std::logic_error("delete_semaphore");
    }
}

void inc_sem(int sem_id) {
    struct sembuf sop{
        .sem_num = 0,
        .sem_op = 1,
        .sem_flg = 0
    };
    if (semop(sem_id, &sop, 1) == -1) {
        std::cout << errno << "\n";
        throw std::logic_error{"inc_sem"};
    }
}

void dec_sem(int sem_id) {
    struct sembuf sop{
        .sem_num = 0,
        .sem_op = -1,
        .sem_flg = 0
    };
    if (semop(sem_id, &sop, 1) == -1) {
        throw std::logic_error{"dec_sem"};
    }
}


int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::invalid_argument("Must have 2 arguments");
    }

    int n_producers_number = std::atoi(argv[1]);
    int m_consumers_number = std::atoi(argv[2]);
    
    auto token = get_token();
   
    int shmid = create_shared_memory(token);
  
    Buffer* buffer = (Buffer*) shmat(shmid, nullptr, 0);
    
    auto sem_id = create_semaphore(token);
    inc_sem(sem_id);

    pid_t initial_pid = getpid();

    for (std::size_t i = 0; i < n_producers_number; ++i) {
        if (fork() == 0) {
            // Producer code
            dec_sem(sem_id);
            buffer->put_item(getpid() - initial_pid);
            inc_sem(sem_id);
            break;
        }
    }

    if (getpid() == initial_pid){
        for (std::size_t i = 0; i < m_consumers_number; ++i) {
            if (fork() == 0) {
                // Consumer code
                dec_sem(sem_id);
                std::cout << buffer->get_item() << std::endl;
                inc_sem(sem_id);
                break;
            }
        }
    }

    if (getpid() == initial_pid) {
        while(wait(nullptr) > 0);
        delete_semaphore(sem_id);
    
        shmdt(buffer);
        shmctl(shmid, IPC_RMID, nullptr);
    }

    return 0;
}
