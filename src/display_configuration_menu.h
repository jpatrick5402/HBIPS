#include <pcap/pcap.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
std::unordered_map<std::string, int> display_configuration_menu() {
    std::unordered_map<std::string, int> result_map;
    bool confirmed;
    std::string user_confirmation_input;

    std::vector<std::string> options = {"Ping Logging", "DDos Detection Logging", "Block unfriendly IP addresses"};
    std::cout << "Configuration Menu" << '\n';
    do { // until user confirms their input
        for (std::string i : options) {
            std::string user_input;
            do { // until user gives y or n
                user_input = "";
                std::cout << "Enable "<< i << "? (y/n): ";
                std::cin >> user_input;
                if (user_input[0] == 'y') {
                    result_map[i] = 1;
                } else if (user_input[0] == 'n') {
                    result_map[i] = 0;
                }
                if (user_input[0] != 'y' && user_input[0] != 'n') {
                    std::cout << "Error: Please enter \"y\" or \"n\"" << '\n';
                }
            } while (user_input[0] != 'y' && user_input[0] != 'n');
        }
        do { // until user gives y or n
            user_confirmation_input = "";
            std::cout << "\nPlease confirm the configuration." << '\n';
            for (auto i : result_map) { // print all configs
                std::cout << i.first << ": " << i.second << '\n';
            }
            std::cout << "Is the above configuration correct? (y/n): ";
            std::cin >> user_confirmation_input;
        } while (user_confirmation_input[0] != 'y' && user_confirmation_input[0] != 'n');

        if (user_confirmation_input[0] == 'y') {
            confirmed = true;
        } else if (user_confirmation_input[0] == 'n') {
            confirmed = false;
        }
    } while (!confirmed);

    return result_map;
}
