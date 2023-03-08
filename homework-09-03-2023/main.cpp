#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <vector>


void remove_semaphore(int semid) {
    auto retval = semctl(semid, 0, IPC_RMID);
    if (retval == -1) {
	    std::cerr << "Remove Semaphore: Semaphore CTL: " << std::endl;
    }
}

int create_semaphore() {
    int semid;
    struct sembuf sbuf;

    /* Get semaphore ID associated with this key. */
    if ((semid = semget(IPC_PRIVATE, 0, 0)) == -1) {

        /* Semaphore does not exist - Create. */
        if ((semid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666)) != -1) {
            /* Initialize the semaphore. */
            sbuf.sem_num = 0;
            sbuf.sem_op = 0;
            sbuf.sem_flg = 0;
            if (semop(semid, &sbuf, 1) == -1) {
		std::cerr << "IPC error: semop" << std::endl;
                exit(1);
            }
        }
        else if (errno == EEXIST) {
            if ((semid = semget(IPC_PRIVATE, 0, 0)) == -1) {
	        std::cerr <<"IPC error 1: semget" << std::endl;
                exit(1);
            }
        }
        else {
	    std::cerr << "IPC error 2: semget" << std::endl;
            exit(1);
        }
    }

    return semid;
}


int main(int argc, char** argv) {
    if (argc != 2) {
	std::cerr << "Arguments error" << std::endl;
	exit(1);
    }

    const auto semid = create_semaphore();
    pid_t pid;
    const int NUMBER_OF_PROCESSES = atoi(argv[1]);
    pid_t parent_pid = getpid();
    for (int i = 0; i < NUMBER_OF_PROCESSES; ++i) {
        pid = fork();
        if (pid == 0) {
            int index = getpid() - parent_pid;
            struct sembuf sbuf;
            sbuf.sem_num = 0;
            sbuf.sem_op = -(index - 1);
            sbuf.sem_flg = 0;
	    if (semop(semid, &sbuf, 1) == -1) {
		std::cerr << "IPC error: semop 2" << std::endl;
                exit(1);
            }
            
            std::cout << " index: " << index << "\n";

            sbuf.sem_num = 0;
            sbuf.sem_op = index;
            sbuf.sem_flg = 0;
            if (semop(semid, &sbuf, 1) == -1) {
		std::cerr << "2 IPC error: semop 3" << std::endl;
                exit(1);
            }
            break;
        }
    }
    
    if (pid > 0) {
        while (wait(nullptr) > 0);
        remove_semaphore(semid);
    }
    return 0;
}
