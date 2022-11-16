#include <iostream>
#include <unistd.h>

int main() {
    int fd_child1[2];
    int fd_child2[2];
    pipe(fd_child1);
    pipe(fd_child2);

    if (fork() == 0) {
        // child process 1
        close(fd_child2[1]);
        close(fd_child1[0]);
        pid_t pid = getpid();
        std::cout << "pid child1: " << pid << "\n";
        write(fd_child1[1], &pid, sizeof(pid_t));
        close(fd_child1[1]);

        pid_t child2_pid;
        read(fd_child2[0], &child2_pid, sizeof(pid_t));
        std::cout << "pid child2 from child1: " << child2_pid << "\n";
	close(fd_child2[0]);
    }
    else if (fork() == 0) {
        // child process 2
        close(fd_child2[0]);
        close(fd_child1[1]);
        pid_t pid = getpid();
        std::cout << "pid child2: " << pid << '\n';
        write(fd_child2[1], &pid, sizeof(pid_t));
        close(fd_child2[1]);

        pid_t child1_pid;
        read(fd_child1[0], &child1_pid, sizeof(pid_t));
        std::cout << "pid child1 from child2: " << child1_pid << "\n";
    	close(fd_child1[0]);
    }
    else {
        close(fd_child1[0]);
        close(fd_child1[1]);
        close(fd_child2[0]);
        close(fd_child2[1]);
    }

    return 0;
}

