#pragma once

#include "atomic_integer.h"

#include <pthread.h>
#include <memory>

class IntegerWithSpinlock : public AtomicInteger {
public:
    explicit IntegerWithSpinlock(int value);

    void Inc() override;
    ~IntegerWithSpinlock() override;

private:
    std::unique_ptr<pthread_spinlock_t> spinlock_;
};
