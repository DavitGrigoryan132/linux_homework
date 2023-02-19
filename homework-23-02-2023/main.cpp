#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>

 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* read_input_numbers(void*) {
    int val{};
    std::vector<int> numbers{};

    bool greater_10 = false;
    bool greater_50 = false;

    while (std::cin >> val) {
        numbers.push_back(val);

        if (numbers.size() > 10 && !greater_10){
            greater_10 = true;
            pthread_cond_signal(&cond);
        }
        else if (numbers.size() > 50 && !greater_50) {
            greater_50 = true;
            pthread_cond_signal(&cond);
        }
    }

    if (numbers.size() == 0) {
        throw std::runtime_error("Error: there are no elements given");
    }

    return nullptr;
}


void* writing_numbers_count(void*) {
    pthread_mutex_lock(&lock);

    pthread_cond_wait(&cond, &lock);
    std::cout << ">10\n";
        
    pthread_cond_wait(&cond, &lock);
    std::cout << ">50\n";

    pthread_mutex_unlock(&lock);

    return nullptr;
}
 
int main() {
    pthread_t tid1, tid2;
 
    pthread_create(&tid1, NULL, &read_input_numbers, NULL);
 
    pthread_create(&tid2, NULL, &writing_numbers_count, NULL);
 
    pthread_join(tid1, NULL);
    pthread_detach(tid2);
 
    return 0;
}
