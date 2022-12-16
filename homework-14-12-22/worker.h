#pragma once

#include "job_queue.h"

#include <iostream>
#include <pthread.h>
#include <vector>
#include <functional>

class Worker {
public:
    explicit Worker(std::size_t num_threads);
    ~Worker();

    void Add(pthread_func_t item);
    void Run();

private:
//    auto PopFunctionFromQueue() -> void*(*)(void*);

    std::vector<pthread_t> threads_;
    JobQueue queue_;
};
