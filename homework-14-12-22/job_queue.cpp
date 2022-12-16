#include "job_queue.h"

JobQueue::JobQueue() : mutex_ PTHREAD_MUTEX_INITIALIZER, condition_variable_ PTHREAD_COND_INITIALIZER {}

JobQueue::~JobQueue() {
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&condition_variable_);
}

void JobQueue::Push(pthread_func_t item) {
    pthread_mutex_lock(&mutex_);
    queue_.push(item);
    pthread_cond_signal(&condition_variable_);
    pthread_mutex_unlock(&mutex_);
}

pthread_func_t JobQueue::Pop() {
    pthread_mutex_lock(&mutex_);
    while (queue_.empty()) {
        pthread_cond_wait(&condition_variable_, &mutex_);
    }

    auto front_item = queue_.front();
    queue_.pop();
    pthread_mutex_unlock(&mutex_);

    return front_item;
}

std::size_t JobQueue::Size() {
    pthread_mutex_lock(&mutex_);
    auto size = queue_.size();
    pthread_mutex_unlock(&mutex_);
    return size;
}
