#pragma once

#include "atomic_integer.h"

#include <semaphore.h>
#include <memory>

class IntegerWithSemaphore : public AtomicInteger {
public:
    explicit IntegerWithSemaphore(int value);

    void Inc() override;

    ~IntegerWithSemaphore() override;

private:
    std::unique_ptr<sem_t> semaphore_;
};