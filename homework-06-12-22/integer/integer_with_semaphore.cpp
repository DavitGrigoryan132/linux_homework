#include "integer_with_semaphore.h"

IntegerWithSemaphore::IntegerWithSemaphore(const int value) : AtomicInteger{value},
                                                              semaphore_{std::make_unique<sem_t>()} {
    if (sem_init(semaphore_.get(), 0, 1) != 0) {
        throw std::runtime_error("Semaphore initialization failed!");
    }
}

void IntegerWithSemaphore::Inc() {
    sem_wait(semaphore_.get());
    value_++;
    sem_post(semaphore_.get());
}

IntegerWithSemaphore::~IntegerWithSemaphore() {
    sem_destroy(semaphore_.get());
}
