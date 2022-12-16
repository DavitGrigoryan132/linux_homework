#include <cstring>

#include "worker.h"


Worker::Worker(const std::size_t num_threads) : threads_(num_threads) {
    if (num_threads == 0) {
        throw std::runtime_error("Threads number must be greater than 0!");
    }
}

Worker::~Worker() {
    for (auto &thread: threads_) {
        if (thread) {
            auto err = pthread_join(thread, nullptr);
            if (err != 0) {
                printf("Error joining thread. errno = %d: %s\n", err,
                       strerror(err));
                exit(1);
            }
        }
    }
}

void Worker::Add(pthread_func_t item) {
    queue_.Push(item);
}

void Worker::Run() {
    while (queue_.Size() > 0) {
        auto size = static_cast<int>(std::min(queue_.Size(), threads_.size()));
        for (auto i = size - 1; i >= 0; --i) {
            auto task = queue_.Pop();
            if (threads_[i]) {
                auto err = pthread_join(threads_[i], nullptr);
                if (err != 0) {
                    printf("Error joining thread. errno = %d: %s\n", err,
                           strerror(err));
                    exit(1);
                }
            }
            auto err = pthread_create(&threads_[i], nullptr, task, nullptr);
            if (err != 0) {
                printf("Error creating thread. errno = %d: %s\n", err,
                       strerror(err));
                exit(1);
            }
        }
    }
}

//auto Worker::PopFunctionFromQueue() -> void *(*)(void *) {
//    return *queue_.Pop()->target<void *(*)(void *)>();
//}
