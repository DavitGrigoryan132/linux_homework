#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {

    if (fork() == 0) {
        auto parent_pid = getppid();

        kill(parent_pid, SIGKILL);

	printf("Parent process killed\n");
    }
    else {
        sleep(50);
    }

    return 0;
} 
