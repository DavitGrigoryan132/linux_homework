#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <sys/wait.h>
#include <cstring>
#include <vector>


int main(int argc, char** argv)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        return errno;
    }

    // Child process
    if (pid == 0)
    {
        std::vector<char*> arguments{};
        for(int i = 1; i < argc; ++i)
        {
            arguments.push_back(argv[i]);
        }
        arguments.push_back(NULL);

        execvp(argv[1], arguments.data());
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
        printf("\nFINISH\n");
    }

    return 0;
}
