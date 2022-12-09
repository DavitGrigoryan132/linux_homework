#include "integer_with_mutex.h"

IntegerWithMutex::IntegerWithMutex(int value) : AtomicInteger(value), mutex_{std::make_unique<pthread_mutex_t>()} {
    if (pthread_mutex_init(mutex_.get(), nullptr) != 0) {
        throw std::runtime_error("Mutex initialization failed!");
    }
}

void IntegerWithMutex::Inc() {
    pthread_mutex_lock(mutex_.get());
    value_++;
    pthread_mutex_unlock(mutex_.get());
}

IntegerWithMutex::~IntegerWithMutex() {
    pthread_mutex_destroy(mutex_.get());
}
