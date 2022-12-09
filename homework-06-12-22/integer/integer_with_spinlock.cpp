#include "integer_with_spinlock.h"

IntegerWithSpinlock::IntegerWithSpinlock(int value) : AtomicInteger(value),
                                                      spinlock_{std::make_unique<pthread_spinlock_t>()} {
    if (pthread_spin_init(spinlock_.get(), 0) != 0) {
        throw std::runtime_error("Spinlock initialization failed!");
    }
}

void IntegerWithSpinlock::Inc() {
    pthread_spin_lock(spinlock_.get());
    value_++;
    pthread_spin_unlock(spinlock_.get());
}

IntegerWithSpinlock::~IntegerWithSpinlock() {
    pthread_spin_destroy(spinlock_.get());
}
