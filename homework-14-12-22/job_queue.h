#pragma once

#include <pthread.h>
#include <memory>
#include <queue>
#include <functional>

typedef void* (*pthread_func_t)(void*);

class JobQueue {
public:
    JobQueue();
    ~JobQueue();

    void Push(pthread_func_t item);
    pthread_func_t Pop();
    std::size_t Size();

private:
    std::queue<pthread_func_t> queue_{};
    pthread_mutex_t mutex_;
    pthread_cond_t condition_variable_;
};
