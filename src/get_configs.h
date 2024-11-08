#include <pcap/pcap.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

std::unordered_map<std::string, int> get_configs() {
    // declaring variables to be used in program
    std::unordered_map<std::string, int> result_map;
    bool confirmed;
    std::string prior_input;
    std::string user_input;
    std::string user_confirmation_input;
    std::string line;
    std::vector<std::string> options;

    std::cout << "Configuration Menu" << '\n';
    do { // until user confirms their input
        do { // until user gives y or n
            prior_input = "";
            std::cout << "Would you like to use prior configuration? (y/n): ";
            std::cin >> prior_input;
            if (prior_input[0] != 'y' && prior_input[0] != 'n') {
                std::cout << "Error: Please enter \"y\" or \"n\"" << '\n';
            }
        } while (prior_input[0] != 'y' && prior_input[0] != 'n');

        if (prior_input[0] == 'y') {
            std::ifstream config_file("src/.configuration");
            while (getline(config_file, line)) {
                result_map[line.substr(0,line.find('='))] = stoi(line.substr(line.find('=') + 1, 1));
            }
            config_file.close();
        } else if (prior_input[0] == 'n') {
            std::ifstream config_file("src/.configuration_items");
            while (getline(config_file, line)) {
                options.push_back(line);
            }
            config_file.close();
            for (std::string i : options) {
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
            std::ofstream config_file_out("src/.configuration");
            for (auto i : result_map) {
                config_file_out << i.first << "=" << i.second << '\n';
            }
            config_file.close();
        }
        do { // until user gives y or n
            user_confirmation_input = "";
            std::cout << "\nPlease confirm the configuration." << '\n';
            for (auto i : result_map) { // print all configs
                std::cout << i.first << ": " << i.second << '\n';
            }
            std::cout << "Is the above configuration correct? (y/n): ";
            std::cin >> user_confirmation_input;
            if (user_confirmation_input[0] != 'y' && user_confirmation_input[0] != 'n') {
                std::cout << "Error: Please enter \"y\" or \"n\"" << '\n';
            }
        } while (user_confirmation_input[0] != 'y' && user_confirmation_input[0] != 'n');

        if (user_confirmation_input[0] == 'y') {
            confirmed = true;
        } else if (user_confirmation_input[0] == 'n') {
            confirmed = false;
        }
    } while (!confirmed);

    return result_map;
}
