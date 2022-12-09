#include "integer/atomic_integer.h"
#include "integer/integer_with_mutex.h"
#include "integer/integer_with_semaphore.h"
#include "integer/integer_with_spinlock.h"

#include <iostream>
#include <pthread.h>
#include <vector>
#include <chrono>


void *inc(void *arg) {
    auto integer = (AtomicInteger *) arg;
    int inc_num = 10000;
    for (int i = 0; i < inc_num; i++) {
        integer->Inc();
    }
    return nullptr;
}

int sync_increment(AtomicInteger *integer, int thread_num) {
    std::vector<pthread_t> threads(thread_num);

    for (int i = 0; i < thread_num; i++) {
        if (pthread_create(&threads[i], nullptr, inc, static_cast<void *>(integer)) != 0) {
            throw std::runtime_error("Cannot create thread");
        }
    }

    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(threads[i], nullptr) != 0) {
            throw std::runtime_error("Cannot join thread");
        }
    }

    return integer->GetValue();
}

int main() {
    int thread_num = 64;

    AtomicInteger *integer_with_mutex = new IntegerWithMutex(0);
    AtomicInteger *integer_with_spinlock = new IntegerWithSpinlock(0);
    AtomicInteger *integer_with_semaphore = new IntegerWithSemaphore(0);

    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = sync_increment(integer_with_mutex, thread_num);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::cout << "Mutex:\n\tResult: " << result << "\ttime: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << "ns\n";

    start_time = std::chrono::high_resolution_clock::now();
    result = sync_increment(integer_with_semaphore, thread_num);
    end_time = std::chrono::high_resolution_clock::now();

    std::cout << "Semaphore:\n\tResult: " << result << "\ttime: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << "ns\n";

    start_time = std::chrono::high_resolution_clock::now();
    result = sync_increment(integer_with_spinlock, thread_num);
    end_time = std::chrono::high_resolution_clock::now();

    std::cout << "Spinlock:\n\tResult: " << result << "\ttime: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << "ns\n";

    delete integer_with_spinlock;
    delete integer_with_semaphore;
    delete integer_with_mutex;
    return 0;
}
