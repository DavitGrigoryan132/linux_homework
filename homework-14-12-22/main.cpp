#include <iostream>
#include <unistd.h>

#include "worker.h"

pthread_mutex_t mutex;

void *task1(void *) {
    std::cout << "Start task1\n";
    sleep(1);
    std::cout << "Done task1\n";
    return nullptr;
}

void *task2(void *) {
    std::cout << "Start task2\n";
    sleep(2);
    std::cout << "Done task2\n";
    return nullptr;
}

void *task3(void *) {
    std::cout << "Start task3\n";
    sleep(3);
    std::cout << "Done task3\n";
    return nullptr;
}

void *task4(void *) {
    std::cout << "Start task4\n";
    sleep(1);
    std::cout << "Done task4\n";
    return nullptr;
}

void *task5(void *) {
    std::cout << "Start task5\n";
    sleep(2);
    std::cout << "Done task5\n";
    return nullptr;
}

void *task6(void *) {
    std::cout << "Start task6\n";
    sleep(3);
    std::cout << "Done task6\n";
    return nullptr;
}

void *task7(void *) {
    std::cout << "Start task7\n";
    sleep(1);
    std::cout << "Done task7\n";
    return nullptr;
}

int main(int argc, char **argv) {
    std::cout << "Number of threads equal to number of tasks" << std::endl;
    {
        Worker worker{3};
        worker.Add(task1);
        worker.Add(task2);
        worker.Add(task3);

        worker.Run();
    }
    std::cout << "Number of threads greater than number of tasks" << std::endl;
    {
        Worker worker{14};
        worker.Add(task1);
        worker.Add(task2);
        worker.Add(task3);
        worker.Add(task4);
        worker.Add(task5);

        worker.Run();
    }
    std::cout << "Number of threads lower than number of tasks" << std::endl;
    {
        Worker worker{2};
        worker.Add(task1);
        worker.Add(task2);
        worker.Add(task3);
        worker.Add(task4);
        worker.Add(task5);
        worker.Add(task6);
        worker.Add(task7);

        worker.Run();
    }
    return 0;
}

