#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <numeric>
#include <unistd.h>
#include <sys/wait.h>

struct pipeMsg {
    int i;
    int j;
    int sum;
};

auto arraySum(const std::vector<int>& arr, int start_idx, int end_idx) {
    return std::accumulate(arr.begin() + start_idx, arr.begin() + end_idx, 0);
}

int main() {
    int n;
    std::cout << "Write size of array (N > 1 000 000): ";
    std::cin >> n;

    std::vector<int> arr(n);

    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<int> dist {1, 100};
    
    auto gen = [&dist, &mersenne_engine](){
                   return dist(mersenne_engine);
               };

    std::generate(arr.begin(), arr.end(), gen);

    int m;
    std::cout << "Write number of processes: ";
    std::cin >> m;

    int sum = 0;
    int fd[2];
    pipe(fd);
    std::vector<int> partial_sums;

    for (std::size_t idx = 0; idx < m; ++idx) {
        if (fork() == 0) {
            // child process
            close(fd[0]); // close read end of the pipe
            pipeMsg pm;
            pm.i = idx * static_cast<int>(arr.size() / m);
            if (idx != m - 1) {
                pm.j = idx * static_cast<int>(arr.size() / m) + static_cast<int>(arr.size() / m);
            }
            else {
                pm.j = arr.size();
            }

            pm.sum = arraySum(arr, pm.i, pm.j);
            std::cout << "child " << idx << " sum: " << pm.sum << "\n";
            write(fd[1], &pm, sizeof(pipeMsg));
            close(fd[1]);
            exit(0); // exit child process
        }
    }
    // parent process
    close(fd[1]); // close write end of the pipe
    for(int i = 0; i < m; i++){
        wait(NULL);

        pipeMsg children_sum;
        read(fd[0], &children_sum, sizeof(pipeMsg));

        partial_sums.push_back(children_sum.sum);
    }
    close(fd[0]);

    std::cout << "Result: " << std::accumulate(partial_sums.begin(), partial_sums.end(), 0) << "\n";
    std::cout << "STL result: " << std::accumulate(arr.begin(), arr.end(), 0) << "\n";

    return 0;
}


