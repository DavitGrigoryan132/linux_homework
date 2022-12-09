#pragma once

#include <pthread.h>

class AtomicInteger {
public:
    explicit AtomicInteger(int value);

    [[nodiscard]] int GetValue() const;

    virtual void Inc() = 0;

    virtual ~AtomicInteger() = 0;

protected:
    int value_;
};
