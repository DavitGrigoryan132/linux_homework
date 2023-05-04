#include "bank_system.h"
#include "accounts_shared_memory.h"

#include <string>
#include <sstream>

int main() {
    key_t key = ftok("/home", 111);

    std::string help_message{
        "- print_current_balance id\n"
        "- print_min_balance id\n"
        "- print_max_balance id\n"
        "- freeze id\n"
        "- unfreeze id\n"
        "- transfer source_id destination_id money\n"
        "- fill_all money\n"
        "- withdraw_all money\n"
        "- set_min id new_min_balance\n"
        "- set_max id new_max_balance\n"
        "- help\n"
        "- exit"
    };
    std::cout << help_message << "\n";
    BankSystem bank_system{key};

    std::string line;

    while (std::getline(std::cin, line)) {
        std::stringstream command_line{ line };

        std::string command;

        command_line >> command;
        if (command == "print_current_balance") {
            int id = -1;
            command_line >> id;
            bank_system.PrintCurrentBalance(id);
        }
        else if (command == "print_min_balance") {
            int id = -1;
            command_line >> id;
            bank_system.PrintMinBalance(id);
        }
        else if (command == "print_max_balance") {
            int id = -1;
            command_line >> id;
            bank_system.PrintMaxBalance(id);
        }
        else if (command == "freeze") {
            int id = -1;
            command_line >> id;
            bank_system.Freeze(id);
        }
        else if (command == "unfreeze") {
            int id = -1;
            command_line >> id;
            bank_system.Unfreeze(id);
        }
        else if (command == "transfer") {
            int id_source = -1;
            int id_dest = -1;
            int money = 0;
            command_line >> id_source;
            command_line >> id_dest;
            command_line >> money;

            bank_system.Transaction(id_source, id_dest, money);
        }
        else if (command == "fill_all") {
            int money = 0;
            command_line >> money;
            bank_system.FillAllBalances(money);
        }
        else if (command == "withdraw_all") {
            int money = 0;
            command_line >> money;
            bank_system.WithdrawFromAllBalances(money);
        }
        else if (command == "set_min"){
            int new_min = 0;
            int id = -1;
            command_line >> id;
            command_line >> new_min;
            bank_system.SetMinBalance(id, new_min);
        }
        else if (command == "set_max"){
            int new_max = 0;
            int id = -1;
            command_line >> id;
            command_line >> new_max;
            bank_system.SetMaxBalance(id, new_max);
        }
        else if (command == "help") {
            std::cout << help_message << "\n";
        }
        else if (command == "exit") {
            break;
        }
    }

    return 0;
}
