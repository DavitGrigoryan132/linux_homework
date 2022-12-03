#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  

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

int main(int argc, char** argv) {
    if (argc <= 2 and (argc - 2) % 3 != 0) {
        return 1;
    }   
    
    auto number_of_pids = std::atoi(argv[1]);

    std::vector<pid_t> pids(number_of_pids);
    
    for (std::size_t i = 0; i < pids.size(); ++i) {
        if (fork() == 0){
            pids[i] = getpid();
        }

        if (getpid() == pids[i]) {
            int num1 = std::atoi(argv[2 + i * 3]);
            int num2 = std::atoi(argv[2 + i * 3 + 1]);
            char* operation = argv[2 + i * 3 + 2];

            OperationCalculator calculator{num1, num2, operation};

            std::string filename{"out_"};
            filename += std::to_string(i+1);
            filename += ".txt";
            
            auto fd = open(filename.c_str(), O_CREAT | O_WRONLY, 0666);

            auto file_text = calculator.ToString();
            
            write(fd, file_text.c_str(), sizeof(char) * file_text.length());

            close(fd);
        }
    }

    return 0;
}

