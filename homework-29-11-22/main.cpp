#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <pthread.h>

struct OperationCalculator {
    explicit OperationCalculator(int num1, int num2, const char* operation):
     num1_{num1}, 
     num2_{num2} {
         SetOperation(operation);
    }

    int DoOperation() {
        if (operation_ == "s") {
            return num1_ + num2_;
        }
        else if (operation_ == "m") {
            return num1_ * num2_;
        }
        // if operation == "ss"
        return num1_ * num1_ + num2_ * num2_;
    }

    std::string ToString() {
        std::string file_output_string;
        file_output_string += std::to_string(num1_);
        file_output_string += " ";
        file_output_string += operation_;
        file_output_string += " ";
        file_output_string += std::to_string(num2_);
        file_output_string += " = ";
        file_output_string += std::to_string(DoOperation());
        file_output_string += "\n";
	return file_output_string;
    }

private:
    void SetOperation(const char* operation) {
        // addition
        if (strcmp(operation, "s")) {
            operation_ = operation;
        }
        // multiplication
        else if (strcmp(operation, "m")) {
            operation_ = operation;
        }
        // addition of squares
        else if (strcmp(operation, "ss")) {
            operation_ = "ss";
        }
    }

    std::string operation_;
    int num1_;
    int num2_;
};

struct ArgumentsForThreads {
    std::size_t thread_index;
    int num1;
    int num2;
    char* operation;
};

void* CreateFileAndWriteOperation(void* args) {
    auto arguments = static_cast<ArgumentsForThreads*>(args);

    OperationCalculator calculator{arguments->num1, arguments->num2, arguments->operation};

    std::string filename{"out_"};
    filename += std::to_string(arguments->thread_index + 1);
    filename += ".txt";
    
    auto fd = open(filename.c_str(), O_CREAT | O_WRONLY, 0666);

    auto file_text = calculator.ToString();
    
    write(fd, file_text.c_str(), sizeof(char) * file_text.length());
    return nullptr;
}

int main(int argc, char** argv) {
    if (argc <= 2 and (argc - 2) % 3 != 0) {
        return 1;
    }   
    
    auto number_of_threads = std::atoi(argv[1]);

    std::vector<pthread_t> threads(number_of_threads);
    std::vector<ArgumentsForThreads> arguments(number_of_threads);
    
    for (std::size_t i = 0; i < threads.size(); ++i) {
        arguments[i].thread_index = i;
        arguments[i].num1 = std::atoi(argv[2 + i * 3]);
        arguments[i].num2 = std::atoi(argv[2 + i * 3 + 1]);
        arguments[i].operation = argv[2 + i * 3 + 2];

        pthread_create( &threads[i], NULL, CreateFileAndWriteOperation, &arguments[i]);
    }

    for (auto& thread : threads) {
        pthread_join(thread, NULL);
    }

    return 0;
}

