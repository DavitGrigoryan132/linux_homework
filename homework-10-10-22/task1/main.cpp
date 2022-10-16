#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <sys/wait.h>


int main(int argc, char** argv)
{
    for(int i = 1; i < argc; ++i)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            return errno;
        }

        // Child process
        if (pid == 0)
        {
            execlp(argv[i], argv[i], NULL);
        }

        // Parent process
        if (pid > 0)
        {
            int childStatus = -1;
            pid_t waitingChild = waitpid(pid, &childStatus, 0);
            if(waitingChild == -1)
            {
                return errno;
            }
        }
    }
    return 0;
}
