#pragma once

#include "atomic_integer.h"

#include <memory>
#include <pthread.h>

class IntegerWithMutex : public AtomicInteger {
public:
    explicit IntegerWithMutex(int value);

    void Inc() override;

    ~IntegerWithMutex() override;

private:
    std::unique_ptr<pthread_mutex_t> mutex_;
};