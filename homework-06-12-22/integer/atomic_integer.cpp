#include "atomic_integer.h"

AtomicInteger::AtomicInteger(const int value) : value_{value} {}

int AtomicInteger::GetValue() const {
    return value_;
}

AtomicInteger::~AtomicInteger() = default;
